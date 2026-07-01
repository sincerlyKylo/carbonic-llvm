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
    Options.Demangle = ArgParser.GetFlag("d") || ArgParser.GetFlag("demangle");
    Options.EntrySymbol = ArgParser.GetArgument("entry");
    Options.ShowHelp = ArgParser.GetFlag("help") || ArgParser.GetFlag("h");
    Options.OutputFile = ArgParser.GetArgument("o");
    Options.ObjectFiles = ArgParser.GetFiles();
    
    Options.Type = GetFileTypeFromString(ArgParser.GetArgument("platform-target"));
    if(Options.Type == UnknownType) {
        e.fatal("Unknown platform target passed, supported targets are: kernel, executable, dlib, slib, kerneldriver, userdriver");
        return -1;
    }
}
} // namespace cex 
}