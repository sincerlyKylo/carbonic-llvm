#pragma once
#include "ArgumentParser.h"
#include "CEXLinkerContext.h"

void TestParser(const lld::cex::ArgumentParser& Parser);
void PrintOptions(const lld::cex::CEXLinkerContext::LinkerOptions Options);