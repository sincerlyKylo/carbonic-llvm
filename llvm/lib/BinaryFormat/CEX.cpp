#include "llvm/BinaryFormat/CEX.h"
#include "llvm/BinaryFormat/Magic.h"

llvm::file_magic llvm::CEX::GetCEXFileMagic(llvm::StringRef File) {
    if(File.size() < sizeof(CEXHeader)) return llvm::file_magic::cex_object;
    CEXHeader* Header = reinterpret_cast<CEXHeader*>(const_cast<char*>(File.data()));
    switch(Header->Type) {
        case llvm::CEX::CEXFileType::DLib:
            return llvm::file_magic::cex_dynamiclibrary;
        case llvm::CEX::CEXFileType::SLib:
            return llvm::file_magic::cex_staticlibrary;
        case llvm::CEX::CEXFileType::SysLib:
            return llvm::file_magic::cex_system_driver;
        case llvm::CEX::CEXFileType::Executable:
            return llvm::file_magic::cex_executable;
        case llvm::CEX::CEXFileType::KernelCollection:
            return llvm::file_magic::cex_kernelcollection;
        case llvm::CEX::CEXFileType::Object:
            return llvm::file_magic::cex_object;
        case llvm::CEX::CEXFileType::Kernel:
            return llvm::file_magic::cex_kernel;
        case llvm::CEX::CEXFileType::SharedCache:
            return llvm::file_magic::cex_shared_cache;
    }
}