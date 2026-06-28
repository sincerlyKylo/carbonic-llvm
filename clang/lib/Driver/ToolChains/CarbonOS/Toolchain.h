#pragma once

#include "ToolChains/CarbonOS/CarbonSDKManager.h"
#include "clang/Driver/ToolChain.h"
#include "clang/Driver/Tool.h"

namespace clang {
namespace driver {
namespace toolchains {

class LLVM_LIBRARY_VISIBILITY CarbonOS : public ToolChain {
public:
  CarbonOS(const Driver &D, const llvm::Triple &Triple, 
           const llvm::opt::ArgList &Args);


  bool IsIntegratedAssemblerDefault() const override;
  bool useIntegratedAs() const override;

  bool isPICDefault() const override;
  bool isPIEDefault(const llvm::opt::ArgList &Args) const override;

  std::string getThreadModel() const override;

  bool isBareMetal() const override;

  const char * getDefaultLinker() const override;

  void AddClangSystemIncludeArgs(const llvm::opt::ArgList &DriverArgs, llvm::opt::ArgStringList &CC1Args) const override;
  void AddClangCXXStdlibIncludeArgs(const llvm::opt::ArgList &DriverArgs, llvm::opt::ArgStringList &CC1Args) const override;
  std::string computeSysRoot() const override;

protected:
  // We explicitly build our own custom tools, bypassing GNU defaults
  Tool *buildAssembler() const override;
  Tool *buildLinker() const override;
private:
   std::unique_ptr<tools::carbon::CarbonSDKManager> SDKManager;
};

} // namespace toolchains
} // namespace driver
} // namespace clang