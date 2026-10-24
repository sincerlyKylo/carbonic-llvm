//===--- ParseExprCXX.cpp - C++ Expression Parsing ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the Grafit parsing implementation for C++.
//
//===----------------------------------------------------------------------===//
#include "clang/AST/ASTContext.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/Basic/DiagnosticParse.h"
#include "clang/Basic/TokenKinds.h"
#include "clang/Parse/Parser.h"
#include "clang/Parse/RAIIObjectsForParser.h"
#include "clang/Sema/DeclSpec.h"

using namespace clang;

static int SelectDigraphErrorMessage(tok::TokenKind Kind) {
    switch (Kind) {
        // template name
        case tok::unknown:             return 0;
            // casts
        case tok::kw_addrspace_cast:   return 1;
        case tok::kw_const_cast:       return 2;
        case tok::kw_dynamic_cast:     return 3;
        case tok::kw_reinterpret_cast: return 4;
        case tok::kw_static_cast:      return 5;
        default:
            llvm_unreachable("Unknown type for digraph error message.");
    }
}

// Suggest fixit for "<::" after a cast.
static void FixDigraph(Parser &P, Preprocessor &PP, Token &DigraphToken,
                       Token &ColonToken, tok::TokenKind Kind, bool AtDigraph) {
    // Pull '<:' and ':' off token stream.
    if (!AtDigraph)
        PP.Lex(DigraphToken);
    PP.Lex(ColonToken);

    SourceRange Range;
    Range.setBegin(DigraphToken.getLocation());
    Range.setEnd(ColonToken.getLocation());
    P.Diag(DigraphToken.getLocation(), diag::err_missing_whitespace_digraph)
        << SelectDigraphErrorMessage(Kind)
        << FixItHint::CreateReplacement(Range, "< ::");

    // Update token information to reflect their change in token type.
    ColonToken.setKind(tok::coloncolon);
    ColonToken.setLocation(ColonToken.getLocation().getLocWithOffset(-1));
    ColonToken.setLength(2);
    DigraphToken.setKind(tok::less);
    DigraphToken.setLength(1);

    // Push new tokens back to token stream.
    PP.EnterToken(ColonToken, /*IsReinject*/ true);
    if (!AtDigraph)
        PP.EnterToken(DigraphToken, /*IsReinject*/ true);
}
clang::ExprResult clang::Parser::ParseGrafitCXXConstructAtExpression(SourceLocation Start) {
    const char *KeywordName = "__grafit_construct_at";
    tok::TokenKind Kind = Tok.getKind();
    SourceLocation OpLoc = ConsumeToken();

    // 1. Handle the '<::' digraph edge-case
    if (Tok.is(tok::l_square) && Tok.getLength() == 2) {
        clang::Token Next = NextToken();
        if (Next.is(tok::colon) && areTokensAdjacent(Tok, Next))
            FixDigraph(*this, PP, Tok, Next, Kind, /*AtDigraph*/true);
    }

    // 2. Expect '<'
    clang::SourceLocation LAngleBracketLoc = Tok.getLocation();
    if (ExpectAndConsume(tok::less, diag::err_expected_less_after, KeywordName))
        return ExprError();

    // 3. Parse Type T using full DeclSpec + Declarator logic
    clang::DeclSpec DS(getAttrFactory());
    ParseSpecifierQualifierList(DS, clang::AS_none, DeclSpecContext::DSC_type_specifier);

    clang::Declarator DeclaratorInfo(DS, clang::ParsedAttributesView::none(),
                                      clang::DeclaratorContext::TypeName);
    ParseDeclarator(DeclaratorInfo);

    // 4. Expect '>' with matching error hint back to '<'
    clang::SourceLocation RAngleBracketLoc = Tok.getLocation();
    if (ExpectAndConsume(tok::greater))
        return ExprError(Diag(LAngleBracketLoc, diag::note_matching) << tok::less);

    // 5. Expect '('
    SourceLocation LParenLoc = Tok.getLocation();
    clang::BalancedDelimiterTracker T(*this, tok::l_paren);
    if (T.expectAndConsume(diag::err_expected_lparen_after, KeywordName))
        return ExprError();

    // 6. Parse expression list (ptr, args...) instead of a single expression
    ExprVector Exprs;
    if (Tok.isNot(tok::r_paren)) {
        if (ParseExpressionList(Exprs, llvm::function_ref<void()>(), true)) {
            T.skipToEnd();
            return ExprError();
        }
    }
    auto EndLoc = T.getCloseLocation();
    // 7. Match closing ')'
    T.consumeClose();

    return getActions().ActOnGrafitExtensionCXXConstructAt(OpLoc, LParenLoc, EndLoc,
      DeclaratorInfo, Exprs);
}