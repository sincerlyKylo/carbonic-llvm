//===--- CGGrafitCXX.cpp - Emit LLVM Code for C++ Grafit extensions -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This contains code dealing with code generation of C++ expressions
//
//===----------------------------------------------------------------------===//

#include "CGDebugInfo.h"
#include "CGObjCRuntime.h"
#include "CodeGenFunction.h"




using namespace clang;
using namespace CodeGen;



llvm::Value* CodeGenFunction::EmitGrafitCXXConstructAt(const GrafitCXXConstructAt *E) {
  Address Dest = EmitPointerWithAlignment(E->getDestination());

  AggValueSlot Slot = AggValueSlot::forAddr(
    Dest,
    Qualifiers(),
    AggValueSlot::IsDestructed,
    AggValueSlot::DoesNotNeedGCBarriers,
    AggValueSlot::IsNotAliased,
    AggValueSlot::MayOverlap
);
  if (const auto *Ctor = E->getConstructor()) {
    EmitCXXConstructExpr(Ctor, Slot);
  }
  else if (Expr *Init = E->getInitializer()) {
    llvm::Value *Value = EmitScalarExpr(Init);
    Builder.CreateStore(Value, Dest);
  }

  return Dest.getBasePointer();
}