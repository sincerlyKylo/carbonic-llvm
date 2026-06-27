#pragma once
#include "ArgumentParser.h"
#include "CEXLinkerServer.h"
#include "lld/Common/CommonLinkerContext.h"

namespace lld {
namespace cex {

class CEXLinkerContext : public CommonLinkerContext {
    struct Options {
    // Output configuration
    llvm::StringRef OutputFile = "a.cex";
    llvm::StringRef EntrySymbol = "main";
    uint64_t ImageBase = 0x400000;
    uint64_t MaxPageSize = 0x1000; // 4KB default for AArch64 compatibility
    
    // Core inputs
    llvm::SmallVector<llvm::StringRef, 16> ObjectFiles;
    llvm::SmallVector<llvm::StringRef, 16> LibrarySearchPaths;
    llvm::SmallVector<llvm::StringRef, 16> LibrariesToLink;
    llvm::SmallVector<llvm::StringRef, 16> FrameworksToLink;
    llvm::SmallVector<llvm::StringRef, 4> UndefinedSymbols; // For -u options
    
    // System configuration
    llvm::StringRef Sysroot = "";
    llvm::StringRef MapFile = "";

    // Behavioral flags
    bool PIE = false;
    bool Static = false;
    bool Strip = false;
    bool Demangle = true;
    bool EmitMap = false;
    
    // Optimization & Validation
    bool GCSections = false;           // --gc-sections
    bool PrintGCRemoved = false;       // --print-gc-sections
    bool UnresolvedSymbolsFatal = true; // Error out on missing links
};

public:
  CEXLinkerContext();
  CEXLinkerContext(const CEXLinkerContext &) = delete;
  CEXLinkerContext &operator=(const CEXLinkerContext &) = delete;
  ~CEXLinkerContext() = default;

  CEXLinkerServer Server;
  ArgumentParser ArgParser;
};

} // namespace cex
} // namespace lld