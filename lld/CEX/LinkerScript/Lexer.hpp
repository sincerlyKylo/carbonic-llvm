#pragma once
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/Support/SourceMgr.h"

namespace lld {
namespace cex {
class Lexer {
public:
  enum class TokenType : uint8_t {
    // Keywords
    KwProgram,
    KwSections,
    KwLoadCommand,
    KwArchitecture,
    KwEntryPoint,
    KwPermissions,
    KwUseSections,
    
    // Identifiers, strings, numbers
    Identifier,  // e.g., "MainSubsystem", "ARM64", "RX"
    String,      // e.g., "TextSegment"
    Number,      // e.g., 0x400000, 4096
    Symbol,      // e.g., .text, .rodata, _start, _data_start
    
    // Operators & Delimiters
    Equals,      // =
    Semicolon,   // ;
    LCurly,      // {
    RCurly,      // }
    LSquare,     // [
    RSquare,     // ]
    Comma,       // ,
    Star,        // *
    
    // Comments & Special
    Comment,     // /* ... */ 
    EndOfFile    // Sentinel value for the end of the file
};

TokenType getKeywordType(llvm::StringRef Str) {
    return llvm::StringSwitch<TokenType>(Str)
        .Case("PROGRAM", TokenType::KwProgram)
        .Case("SECTIONS", TokenType::KwSections)
        .Case("LOAD_COMMAND", TokenType::KwLoadCommand)
        .Case("Architecture", TokenType::KwArchitecture)
        .Case("EntryPoint", TokenType::KwEntryPoint)
        .Case("Permissions", TokenType::KwPermissions)
        .Case("UseSections", TokenType::KwUseSections)
        .Default(TokenType::Identifier);
}

struct Token {
    TokenType Type;
    llvm::StringRef Lexeme;  // Points into the source buffer
    llvm::SMLoc Loc;         // Start location in the source file
    // Helper to check type quickly
    bool is(TokenType t) const { return Type == t; }
    bool is_not(TokenType t) const { return Type != t; }
};

    Lexer(llvm::StringRef FilePath);
    Token NextToken();

private:
    llvm::SourceMgr SrcMgr;
    unsigned MainFileID;       
    const char* CurrentChar;
    llvm::SMLoc CurrentLoc;

    void Error(const char* Message, llvm::SMLoc Loc);
};

}
}


