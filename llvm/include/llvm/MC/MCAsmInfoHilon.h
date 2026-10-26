//===- MCAsmInfoHilon.h - Hilon asm properties ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines target asm properties related what form asm statements
// should take in general on Hilon-based targets
//
//===----------------------------------------------------------------------===//

#pragma once
#include "llvm/MC/MCAsmInfo.h"

namespace llvm {
    class LLVM_ABI MCAsmInfoHilon : public MCAsmInfo {
    public:
        explicit MCAsmInfoHilon(const MCTargetOptions &Options);
    };

} // end namespace llvm
