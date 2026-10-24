//===--- SemaExprCXX.cpp - Semantic Analysis for Grafit --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Implements semantic analysis for C++ Grafit.
///
//===----------------------------------------------------------------------===//

#include "TreeTransform.h"
#include "clang/AST/CXXInheritance.h"
#include "clang/AST/ExprCXX.h"
#include "clang/Basic/DiagnosticSema.h"
#include "clang/Sema/DeclSpec.h"
#include "clang/Sema/EnterExpressionEvaluationContext.h"
#include "clang/Sema/Initialization.h"

using namespace clang;
using namespace sema;

ExprResult Sema::ActOnGrafitExtensionCXXConstructAt(SourceLocation StartLoc, SourceLocation LParenLoc, SourceLocation EndLoc,
                                Declarator& D,
                                ExprVector& Exprs) {

  //Declarator validation
  if (D.isInvalidType()) {
    return ExprError();
  }
  TypeSourceInfo* TInfo = GetTypeForDeclarator(D);

  if (D.isInvalidType())
    return ExprError();

  QualType ConstructType = TInfo->getType();

  if (ConstructType->isArrayType())
  {
    return ExprError(
        Diag(D.getSourceRange().getBegin(),
             diag::err_grafit_construct_at_array_type));
  }
  if (ConstructType->isReferenceType())
  {
    return ExprError(
        Diag(D.getSourceRange().getBegin(),
             diag::err_grafit_construct_at_reference_type) << ConstructType);
  }
  if (ConstructType->isFunctionType())
  {
    return ExprError(
        Diag(D.getSourceRange().getBegin(),
             diag::err_grafit_construct_at_function_type) << ConstructType);
  }
  if (ConstructType->isIncompleteType())
  {
    return ExprError(
        Diag(D.getSourceRange().getBegin(),
             diag::err_grafit_construct_at_incompatible_type) << ConstructType);
  }
  if (!ConstructType->isObjectType())
  {
    return ExprError(
        Diag(D.getSourceRange().getBegin(),
             diag::err_grafit_construct_at_incomplete_type) << ConstructType);
  }


  //Expression splitting and validation
  Expr* Destination;
  ExprVector ConstructorArguments;

  if (Exprs.empty()) {
    return ExprError(Diag(StartLoc, diag::err_grafit_construct_at_insufficient_args)
      );
  }
  Destination = Exprs[0];
  if (isa<RecoveryExpr>(Destination))
    return ExprError();

  if(!Destination->getType()->isPointerType()) {
    return ExprError(
        Diag(D.getSourceRange().getBegin(),
             diag::err_grafit_construct_at_arg_not_pointer));
  }

  for (size_t i = 1; i < Exprs.size(); ++i) {
    ConstructorArguments.push_back(Exprs[i]);
  }
  return Sema::BuildGrafitExtensionCXXConstructAt(StartLoc, LParenLoc, EndLoc, D, *Destination, ConstructorArguments);
}


ExprResult Sema::BuildGrafitExtensionCXXConstructAt(SourceLocation StartLoc, SourceLocation LParenLoc, SourceLocation EndLoc, Declarator& D,
                              Expr& Destination, ExprVector& ConstructorArgs) {
  TypeSourceInfo* TInfo = GetTypeForDeclarator(D);
  QualType TQual = TInfo->getType();
  Expr* Init = nullptr;

  bool IsDependent = TInfo->getType()->isDependentType() ||
                    Destination.isTypeDependent() ||
                    Expr::hasAnyTypeDependentArguments(ConstructorArgs);
  if (IsDependent) {
    Expr *DependentArgsExpr = ParenListExpr::Create(Context, LParenLoc, ConstructorArgs, EndLoc);
    Init = cast<Expr>(DependentArgsExpr);
  }

  else {
    InitializedEntity Entity = InitializedEntity::InitializeNew(StartLoc, TQual, InitializedEntity::NewArrayKind::KnownLength);
    InitializationKind Kind = InitializationKind::InitializationKind::CreateValue(
        StartLoc, LParenLoc, EndLoc);
    if (ConstructorArgs.empty()) {
      Kind = InitializationKind::CreateValue(
          StartLoc, LParenLoc, EndLoc);
    } else {
      Kind = InitializationKind::CreateDirect(
          StartLoc, LParenLoc, EndLoc);
      }
    MultiExprArg InitArgs(ConstructorArgs);

    InitializationSequence InitSeq(*this, Entity, Kind, InitArgs);
    ExprResult InitResult = InitSeq.Perform(*this, Entity, Kind, InitArgs);
    Init = InitResult.get();
  }

  return GrafitCXXConstructAt::Create(Context, TQual, StartLoc, D.getBeginLoc(), EndLoc, &Destination, Init);
}

ExprResult Sema::BuildGrafitCXXConstructAtInit(
    SourceLocation StartLoc,
    SourceLocation LParenLoc,
    SourceLocation EndLoc,
    QualType TQual,
    ExprVector &ConstructorArgs) {

  InitializedEntity Entity =
      InitializedEntity::InitializeNew(
          StartLoc,
          TQual,
          InitializedEntity::NewArrayKind::KnownLength);

  InitializationKind Kind =
      InitializationKind::CreateValue(
          StartLoc, LParenLoc, EndLoc);

  if (!ConstructorArgs.empty()) {
    Kind = InitializationKind::CreateDirect(
        StartLoc, LParenLoc, EndLoc);
  }

  MultiExprArg InitArgs(ConstructorArgs);

  InitializationSequence InitSeq(
      *this, Entity, Kind, InitArgs);

  return InitSeq.Perform(
      *this, Entity, Kind, InitArgs);
}
