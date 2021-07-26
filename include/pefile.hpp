#ifndef PEFILE_HPP
#define PEFILE_HPP

#include<iostream>
#include<fstream>

#include "streamreader.hpp"
#include "istreamrw.hpp"
#include "coffheader.hpp"
#include "coffoptionalheader.hpp"
#include "sectionheader.hpp"

class PEFile : public IStreamRW, public IJSONWritable {
public:
    uint32_t pe_header_offset;
    COFFHeader coff_header;
    COFFOptionalHeader* coff_optional_header;
    SectionTable section_table;

    PEFile(){
        coff_optional_header = new COFFOptionalHeader_Windows();
    }

    void ReadFromStream(std::istream& istr, std::ostream& out = std::cout, std::ostream& err = std::cerr){
        istr.seekg(0x3c);
        uint32_t temp_pe_header_offset = GetNextLEU32(istr, err);
        istr.seekg(temp_pe_header_offset);
        if(GetNextLEU32(istr, err) != 0x00004550){
            err<<"Error: PE Magic Number Was Not Found."<<std::endl;
            return;
        }
        else{pe_header_offset = temp_pe_header_offset;}
        coff_header.ReadFromStream(istr, out, err);
        coff_optional_header->ReadFromStream(istr, out, err);
        section_table.number_of_sections = coff_header.number_of_sections;
        section_table.ReadFromStream(istr, out, err);
    }

    void WriteJSON(std::ostream& ostr, const int& flags, const int& depth) const override {
        IJSONWritable::WriteJSONHexString<uint32_t>(ostr, "PE Header Offset", pe_header_offset, flags, depth);
        IJSONWritable::WriteJSONObjectAsElement(ostr, "COFF Header", coff_header, flags, depth);
        IJSONWritable::WriteJSONObjectAsElement(ostr, "COFF Optional Header", *coff_optional_header, flags, depth);
        IJSONWritable::WriteJSONObjectAsElement(ostr, "Section Table", section_table, flags, depth, true);
    }
};

#endif