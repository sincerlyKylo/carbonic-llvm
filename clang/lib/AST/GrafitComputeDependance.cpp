//===- GrafitComputeDependence.cpp ----------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#include "clang/AST/ComputeDependence.h"
#include "clang/AST/DependenceFlags.h"
#include "clang/AST/Expr.h"
#include "clang/AST/ExprCXX.h"

using namespace clang;

ExprDependence clang::computeDependence(GrafitCXXConstructAt *E) {
    if (E->hasInitializer()) return E->getDestination()->getDependence() | E->getInitializer()->getDependence();
    return E->getDestination()->getDependence();
}