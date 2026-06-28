#include "lld/Common/ErrorHandler.h"
#include "llvm/Support/raw_ostream.h"
#include "CEXLinkerContext.h"
#include "lld/Common/Args.h"

namespace lld {
namespace cex {
uint64_t errCount(CEXLinkerContext &ctx) { return ctx.e.errorCount; }

bool link(llvm::ArrayRef<const char *> args, llvm::raw_ostream &stdoutOS,    
            llvm::raw_ostream &stderrOS, bool exitEarly, bool disableOutput) {
    
    auto* context = new CEXLinkerContext();
    context->e.initialize(stdoutOS, stderrOS, exitEarly, disableOutput);
    context->e.logName = args::getFilenameWithoutExe(args[0]);
    context->e.errorLimitExceededMsg = "too many errors emitted, stopping now"
                                 " (use /errorlimit:0 to see all errors)";

    context->Server.linkerMain(args);
    return errCount(*context) == 0; 
}
} // namespace cex
} // namespace lldz