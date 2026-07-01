#include "Tests.h"

void TestParser(const lld::cex::ArgumentParser& Parser) {
    llvm::outs() << "========================================\n";
    llvm::outs() << "        ARGUMENT PARSER TEST            \n";
    llvm::outs() << "========================================\n\n";

    // 1. Dump Parsed Key-Value Arguments
    llvm::outs() << "--- Arguments Found ---\n";
    const auto& Args = Parser.GetArguments();
    if (Args.empty()) {
        llvm::outs() << "  (None)\n";
    } else {
        for (const auto& It : Args) {
            llvm::StringRef Key = It.first();
            std::string Value = It.second;
            
            llvm::outs() << "  Key:   " << Key << "\n"
                         << "  Raw:   " << Value << "\n";
                         
            // Demonstrate GetArgumentVector parsing the comma-separated values
            auto Vector = Parser.GetArgumentVector(Key);
            llvm::outs() << "  Split: [";
            for (size_t i = 0; i < Vector.size(); ++i) {
                llvm::outs() << "'" << Vector[i] << "'";
                if (i + 1 < Vector.size()) llvm::outs() << ", ";
            }
            llvm::outs() << "]\n\n";
        }
    }

    // 2. Dump Flags
    llvm::outs() << "--- Flags Found ---\n";
    const auto& Flags = Parser.GetFlags();
    if (Flags.empty()) {
        llvm::outs() << "  (None)\n";
    } else {
        for (const auto& It : Flags) {
            llvm::outs() << "  - " << It.first() << " (Active)\n";
        }
    }
    llvm::outs() << "\n";

    // 3. Dump Positional Files
    llvm::outs() << "--- Files / Positional Arguments ---\n";
    const auto& Files = Parser.GetFiles();
    if (Files.empty()) {
        llvm::outs() << "  (None)\n";
    } else {
        for (size_t i = 0; i < Files.size(); ++i) {
            llvm::outs() << "  [" << i << "]: " << Files[i] << "\n";
        }
    }
    llvm::outs() << "========================================\n";
    
    // Explicitly flush outs() to ensure everything hits the console immediately
    llvm::outs().flush();
}

static llvm::StringRef FileTypeToString(lld::cex::CEXLinkerContext::FileType Type) {
    using namespace lld::cex;
    switch (Type) {
        case CEXLinkerContext::Executable:     return "Executable";
        case CEXLinkerContext::Kernel:         return "Kernel";
        case CEXLinkerContext::DynamicLibrary: return "DynamicLibrary";
        case CEXLinkerContext::StaticLibrary:  return "StaticLibrary";
        case CEXLinkerContext::KernelDriver:   return "KernelDriver";
        case CEXLinkerContext::UserDriver:     return "UserDriver";
        case CEXLinkerContext::UnknownType:    return "UnknownType";
    }
    return "InvalidType";
}

void PrintOptions(const lld::cex::CEXLinkerContext::LinkerOptions Options) {
    llvm::outs() << "==================================================\n";
    llvm::outs() << "               CEX Linker Options                 \n";
    llvm::outs() << "==================================================\n";

    // --- Output Configuration ---
    llvm::outs() << "[Output Configuration]\n";
    llvm::outs() << "  Output File:    " << Options.OutputFile << "\n";
    llvm::outs() << "  Entry Symbol:   " << Options.EntrySymbol << "\n";
    llvm::outs() << "  Platform Type:  " << FileTypeToString(Options.Type) << "\n";
    llvm::outs() << "  Image Base:     " << llvm::format_hex(Options.ImageBase, 10) << "\n";
    llvm::outs() << "  Page Size:      " << llvm::format_hex(Options.PageSize, 6) << " (" << (Options.PageSize / 1024) << " KB)\n";
    llvm::outs() << "  Stack Size:     " << llvm::format_hex(Options.StackSize, 10);
    if (Options.StackSize == 0) {
        llvm::outs() << " (Default / Unspecified)";
    }
    llvm::outs() << "\n\n";

    // --- Core Inputs ---
    llvm::outs() << "[Core Inputs]\n";
    
    llvm::outs() << "  Object Files (" << Options.ObjectFiles.size() << "):\n";
    for (const auto &File : Options.ObjectFiles)
        llvm::outs() << "    - " << File << "\n";

    llvm::outs() << "  Static Library Search Paths (" << Options.LibrarySearchPaths.size() << "):\n";
    for (const auto &Path : Options.LibrarySearchPaths)
        llvm::outs() << "    - " << Path << "\n";

    llvm::outs() << "  Static Libraries to Link (" << Options.LibrariesToLink.size() << "):\n";
    for (const auto &Lib : Options.LibrariesToLink)
        llvm::outs() << "    - -l" << Lib << "\n";

    llvm::outs() << "  Dynamic Library Search Paths (" << Options.DynamicLibrariesSearchPaths.size() << "):\n";
    for (const auto &Path : Options.DynamicLibrariesSearchPaths)
        llvm::outs() << "    - " << Path << "\n";

    llvm::outs() << "  Dynamic Libraries to Link (" << Options.DynamicLibrariesToLink.size() << "):\n";
    for (const auto &DLib : Options.DynamicLibrariesToLink)
        llvm::outs() << "    - -d" << DLib << "\n";

    llvm::outs() << "  Frameworks to Link (" << Options.FrameworksToLink.size() << "):\n";
    for (const auto &Fw : Options.FrameworksToLink)
        llvm::outs() << "    - " << Fw << "\n";

    llvm::outs() << "  Forced Undefined Symbols (" << Options.UndefinedSymbols.size() << "):\n";
    for (const auto &Sym : Options.UndefinedSymbols)
        llvm::outs() << "    - " << Sym << "\n";
    llvm::outs() << "\n";

    // --- System Configuration ---
    llvm::outs() << "[System Configuration]\n";
    llvm::outs() << "  Map File:       " << (Options.MapFile.empty() ? "(None)" : Options.MapFile) << "\n\n";

    // --- Behavioral Flags ---
    auto PrintBool = [](bool Val) { return Val ? "TRUE" : "FALSE"; };
    
    llvm::outs() << "[Behavioral Flags]\n";
    llvm::outs() << "  Show Help:      " << PrintBool(Options.ShowHelp) << "\n";
    llvm::outs() << "  Force No PIC:   " << PrintBool(Options.ForceNoPIC) << "\n";
    llvm::outs() << "  Static Link:    " << PrintBool(Options.Static) << "\n";
    llvm::outs() << "  Strip Symbols:  " << PrintBool(Options.Strip) << "\n";
    llvm::outs() << "  Demangle:       " << PrintBool(Options.Demangle) << "\n";
    llvm::outs() << "  Verbose:        " << PrintBool(Options.Verbose) << "\n\n";

    // --- Optimization & Validation ---
    llvm::outs() << "[Optimization & Validation]\n";
    llvm::outs() << "  GC Sections:    " << PrintBool(Options.GCSections) << "\n";
    llvm::outs() << "  Print GC Rem:   " << PrintBool(Options.PrintGCRemoved) << "\n";
    llvm::outs() << "  Unresolved Fatal: " << PrintBool(Options.UnresolvedSymbolsFatal) << "\n";
    llvm::outs() << "==================================================\n";
    
    // Explicitly flush to guarantee everything streams out immediately
    llvm::outs().flush();
}