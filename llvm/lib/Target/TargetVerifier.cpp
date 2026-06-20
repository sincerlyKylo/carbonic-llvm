//===-- TargetVerifier.cpp - Target-dependent IR Verifier ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the target-independent dispatcher for target-dependent
// IR verification. See llvm/Target/TargetVerifier.h for the design.
//
//===----------------------------------------------------------------------===//

#include "llvm/Target/TargetVerifier.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

// The registry maps Triple::ArchType (stored as unsigned, since there is no
// DenseMapInfo for the enum) to the factory that builds that target's
// TargetVerify. It is populated by backends from LLVMInitialize<T>Target().
static DenseMap<unsigned, TargetVerifyFactory> &getTargetVerifyRegistry() {
  static DenseMap<unsigned, TargetVerifyFactory> Registry;
  return Registry;
}

void llvm::registerTargetVerify(Triple::ArchType Arch,
                                TargetVerifyFactory Factory) {
  getTargetVerifyRegistry()[static_cast<unsigned>(Arch)] = std::move(Factory);
}

const TargetVerifyFactory *llvm::getTargetVerify(Triple::ArchType Arch) {
  DenseMap<unsigned, TargetVerifyFactory> &Registry = getTargetVerifyRegistry();
  auto It = Registry.find(static_cast<unsigned>(Arch));
  if (It == Registry.end())
    return nullptr;
  return &It->second;
}

PreservedAnalyses TargetVerifierPass::run(Module &M, ModuleAnalysisManager &AM) {
  // Use the module triple to decide which target-dependent verification to run.
  // If the target did not register a verifier, there is nothing target-specific
  // to do, so skip everything (including the generic verifier) and stay a no-op.
  const TargetVerifyFactory *Factory =
      getTargetVerify(M.getTargetTriple().getArch());
  if (!Factory)
    return PreservedAnalyses::all();

  bool Broken = false;

  // (1) Run the generic IR verifier. After the VerifierAMDGPU split, the
  // generic verifier dispatches to each target's IR-level checks (e.g. the
  // verifyAMDGPU* routines in VerifierAMDGPU.cpp for AMDGPU modules), so this
  // is how the target's "<Target>Verifier" runs.
  Broken |= verifyModule(M, &errs());

  // (2) Run the target's subtarget/feature-dependent verifier (TargetVerify).
  if (std::unique_ptr<TargetVerify> TV{(*Factory)(M)}) {
    for (Function &F : M) {
      if (F.isDeclaration())
        continue;
      if (!TV->run(F))
        Broken = true;
    }
    TV->MessagesStr.flush();
    if (!TV->Messages.empty())
      errs() << TV->Messages;
  }

  if (Broken) {
    if (FatalErrors)
      report_fatal_error("broken module found, compilation aborted!");
    errs() << "broken module found\n";
    return PreservedAnalyses::none();
  }
  return PreservedAnalyses::all();
}
