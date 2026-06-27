#include "lld/Common/ErrorHandler.h"
#include "llvm/Support/raw_ostream.h"

namespace lld {
namespace cex {
bool link(llvm::ArrayRef<const char *> args, llvm::raw_ostream &stdoutOS,    
            llvm::raw_ostream &stderrOS, bool exitEarly, bool disableOutput) {
    
    llvm::outs() << "Hello from the custom CEX linker driver!\n";

    return true; 
}
} // namespace cex
} // namespace lld