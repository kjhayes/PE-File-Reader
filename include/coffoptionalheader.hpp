#ifndef COFFOPTIONALHEADER_HPP
#define COFFOPTIONALHEADER_HPP

#include "streamreader.hpp"
#include "istreamrw.hpp"
#include "ijsonwritable.hpp"

#include<iostream>
#include<vector>

enum PEFormat {
    ROM = 0x107,
    PE32 = 0x10b,
    PE32Plus = 0x20b
};

const char* GetPEFormatString(const PEFormat& pe_format){
    switch(pe_format){
        case(PEFormat::ROM):{return "ROM Image";}
        case(PEFormat::PE32):{return "PE32 Executable";}
        case(PEFormat::PE32Plus):{return "PE32+ Executable";}
        default:{return "N/A";}
    }
}

class COFFOptionalHeader : public IStreamRW, public IJSONWritable {
public:
    bool is_pure_standard_coff = true;

//COFF Standard
    uint16_t pe_format;
    uint8_t linker_version_major;
    uint8_t linker_version_minor;
    uint32_t size_of_code;
    uint32_t size_of_initialized_data;
    uint32_t size_of_uninitialized_data;
    uint32_t rel_address_of_entry_point;
    uint32_t base_of_code;
    uint32_t base_of_data;

    virtual void ReadFromStream(std::istream& istr, std::ostream& out = std::cout, std::ostream& err = std::cerr) override {
        pe_format = (PEFormat)GetNextLEU16(istr, err);
        linker_version_major = GetNextU8(istr, err);
        linker_version_minor = GetNextU8(istr, err);
        size_of_code = GetNextLEU32(istr, err);
        size_of_initialized_data = GetNextLEU32(istr, err);
        size_of_uninitialized_data = GetNextLEU32(istr, err);
        rel_address_of_entry_point = GetNextLEU32(istr, err);
        base_of_code = GetNextLEU32(istr, err);
        if(pe_format == PEFormat::PE32){base_of_data = GetNextLEU32(istr, err);}
    }
    virtual void WriteJSON(std::ostream& ostr, const int& flags, const int& depth) const override {
        IJSONWritable::WriteJSONString(ostr, "PE Format", GetPEFormatString((PEFormat)pe_format), flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned(ostr, "Linker Version Major", linker_version_major, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned(ostr, "Linker Version Minor", linker_version_minor, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned(ostr, "Size Of Code", size_of_code, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned(ostr, "Size Of Initialized Data", size_of_initialized_data, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned(ostr, "Size Of Uninitialized Data", size_of_uninitialized_data, flags, depth);
        IJSONWritable::WriteJSONHexString(ostr, "Relative Address Of Entry Point", rel_address_of_entry_point, flags, depth);
        IJSONWritable::WriteJSONHexString(ostr, "Base Of Code", base_of_code, flags, depth);
        if(pe_format == PEFormat::PE32){
            IJSONWritable::WriteJSONHexString(ostr, "Base Of Data", base_of_data, flags, depth, is_pure_standard_coff);
        }
        
    }
};

enum WindowsSubsystem {
    UNKNOWN = 0x0,
    NATIVE = 0x1,
    WINDOWS_GUI = 0x2,
    WINDOWS_CUI = 0x3,
    OS2_CUI = 0x5,
    POSIX_CUI = 0x7,
    NATIVE_WINDOWS = 0x8,
    WINDOWS_CE_GUI = 0x9,
    EFI_APPLICATION = 0xa,
    EFI_BOOT_SERVICE_DRIVER = 0xb,
    EFI_RUNTIME_DRIVER = 0xc,
    EFI_ROM = 0xd,
    XBOX = 0xe,
    WINDOWS_BOOT_APPLICATION = 0xf
};

const char* GetWindowsSubsystemString(const WindowsSubsystem& ss) {
    switch(ss){
        case(WindowsSubsystem::UNKNOWN):{return "UNKNOWN";}
        case(WindowsSubsystem::NATIVE):{return "NATIVE";}
        case(WindowsSubsystem::WINDOWS_GUI):{return "WINDOWS GUI";}
        case(WindowsSubsystem::WINDOWS_CUI):{return "WINDOWS CUI";}
        case(WindowsSubsystem::OS2_CUI):{return "OS2 CUI";}
        case(WindowsSubsystem::POSIX_CUI):{return "POSIX CUI";}
        case(WindowsSubsystem::NATIVE_WINDOWS):{return "NATIVE WINDOWS";}
        case(WindowsSubsystem::WINDOWS_CE_GUI):{return "WINDOWS CE GUI";}
        case(WindowsSubsystem::EFI_APPLICATION):{return "EFI APPLICATION";}
        case(WindowsSubsystem::EFI_BOOT_SERVICE_DRIVER):{return "EFI BOOT SERVICE DRIVER";}
        case(WindowsSubsystem::EFI_RUNTIME_DRIVER):{return "EFI RUNTIME DRIVER";}
        case(WindowsSubsystem::EFI_ROM):{return "EFI ROM";}
        case(WindowsSubsystem::XBOX):{return "XBOX";}
        case(WindowsSubsystem::WINDOWS_BOOT_APPLICATION):{return "WINDOWS BOOT APPLICATION";}
        default:{return "N/A";}
    }
}

enum DLL_CHARACTERISTICS {
    HIGH_ENTROPY_VA = 0x0020,
    DYNAMIC_BASE = 0x0040,
    FORCE_INTEGRITY = 0x0080,
    NX_COMPAT = 0x0100,
    NO_ISOLATION = 0x0200,
    NO_SEH = 0x0400,
    NO_BIND = 0x0800,
    APPCONTAINER = 0x1000,
    WDM_DRIVER = 0x2000,
    GUARD_CF = 0x4000,
    TERMINAL_SERVER_AWARE = 0x8000
};

struct DataDirectory : public IJSONWritable {
public:
    DataDirectory(const uint32_t& va, const uint32_t& size):virtual_address(va),size(size){}

    uint32_t virtual_address;
    uint32_t size;
    void WriteJSON(std::ostream& ostr, const int& flags, const int& depth) const override {
        IJSONWritable::WriteJSONHexString(ostr, "Virtual Address", virtual_address, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned(ostr, "Size", size, flags, depth);
    }

};
struct RVATable : public IStreamRW, public IJSONWritable {
public:
    std::vector<DataDirectory> data_directories;

    uint32_t size() const {return data_directories.size();}
    bool isEmpty() const {
        return (data_directories.size() == 0);
    }

    void ReadFromStream(std::istream& istr, std::ostream& out = std::cout, std::ostream& err = std::cerr){
        uint32_t number_of_rva_and_sizes = GetNextLEU32(istr, err);
        for(int i = 0; i<number_of_rva_and_sizes; i++){
            uint32_t va = GetNextLEU32(istr, err);
            uint32_t size = GetNextLEU32(istr, err);
            data_directories.emplace_back(va, size);
        }
    }

    void WriteJSON(std::ostream& ostr, const int& flags, const int& depth) const override {
        for(int i = 0; i < data_directories.size(); i++){
            IJSONWritable::WriteJSONObjectAsElement(ostr, rva_names[i], data_directories[i], flags, depth, (i == data_directories.size()-1));
        }
    }

    static const char* rva_names[16];
};
const char* RVATable::rva_names[16] = {
    "Export Table",
    "Import Table",
    "Resource Table",
    "Exception Table",
    "Certificate Table",
    "Base Relocation Table",
    "Debug",
    "Architecture", //Reserved: Must Be Zero.
    "Global Ptr",
    "TLS Table",
    "Load Config Table",
    "Bound Import",
    "IAT", //Import Address Table
    "Delay Import Descriptor",
    "CLR Runtime Header",
    "Reserved" //Reserved: Must Be Zero.
};

class COFFOptionalHeader_Windows : public COFFOptionalHeader {
public:
    COFFOptionalHeader_Windows(){
        is_pure_standard_coff = false;
    }

    union {
        uint32_t dw_image_base;
        uint64_t l_image_base;
    };
    uint32_t section_alignment;
    uint32_t file_alignment;
    uint16_t major_os_version;
    uint16_t minor_os_version;
    uint16_t major_image_version;
    uint16_t minor_image_version;
    uint16_t major_subsystem_version;
    uint16_t minor_subsystem_version;
    uint32_t win32_version_value; //Should Be Zero Apparently?
    uint32_t size_of_image; //rounded to section alignment multiple
    uint32_t size_of_headers; //rounded to file alignment multiple
    uint32_t check_sum;
    uint16_t subsystem;
    uint16_t dll_characteristics;
    union {
        uint32_t dw_size_of_stack_reserve;
        uint64_t l_size_of_stack_reserve;
    };
    union {
        uint32_t dw_size_of_stack_commit;
        uint64_t l_size_of_stack_commit;
    };
    union {
        uint32_t dw_size_of_heap_reserve;
        uint64_t l_size_of_heap_reserve;
    };
    union {
        uint32_t dw_size_of_heap_commit;
        uint64_t l_size_of_heap_commit;
    };
    uint32_t loader_flags; //Also Must Be Zero Apparently?
    RVATable rva_table;    

    void ReadFromStream(std::istream& istr, std::ostream& out = std::cout, std::ostream& err = std::cerr) override {
        COFFOptionalHeader::ReadFromStream(istr, out, err);
        if(pe_format == PEFormat::PE32Plus){l_image_base = GetNextLEU64(istr, err);}
        else{dw_image_base = GetNextLEU32(istr, err);}
        section_alignment = GetNextLEU32(istr, err);
        file_alignment = GetNextLEU32(istr, err);
        major_os_version = GetNextLEU16(istr, err);
        minor_os_version = GetNextLEU16(istr, err);
        major_image_version = GetNextLEU16(istr, err);
        minor_image_version = GetNextLEU16(istr, err);
        major_subsystem_version = GetNextLEU16(istr, err);
        minor_subsystem_version = GetNextLEU16(istr, err);
        win32_version_value = GetNextLEU32(istr, err);
        size_of_image = GetNextLEU32(istr, err);
        size_of_headers = GetNextLEU32(istr, err);
        check_sum = GetNextLEU32(istr, err);
        subsystem = GetNextLEU16(istr, err);
        dll_characteristics= GetNextLEU16(istr, err);
        if(pe_format == PEFormat::PE32Plus){
            l_size_of_stack_reserve = GetNextLEU64(istr, err);
            l_size_of_stack_commit = GetNextLEU64(istr, err);
            l_size_of_heap_reserve = GetNextLEU64(istr, err);
            l_size_of_heap_commit = GetNextLEU64(istr, err);
        }
        else{
            dw_size_of_stack_reserve = GetNextLEU32(istr, err);
            dw_size_of_stack_commit = GetNextLEU32(istr, err);
            dw_size_of_heap_reserve = GetNextLEU32(istr, err);
            dw_size_of_heap_commit = GetNextLEU32(istr, err);
        }
        loader_flags = GetNextLEU32(istr, err);
        rva_table.ReadFromStream(istr, out, err);
    }
    void WriteJSON(std::ostream& ostr, const int& flags, const int& depth) const override {
        COFFOptionalHeader::WriteJSON(ostr, flags, depth);
        if(pe_format == PEFormat::PE32Plus){
            IJSONWritable::WriteJSONHexString(ostr, "Image Base", l_image_base, flags, depth);
        }
        else{
            IJSONWritable::WriteJSONHexString(ostr, "Image Base", dw_image_base, flags, depth);
        }
        IJSONWritable::WriteJSONHexString(ostr, "Section Alignment", section_alignment, flags, depth);
        IJSONWritable::WriteJSONHexString(ostr, "File Alignment", file_alignment, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned(ostr, "Major OS Version", major_os_version, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned(ostr, "Minor OS Version", minor_os_version, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned(ostr, "Major Image Version", major_image_version, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned(ostr, "Minor Image Version", minor_image_version, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned(ostr, "Major Subsystem Version", major_subsystem_version, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned(ostr, "Minor Subsystem Version", minor_subsystem_version, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned(ostr, "Win32 Version Value", win32_version_value, flags, depth);
        IJSONWritable::WriteJSONHexString(ostr, "Size Of Image", size_of_image, flags, depth);
        IJSONWritable::WriteJSONHexString(ostr, "Size Of Headers", size_of_headers, flags, depth);
        IJSONWritable::WriteJSONHexString(ostr, "Check Sum", check_sum, flags, depth);
        IJSONWritable::WriteJSONString(ostr, "Subsystem", GetWindowsSubsystemString((WindowsSubsystem)subsystem), flags, depth);
        IJSONWritable::WriteJSONHexString(ostr, "DLL Characteristics", dll_characteristics, flags, depth);
        if(pe_format == PEFormat::PE32Plus){
            IJSONWritable::WriteJSONHexString(ostr, "Size Of Stack Reserve", l_size_of_stack_reserve, flags, depth);
            IJSONWritable::WriteJSONHexString(ostr, "Size Of Stack Commit", l_size_of_stack_commit, flags, depth);
            IJSONWritable::WriteJSONHexString(ostr, "Size Of Heap Reserve", l_size_of_heap_reserve, flags, depth);
            IJSONWritable::WriteJSONHexString(ostr, "Size Of Heap Commit", l_size_of_heap_commit, flags, depth);
        }
        else{
            IJSONWritable::WriteJSONHexString(ostr, "Size Of Stack Reserve", dw_size_of_stack_reserve, flags, depth);
            IJSONWritable::WriteJSONHexString(ostr, "Size Of Stack Commit", dw_size_of_stack_commit, flags, depth);
            IJSONWritable::WriteJSONHexString(ostr, "Size Of Heap Reserve", dw_size_of_heap_reserve, flags, depth);
            IJSONWritable::WriteJSONHexString(ostr, "Size Of Heap Commit", dw_size_of_heap_commit, flags, depth);
        }
        IJSONWritable::WriteJSONHexString(ostr, "Loader Flags", loader_flags, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned(ostr, "Number Of Data Directories", rva_table.size(), flags, depth, rva_table.isEmpty());
        if(!rva_table.isEmpty()){
            IJSONWritable::WriteJSONObjectAsElement(ostr, "RVA Table", rva_table, flags, depth, true);
        }
    }
};

#endif