#pragma once

#include "clang/Driver/Driver.h"
#include "llvm/ADT/StringRef.h"

namespace clang {
namespace driver {
namespace tools {
namespace carbon {
class CarbonSDKManager {
public:
    enum ResponseCode {
        CSDK_SYSROOT_PATH,
        CSDK_BIN_PATH,
        CSDK_LIB_PATH,

        ErrorStart,
        CSDK_SDK_NOT_FOUND,
        CSDK_TOOLCHAIN_NOT_FOUND,
        CSDK_COMPONENT_MISSING,
    };
    struct Response {
        ResponseCode Code;
        std::string Response;
    };

    CarbonSDKManager(const clang::driver::Driver& Driver);
    std::string getSysRoot() const;
    std::string getBinPath() const;
    std::string getLibPath() const;

private:
    Response querySDK(ResponseCode Requested) const;

    std::string SDKExecutablePath; 
    const clang::driver::Driver& TheDriver;
};
} // namespace carbon
} // namespace tools
} // namespace driver
} // namespace clang

