#include "CEXLinkerServer.h"
#include "ArgumentParser.h"
#include "CEXLinkerContext.h"
#include "Tests.h"
namespace lld {
namespace cex {

#include "llvm/Support/raw_ostream.h"



CEXLinkerServer::CEXLinkerServer(CEXLinkerContext &ctx) : ctx(ctx) {}
void CEXLinkerServer::linkerMain(llvm::ArrayRef<const char *>& Arguments) {
    ctx.ArgParser.ParseArguments(Arguments);

    ctx.BuildOptions();
    PrintOptions(ctx.Options);
}
CEXLinkerServer::~CEXLinkerServer() {}
} // namespace cex
} // namespace lld