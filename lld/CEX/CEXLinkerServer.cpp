#include "CEXLinkerServer.h"
#include "ArgumentParser.h"
#include "CEXLinkerContext.h"
namespace lld {
namespace cex {

#include "llvm/Support/raw_ostream.h"

void TestParser(const ArgumentParser& Parser) {
    llvm::outs() << "========================================\n";
    llvm::outs() << "        ARGUMENT PARSER TEST            \n";
    llvm::outs() << "========================================\n\n";

    // 1. Dump Parsed Key-Value Arguments
    llvm::outs() << "--- Arguments Found ---\n";
    const auto& Args = Parser.GetArguments();
    if (Args.empty()) {
        llvm::outs() << "  (None)\n";
    } else {
        for (const auto& It : Args) {
            llvm::StringRef Key = It.first();
            std::string Value = It.second;
            
            llvm::outs() << "  Key:   " << Key << "\n"
                         << "  Raw:   " << Value << "\n";
                         
            // Demonstrate GetArgumentVector parsing the comma-separated values
            auto Vector = Parser.GetArgumentVector(Key);
            llvm::outs() << "  Split: [";
            for (size_t i = 0; i < Vector.size(); ++i) {
                llvm::outs() << "'" << Vector[i] << "'";
                if (i + 1 < Vector.size()) llvm::outs() << ", ";
            }
            llvm::outs() << "]\n\n";
        }
    }

    // 2. Dump Flags
    llvm::outs() << "--- Flags Found ---\n";
    const auto& Flags = Parser.GetFlags();
    if (Flags.empty()) {
        llvm::outs() << "  (None)\n";
    } else {
        for (const auto& It : Flags) {
            llvm::outs() << "  - " << It.first() << " (Active)\n";
        }
    }
    llvm::outs() << "\n";

    // 3. Dump Positional Files
    llvm::outs() << "--- Files / Positional Arguments ---\n";
    const auto& Files = Parser.GetFiles();
    if (Files.empty()) {
        llvm::outs() << "  (None)\n";
    } else {
        for (size_t i = 0; i < Files.size(); ++i) {
            llvm::outs() << "  [" << i << "]: " << Files[i] << "\n";
        }
    }
    llvm::outs() << "========================================\n";
    
    // Explicitly flush outs() to ensure everything hits the console immediately
    llvm::outs().flush();
}

CEXLinkerServer::CEXLinkerServer(CEXLinkerContext &ctx) : ctx(ctx) {}
void CEXLinkerServer::linkerMain(llvm::ArrayRef<const char *>& Arguments) {
    ctx.ArgParser.ParseArguments(Arguments);
    TestParser(ctx.ArgParser);

    ctx.BuildOptions();
}
CEXLinkerServer::~CEXLinkerServer() {}
} // namespace cex
} // namespace lld