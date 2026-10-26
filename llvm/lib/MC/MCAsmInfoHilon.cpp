//===- MCAsmInfoDarwin.cpp - Darwin asm properties ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines target asm properties related what form asm statements
// should take in general on Darwin-based targets
//
//===----------------------------------------------------------------------===//

#include "llvm/MC/MCAsmInfoHilon.h"
#include "llvm/MC/MCDirectives.h"

using namespace llvm;


MCAsmInfoHilon::MCAsmInfoHilon(const MCTargetOptions &Options)
    : MCAsmInfo(Options) {
  // Common settings for all Darwin targets.
  // Syntax:
  LinkerPrivateGlobalPrefix = "l";
  HasSingleParameterDotFile = false;
  HasSubsectionsViaSymbols = true;

  AlignmentIsInBytes = false;
  COMMDirectiveAlignmentIsInBytes = false;
  LCOMMDirectiveAlignmentType = LCOMM::Log2Alignment;
  InlineAsmStart = " InlineAsm Start";
  InlineAsmEnd = " InlineAsm End";

  // Directives:
  HasWeakDefCanBeHiddenDirective = true;
  WeakRefDirective = "\t.weak_reference ";
  ZeroDirective = "\t.space\t";  // ".space N" emits N zeros.

  HiddenVisibilityAttr = MCSA_PrivateExtern;
  HiddenDeclarationVisibilityAttr = MCSA_Invalid;

  // Doesn't support protected visibility.
  ProtectedVisibilityAttr = MCSA_Invalid;

  HasDotTypeDotSizeDirective = false;
  HasNoDeadStrip = true;

  DwarfUsesRelocationsAcrossSections = false;
  SetDirectiveSuppressesReloc = true;
}


