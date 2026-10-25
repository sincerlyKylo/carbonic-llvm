//===- MCAsmInfoHilon.h - Hilon asm properties ----------------*- C++ -*-===//
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

#pragma once
#include "llvm/MC/MCAsmInfo.h"

namespace llvm {
    class LLVM_ABI MCAsmInfoHilon : public MCAsmInfo {
    public:
        explicit MCAsmInfoHilon(const MCTargetOptions &Options);
        void printSwitchToSection(const MCSection &, uint32_t, const Triple &,
                                  raw_ostream &) const final;
        bool useCodeAlign(const MCSection &Sec) const final;

        /// True if the section is atomized using the symbols in it.
        /// This is false if the section is atomized based on its contents (MachO' __TEXT,__cstring for
        /// example).
        static bool isSectionAtomizableBySymbols(const MCSection &Section);
    };

} // end namespace llvm
