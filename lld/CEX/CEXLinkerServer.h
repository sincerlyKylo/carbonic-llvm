#pragma once
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/SmallVector.h"
namespace lld {
namespace cex {

class CEXLinkerContext;

class CEXLinkerServer {


public:
    CEXLinkerServer(CEXLinkerContext &ctx); 
    ~CEXLinkerServer();
    void linkerMain(llvm::ArrayRef<const char *>& Arguments);
    
private:
    CEXLinkerContext& ctx;
};

} // namespace cex 
} // namespace lld