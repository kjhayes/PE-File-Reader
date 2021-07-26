#ifndef IMAGE_FILE_HPP
#define IMAGE_FILE_HPP

//Sourced From https://docs.microsoft.com/en-us/windows/win32/debug/pe-format

class IMAGE_FILE {
public:

enum MACHINE {
    UNKNOWN = 0x0,
    AM33 = 0x1d3,
    AMD64 = 0x8664,
    ARM = 0x1c0,
    ARM64 = 0xaa64,
    ARMNT = 0x1c4,
    EBC = 0xebc,
    I386 = 0x14c,
    IA64 = 0x200,
    M32R = 0x9041,
    MIPS16 = 0x266,
    MIPSFPU = 0x366,
    MIPSFPU16 = 0x466,
    POWERPC = 0x1f0,
    POWERPCFP = 0x1f1,
    R4000 = 0x166,
    RISCV32 = 0x5032,
    RISCV64 = 0x5064,
    RISCV128 = 0x5128,
    SH3 = 0x1a2,
    SH3DSP = 0x1a3,
    SH4 = 0x1a6,
    SH5 = 0x1a8,
    THUMB = 0x1c2,
    WCEMIPSV2 = 0x169
};

static const char* GetMachineString(const MACHINE& m){
    switch (m)
    {
    case(MACHINE::UNKNOWN):{return "UNKNOWN";}
    case(MACHINE::AM33):{return "AM33";}
    case(MACHINE::AMD64):{return "AMD64";}
    case(MACHINE::ARM):{return "ARM";}
    case(MACHINE::ARM64):{return "ARM64";}
    case(MACHINE::ARMNT):{return "ARMNT";}
    case(MACHINE::EBC):{return "EBC";}
    case(MACHINE::I386):{return "I386";}
    case(MACHINE::IA64):{return "IA64";}
    case(MACHINE::M32R):{return "M32R";}
    case(MACHINE::MIPS16):{return "MIPS16";}
    case(MACHINE::MIPSFPU):{return "MIPSFPU";}
    case(MACHINE::MIPSFPU16):{return "MIPSFPU16";}
    case(MACHINE::POWERPC):{return "POWERPC";}
    case(MACHINE::POWERPCFP):{return "POWERPCFP";}
    case(MACHINE::R4000):{return "R4000";}
    case(MACHINE::RISCV32):{return "RISCV32";}
    case(MACHINE::RISCV64):{return "RISCV64";}
    case(MACHINE::RISCV128):{return "RISCV128";}
    case(MACHINE::SH3):{return "SH3";}
    case(MACHINE::SH3DSP):{return "SH3DSP";}
    case(MACHINE::SH4):{return "SH4";}
    case(MACHINE::SH5):{return "SH5";}
    case(MACHINE::THUMB):{return "THUMB";}
    case(MACHINE::WCEMIPSV2):{return "WCEMIPSV2";}
    default:{return "N/A";}
    }
}

};

#endif