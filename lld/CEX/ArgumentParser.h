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
    ArgumentParser(llvm::ArrayRef<const char *> args, CEXLinkerContext& context);
    
    llvm::StringRef GetArgument(llvm::StringRef argName) const;
    llvm::SmallVector<llvm::StringRef, 8> GetArgumentVector(llvm::StringRef argName) const;
    bool HasArgument(llvm::StringRef argName) const;

    bool GetFlag(llvm::StringRef argName) const;

    void BuildArguments();
    
private:
    // StringMap is an incredibly fast HashTable bucketed specifically for string keys
    llvm::StringMap<llvm::StringRef> Arguments;
    llvm::StringSet<> Flags; // Shorthand for StringMap<NoneType>
    llvm::SmallVector<llvm::StringRef, 16> Files; // Allocates space for 16 files right on the stack!
    CEXLinkerContext& context;
};

} // namespace cex
} // namespace lld