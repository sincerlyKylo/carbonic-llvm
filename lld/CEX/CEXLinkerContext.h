#pragma once
#include "ArgumentParser.h"
#include "CEXLinkerServer.h"
#include "lld/Common/CommonLinkerContext.h"
#include <cstdint>

namespace lld {
namespace cex {

class CEXLinkerContext : public CommonLinkerContext {
  public:
    enum FileType {
      Executable,
      Kernel,
      DynamicLibrary,
      StaticLibrary,
      KernelDriver,
      UserDriver,
      UnknownType
    };

    struct LinkerOptions {
    // Output configuration
    llvm::StringRef OutputFile = "a.cex"; // -o
    llvm::StringRef EntrySymbol = "main"; //-e / --entry
    uint64_t ImageBase = 0x400000; // --image-base / -b
    uint64_t PageSize = 0x1000; // 4KB default for AArch64 compatibility, --page-size = <hex>
    uint64_t StackSize = 0; //0 = Don't assume a custom stack size, --stack-size = <hex>
    FileType Type;
    // Core inputs
    llvm::SmallVector<llvm::StringRef, 16> ObjectFiles; //files, positrional arguments wiith nothing else 
    llvm::SmallVector<llvm::StringRef, 6>  LibrarySearchPaths; //-L
    llvm::SmallVector<llvm::StringRef, 16> LibrariesToLink; //-l
    llvm::SmallVector<llvm::StringRef, 6>  DynamicLibrariesSearchPaths; //-D
    llvm::SmallVector<llvm::StringRef, 16> DynamicLibrariesToLink; //-d<symbol>
    llvm::SmallVector<llvm::StringRef, 16> FrameworksToLink; //-f <file> / --framework <file>

    llvm::SmallVector<llvm::StringRef, 16>  UndefinedSymbols; // For -u options
    
    // System configuration
    //--sysroot
    llvm::StringRef MapFile = ""; //-map = <file>

    // Behavioral flags
    bool ShowHelp = false; //-h or --help
    bool ForceNoPIE = false; //--force-no-pie
    bool ForceNoPIC = false; //--force-no-pie
    bool Static = false; //--static or -s
    bool Strip = false;  //--strip
    bool Demangle = true; //-d or --demangle
    bool Verbose = false; //-v or --verbose
    
    // Optimization & Validation
    bool GCSections = false;           // --gc-sections
    bool PrintGCRemoved = false;       // --print-gc-sections
    bool UnresolvedSymbolsFatal = true; // Error out on missing links //--fatal-unrezolved-symbols
};


  CEXLinkerContext();
  CEXLinkerContext(const CEXLinkerContext &) = delete;
  CEXLinkerContext &operator=(const CEXLinkerContext &) = delete;
  ~CEXLinkerContext() = default;

  int BuildOptions();

  CEXLinkerServer Server;
  ArgumentParser ArgParser;
  LinkerOptions Options;
};

} // namespace cex
} // namespace lld