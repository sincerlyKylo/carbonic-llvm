#pragma once 
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringSet.h"


namespace lld {
namespace cex {

class CEXLinkerContext;

class ArgumentParser {
public:
    ArgumentParser(CEXLinkerContext& Context);
    
    // Parses arguments from an ArrayRef (e.g., made from argv)
    void ParseArguments(llvm::ArrayRef<const char *> PassedArguments);
    
    llvm::StringRef GetArgument(llvm::StringRef ArgName) const;
    llvm::SmallVector<llvm::StringRef, 4> GetArgumentVector(llvm::StringRef ArgName) const;
    bool HasArgument(llvm::StringRef ArgName) const;
    bool GetFlag(llvm::StringRef ArgName) const;
    
    const llvm::SmallVector<llvm::StringRef, 8>& GetFiles() const { return Files; }
    const llvm::StringMap<std::string>& GetArguments() const { return Arguments; }
    const llvm::StringSet<>& GetFlags() const { return Flags; }
    
private:
    llvm::StringMap<std::string> Arguments;
    llvm::StringSet<> Flags;
    llvm::SmallVector<llvm::StringRef, 8> Files;
    CEXLinkerContext& context;
};

} // namespace cex
} // namespace lld