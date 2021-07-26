#ifndef COFFHEADER_HPP
#define COFFHEADER_HPP

#include "streamreader.hpp"
#include "image_file.hpp"
#include "istreamrw.hpp"
#include "ijsonwritable.hpp"

#include<stdint.h>

class COFFHeader : public IStreamRW, public IJSONWritable {
public:
    uint16_t machine;
    uint16_t number_of_sections;
    uint32_t time_date_stamp;
    uint32_t ptr_to_symbol_table;
    uint32_t number_of_symbols;
    uint16_t size_of_optional_header;
    uint16_t characteristics;

    void ReadFromStream(std::istream& istr, std::ostream& out = std::cout, std::ostream& err = std::cerr) override {
        machine = GetNextLEU16(istr, err);
        number_of_sections = GetNextLEU16(istr, err);
        time_date_stamp = GetNextLEU32(istr, err);
        ptr_to_symbol_table = GetNextLEU32(istr, err);
        number_of_symbols = GetNextLEU32(istr, err);
        size_of_optional_header = GetNextLEU16(istr, err);
        characteristics = GetNextLEU16(istr, err);
    } 

    void WriteJSON(std::ostream& ostr, const int& flags, const int& depth) const override {
        IJSONWritable::WriteJSONString(ostr, "Machine", IMAGE_FILE::GetMachineString((IMAGE_FILE::MACHINE)machine), flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned<uint16_t>(ostr, "Number Of Sections", number_of_sections, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned<uint32_t>(ostr, "Time Date Stamp", time_date_stamp, flags, depth);
        IJSONWritable::WriteJSONHexString<uint32_t>(ostr, "Pointer To Symbol Table", ptr_to_symbol_table, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned<uint32_t>(ostr, "Number Of Symbols", number_of_symbols, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned<uint16_t>(ostr, "Size Of Optional Header", size_of_optional_header, flags, depth);
        IJSONWritable::WriteJSONHexString<uint16_t>(ostr, "Characteristics", characteristics, flags, depth, true);
    }
};

#endif