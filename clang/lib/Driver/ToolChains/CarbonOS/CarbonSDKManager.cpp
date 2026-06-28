#include "CarbonSDKManager.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/JSON.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Process.h"
#include "llvm/Support/Program.h"
#include "llvm/Support/MemoryBuffer.h"

namespace clang {
namespace driver {
namespace tools {
namespace carbon {

static inline std::string
GetResponseCodeString(CarbonSDKManager::ResponseCode RCode) {
    switch (RCode) {
        case CarbonSDKManager::CSDK_BIN_PATH:
            return "CSDK_BIN_PATH";
        case CarbonSDKManager::CSDK_LIB_PATH:
            return "CSDK_LIB_PATH";
        case CarbonSDKManager::CSDK_SYSROOT_PATH:
            return "CSDK_SYSROOT_PATH";
        case CarbonSDKManager::CSDK_COMPONENT_MISSING:
            return "CSDK_COMPONENT_MISSING";
        case CarbonSDKManager::CSDK_TOOLCHAIN_NOT_FOUND:
            return "CSDK_TOOLCHAIN_NOT_FOUND";
        case CarbonSDKManager::CSDK_SDK_NOT_FOUND:
            return "CSDK_SDK_NOT_FOUND";
        case CarbonSDKManager::ErrorStart:
            return "";
    }
}

CarbonSDKManager::CarbonSDKManager(const clang::driver::Driver &Driver)
    : TheDriver(Driver) {
  SmallString<128> ToolPath(TheDriver.Dir);
  llvm::sys::path::append(ToolPath, "carbon-sdk");
  SDKExecutablePath = std::string(ToolPath.str());

  // If not found right next to clang, check the host environment PATH
  if (!llvm::sys::fs::exists(SDKExecutablePath)) {
    if (auto Lookup = llvm::sys::Process::FindInEnvPath("PATH", "carbon-sdk")) {
      SDKExecutablePath = *Lookup;
    } else {
      // Hard crash immediately if the manager is missing completely
      unsigned DiagID = TheDriver.getDiags().getCustomDiagID(
          DiagnosticsEngine::Fatal,
          "CarbonOS Toolchain Error: 'carbon-sdk' manager utility could not be "
          "found in your environment.");
      TheDriver.Diag(DiagID);
      exit(1);
    }
  }
}

CarbonSDKManager::Response
CarbonSDKManager::querySDK(ResponseCode Requested) const {
  SmallString<128> OutputFile;
  llvm::sys::fs::createTemporaryFile("carbon-sdk-out", "json", OutputFile);

  std::optional<StringRef> Redirects[] = {std::nullopt, OutputFile.str(),
                                          std::nullopt};
  
  std::string RequestString = GetResponseCodeString(Requested);
  // Keep RequestString alive throughout this block!
  StringRef Flag(RequestString); 
  
  std::vector<StringRef> Args = {SDKExecutablePath, "--request", Flag};

  // 2. Execute the verified process
  int RetValue =
      llvm::sys::ExecuteAndWait(SDKExecutablePath, Args, std::nullopt, Redirects);

  if (RetValue != 0) {
    llvm::sys::fs::remove(OutputFile.str());
    unsigned DiagID = TheDriver.getDiags().getCustomDiagID(
        DiagnosticsEngine::Fatal, "CarbonOS SDK manager crashed or exited with "
                                  "error code %0 while executing request flag '%1'.");
    TheDriver.Diag(DiagID) << RetValue << Flag;
    exit(1);
  }

  // 3. Map the temporary file into a memory buffer
  auto BufOrErr = llvm::MemoryBuffer::getFile(OutputFile.str());

  if (!BufOrErr) {
    unsigned DiagID = TheDriver.getDiags().getCustomDiagID(
        DiagnosticsEngine::Fatal, "CarbonOS SDK manager communication failed reading IPC stream.");
    TheDriver.Diag(DiagID);
    exit(1);
  }

  // Get a StringRef view of the buffer contents
  llvm::StringRef RawJson = BufOrErr->get()->getBuffer().trim();

  // Delete the temporary file from disk now that it's safe in memory
  llvm::sys::fs::remove(OutputFile.str());

  // 4. Safely parse JSON structure
  llvm::Expected<llvm::json::Value> ExpectedJson = llvm::json::parse(RawJson);
  if (!ExpectedJson) {
    unsigned DiagID = TheDriver.getDiags().getCustomDiagID(
        DiagnosticsEngine::Fatal, "Malformed JSON payload returned by 'carbon-sdk'.");
    TheDriver.Diag(DiagID);
    exit(1);
  }

  const llvm::json::Object *Object = ExpectedJson->getAsObject();
  if (!Object) {
    unsigned DiagID = TheDriver.getDiags().getCustomDiagID(
        DiagnosticsEngine::Fatal, "Root level element from 'carbon-sdk' payload must be a JSON object.");
    TheDriver.Diag(DiagID);
    exit(1);
  }

  // 5. Evaluate response contents according to custom schema
  llvm::StringRef ResponseID = Object->getString("ResponseID").value_or("");
  llvm::StringRef ResponseString = Object->getString("ResponseString").value_or("");

  Response R;
  R.Response = std::string(ResponseString);

  // Map ResponseID string back to our enum tracking
  if (ResponseID == "CSDK_BIN_PATH") R.Code = CSDK_BIN_PATH;
  else if (ResponseID == "CSDK_LIB_PATH") R.Code = CSDK_LIB_PATH;
  else if (ResponseID == "CSDK_SYSROOT_PATH") R.Code = CSDK_SYSROOT_PATH;
  else if (ResponseID == "CSDK_COMPONENT_MISSING") R.Code = CSDK_COMPONENT_MISSING;
  else if (ResponseID == "CSDK_TOOLCHAIN_NOT_FOUND") R.Code = CSDK_TOOLCHAIN_NOT_FOUND;
  else if (ResponseID == "CSDK_SDK_NOT_FOUND") R.Code = CSDK_SDK_NOT_FOUND;
  else R.Code = ErrorStart;

  if (R.Code > ErrorStart) {
    unsigned DiagID = TheDriver.getDiags().getCustomDiagID(
        DiagnosticsEngine::Fatal, "CarbonOS SDK Configuration Failure [%0]: %1");
    TheDriver.Diag(DiagID) << ResponseID << ResponseString;
    exit(1);
  }

  return R;
}

  std::string CarbonSDKManager::getSysRoot() const {
    return querySDK(CarbonSDKManager::CSDK_SYSROOT_PATH).Response;
  }

  std::string CarbonSDKManager::getBinPath() const {
    return querySDK(CarbonSDKManager::CSDK_BIN_PATH).Response;
  }

  std::string CarbonSDKManager::getLibPath() const {
    return querySDK(CarbonSDKManager::CSDK_LIB_PATH).Response;
  }

} // namespace carbon
} // namespace tools
} // namespace driver
} // namespace clang