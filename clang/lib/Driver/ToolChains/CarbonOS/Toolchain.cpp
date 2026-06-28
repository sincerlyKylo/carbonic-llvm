#include "Toolchain.h"
#include "ToolChains/CarbonOS/CarbonSDKManager.h"

namespace clang {
namespace driver {

namespace toolchains {
CarbonOS::CarbonOS(const Driver &D, const llvm::Triple &Triple, const llvm::opt::ArgList &Args) : ToolChain(D, Triple, Args) {
  getFilePaths().clear();
  getLibraryPaths().clear();
  getProgramPaths().clear();

  SDKManager = std::make_unique<tools::carbon::CarbonSDKManager>(D);

  getProgramPaths().push_back(SDKManager->getBinPath());

  bool IsFreestanding = Args.hasArg(options::OPT_ffreestanding);
  bool NoStdLib = Args.hasArg(options::OPT_nostdlib) || Args.hasArg(options::OPT_nodefaultlibs);

  if (!IsFreestanding && !NoStdLib) {
    std::string LibDir = SDKManager->getLibPath();
    getFilePaths().push_back(LibDir);
    getLibraryPaths().push_back(LibDir);
  }
}

bool CarbonOS::IsIntegratedAssemblerDefault() const {

}

bool CarbonOS::useIntegratedAs() const {

}

bool CarbonOS::isPICDefault() const { return true; } // CarbonOS style preference
bool CarbonOS::isPIEDefault(const llvm::opt::ArgList &Args) const { return false; }

std::string CarbonOS::getThreadModel() const {
return "single";
}

bool CarbonOS::isBareMetal() const {
  return false;
}

const char * CarbonOS::getDefaultLinker() const {
  
}

void CarbonOS::AddClangSystemIncludeArgs(const llvm::opt::ArgList &DriverArgs, llvm::opt::ArgStringList &CC1Args) const {

}
void CarbonOS::AddClangCXXStdlibIncludeArgs(const llvm::opt::ArgList &DriverArgs, llvm::opt::ArgStringList &CC1Args) const {

}
std::string CarbonOS::computeSysRoot() const {

}


// We explicitly build our own custom tools, bypassing GNU defaults
Tool* CarbonOS::buildAssembler() const {

}

Tool* CarbonOS::buildLinker() const {

}


} // namespace toolchains
} // namespace driver
} // namespace clang