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
    
    void ParseArguments(llvm::ArrayRef<const char *> PassedArguments);
    
    // --- 1. Single Arguments ---
    llvm::StringRef GetArgument(llvm::StringRef ArgName) const;
    
    template<typename... Aliases> 
    llvm::StringRef GetArgument(llvm::StringRef ArgName, Aliases... Alias) const {
        llvm::StringRef Current = GetArgument(ArgName);
        if (!Current.empty()) 
            return Current;
        return GetArgument(Alias...);
    }

    // --- 2. Multiple Arguments (Vectors) ---
    llvm::SmallVector<llvm::StringRef, 4> GetArgumentVector(llvm::StringRef ArgName) const;
    
    template<typename... Aliases> 
    llvm::SmallVector<llvm::StringRef, 4> GetArgumentVector(llvm::StringRef ArgName, Aliases... Alias) const {
        auto MasterVector = GetArgumentVector(ArgName);
        auto AliasVector = GetArgumentVector(Alias...);
        
        // Append all found elements from the aliases into the master list
        MasterVector.append(AliasVector.begin(), AliasVector.end());
        return MasterVector;
    }

    // --- 3. Positional / Flag checks ---
    bool HasArgument(llvm::StringRef ArgName) const;
    
    template<typename... Aliases> 
    bool HasArgument(llvm::StringRef ArgName, Aliases... Alias) const {
        return HasArgument(ArgName) || HasArgument(Alias...);
    }

    bool GetFlag(llvm::StringRef ArgName) const;
    
    template<typename... Aliases> 
    bool GetFlag(llvm::StringRef ArgName, Aliases... Alias) const {
        return GetFlag(ArgName) || GetFlag(Alias...);
    }
    
    const llvm::SmallVector<llvm::StringRef, 16>& GetFiles() const { return Files; }
    const llvm::StringMap<std::string>& GetArguments() const { return Arguments; }
    const llvm::StringSet<>& GetFlags() const { return Flags; }
private:
    llvm::StringMap<std::string> Arguments;
    llvm::StringSet<> Flags;
    llvm::SmallVector<llvm::StringRef, 16> Files;
    CEXLinkerContext& context;
};

} // namespace cex
} // namespace lld