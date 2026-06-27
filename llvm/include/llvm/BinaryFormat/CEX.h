#pragma once 
#include "llvm/ADT/StringRef.h"
#include "llvm/BinaryFormat/Magic.h"

#define CEX_PACKED __attribute__((packed))

namespace llvm {
namespace CEX {
enum class CEXFileType : uint16_t {
    Kernel            = 0xC1EE,
    KernelCollection  = 0xC2EE,
    Executable        = 0xC3EE,
    DLib              = 0xC4EE,
    SysLib            = 0xC5EE,
    Object            = 0xC6EE,
    SLib              = 0xC7EE,
    SharedCache       = 0xC8EE
};

struct CEXHeader {
    char Magic[4];
    uint8_t Version;
    CEXFileType Type;
    uint64_t ProgramTableOffset;
    uint64_t ProgramTableEntrySize;
    uint64_t ProgramTableEntryCount;
    uint16_t DefaultProgramTableEntry;
    uint64_t Flags; //Bit field of CEXHeaderFlags
} CEX_PACKED;

llvm::file_magic GetCEXFileMagic(llvm::StringRef File); 
}
}