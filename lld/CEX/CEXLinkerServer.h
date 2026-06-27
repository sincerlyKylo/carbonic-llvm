#pragma once
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/SmallVector.h"
namespace lld {
namespace cex {

class CEXLinkerContext;

class CEXLinkerServer {
    struct Options {
        llvm::StringRef OutputFile = "a.cex";
        llvm::StringRef EntrySymbol = "main";
        uint64_t ImageBase = 0x400000;
            
        llvm::SmallVector<llvm::StringRef, 16> ObjectFiles;
        llvm::SmallVector<llvm::StringRef, 16> LibrarySearchPaths;
        llvm::SmallVector<llvm::StringRef, 16> LibrariesToLink;
        llvm::SmallVector<llvm::StringRef, 16> FrameworksToLink;
        
        bool PIE = false;
        bool Static = false;
        bool Strip = false;
    };

public:
    CEXLinkerServer(CEXLinkerContext &ctx) : ctx(ctx) {}

    void linkerMain();
    
private:
    CEXLinkerContext& ctx;
};

} // namespace cex 
} // namespace lld