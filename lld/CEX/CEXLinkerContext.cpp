#include "CEXLinkerContext.h"
#include "ArgumentParser.h"
#include "CEXLinkerServer.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "lld/Common/Args.h"
namespace lld {
namespace cex {

static inline CEXLinkerContext::FileType GetFileTypeFromString(llvm::StringRef String) {
    return llvm::StringSwitch<CEXLinkerContext::FileType>(String)
        .Case("kernel", CEXLinkerContext::Kernel)
        .Case("executable", CEXLinkerContext::Executable)
        .Case("dlib", CEXLinkerContext::DynamicLibrary)
        .Case("slib", CEXLinkerContext::StaticLibrary)
        .Case("kerneldriver", CEXLinkerContext::KernelDriver)
        .Case("userdriver",  CEXLinkerContext::UserDriver)
        .Default(CEXLinkerContext::UnknownType);
}

CEXLinkerContext::CEXLinkerContext() : Server(*this), ArgParser(*this){

}
int CEXLinkerContext::BuildOptions() {
    Options.OutputFile = ArgParser.GetArgument("o");
    Options.EntrySymbol = ArgParser.GetArgument("entry", "e");
    

    if (ArgParser.HasArgument("image-base", "b")) {
        if (!llvm::to_integer(ArgParser.GetArgument("image-base", "b"), Options.ImageBase, 16)) {
            e.fatal("Failed to parse argument for image base (must be valid hex).\n");
            return -1;
        }
    } // If not passed, it quietly retains its struct default value (0x400000)

    if (ArgParser.HasArgument("page-size")) {
        if (!llvm::to_integer(ArgParser.GetArgument("page-size"), Options.PageSize, 16)) {
            e.fatal("Failed to parse argument for page size (must be valid hex).\n");
            return -1;
        }
    }

    if (ArgParser.HasArgument("stack-size")) {
        if (!llvm::to_integer(ArgParser.GetArgument("stack-size"), Options.StackSize, 16)) {
            e.fatal("Failed to parse argument for stack size (must be valid hex).\n");
            return -1;
        }
    }

    Options.Type = GetFileTypeFromString(ArgParser.GetArgument("platform-target"));
    if(Options.Type == UnknownType) {
        e.fatal("Unknown platform target passed, supported targets are: kernel, executable, dlib, slib, kerneldriver, userdriver");
        return -1;
    }

    Options.ObjectFiles = ArgParser.GetFiles();

    //llvm::StringRef Sysroot = ArgParser.GetArgument("sysroot");
    Options.LibrariesToLink = ArgParser.GetArgumentVector("l");

    Options.MapFile = ArgParser.GetArgument("map");

    Options.ShowHelp = ArgParser.GetFlag("h", "help");
    Options.ForceNoPIC = ArgParser.GetFlag("force_no_pic");
    Options.Static = ArgParser.GetFlag("s", "static");
    Options.Strip = ArgParser.GetFlag("strip");
    Options.Demangle = ArgParser.GetFlag("d", "demangle");
    Options.Verbose = ArgParser.GetFlag("v", "verbose");

    Options.GCSections = ArgParser.GetFlag("gc-sections");
    Options.PrintGCRemoved = ArgParser.GetFlag("print-gc-sections");
    Options.UnresolvedSymbolsFatal = ArgParser.GetFlag("fatal-unresolved-symbols");
}
} // namespace cex 
}