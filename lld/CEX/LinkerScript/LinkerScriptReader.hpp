#pragma once
#include "CEXLinkerContext.h"
#include "LinkerScript/Lexer.hpp"

namespace lld{
namespace cex {

class LinkerScriptReader {
    LinkerScriptReader(CEXLinkerContext& Context);
    
    void ReadLinkerScript();
private:
    CEXLinkerContext& Context;
    Lexer lexer;
};

}
}

