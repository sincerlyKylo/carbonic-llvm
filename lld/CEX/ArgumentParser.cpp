#include "ArgumentParser.h"

namespace lld{
namespace cex{
    
ArgumentParser::ArgumentParser(CEXLinkerContext& Context) : context(Context) {

}
void ArgumentParser::ParseArguments(llvm::ArrayRef<const char *> PassedArguments) {
    // Skip the first argument (program name) just like the original loop (i = 1)
    for (size_t i = 1; i < PassedArguments.size(); ++i) {
        llvm::StringRef Current(PassedArguments[i]);

        if (Current.consume_front("-")) {
            // Drop any additional leading dashes (e.g., --foo becomes foo)
            Current = Current.drop_while([](char c) { return c == '-'; });
            llvm::StringRef Key = Current;

            // Check if there's a next argument and it doesn't start with a dash
            if ((i + 1) < PassedArguments.size() && PassedArguments[i + 1][0] != '-') {
                llvm::StringRef Value(PassedArguments[++i]);
                
                auto &ArgSlot = Arguments[Key];
                if (!ArgSlot.empty()) {
                    ArgSlot += "," + Value.str();
                } else {
                    ArgSlot = Value.str();
                }
            } else {
                Flags.insert(Key);
            }
        } else {
            Files.push_back(Current);
        }
    }
}

llvm::StringRef ArgumentParser::GetArgument(llvm::StringRef ArgName) const {
    auto It = Arguments.find(ArgName);
    if (It != Arguments.end()) {
        return It->second;
    }
    return llvm::StringRef();
}

bool ArgumentParser::HasArgument(llvm::StringRef ArgName) const {
    return Arguments.count(ArgName) > 0;
}

bool ArgumentParser::GetFlag(llvm::StringRef ArgName) const {
    return Flags.count(ArgName) > 0;
}

llvm::SmallVector<llvm::StringRef, 4> ArgumentParser::GetArgumentVector(llvm::StringRef ArgName) const {
    llvm::SmallVector<llvm::StringRef, 4> Result;
    
    auto It = Arguments.find(ArgName);
    if (It == Arguments.end()) {
        return Result;
    }
    
    // Use StringRef::split to completely eliminate std::stringstream
    llvm::StringRef Storage(It->second);
    std::pair<llvm::StringRef, llvm::StringRef> SplitPair;
    
    while (!Storage.empty()) {
        SplitPair = Storage.split(',');
        if (!SplitPair.first.empty()) {
            Result.push_back(SplitPair.first);
        }
        Storage = SplitPair.second;
    }
    
    return Result;
}

} // namespace cex 
} // namespace lld