#include "CEXLinkerContext.h"
#include "ArgumentParser.h"
#include "CEXLinkerServer.h"
namespace lld {
namespace cex {

CEXLinkerContext::CEXLinkerContext() : Server(*this), ArgParser(*this){

}

}
}