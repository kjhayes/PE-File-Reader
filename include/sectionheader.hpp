
#ifndef SECTIONHEADER_HPP
#define SECTIONHEADER_HPP 

#include "streamreader.hpp"
#include "istreamrw.hpp"
#include "ijsonwritable.hpp"

#include<iostream>
#include<stdint.h>
#include<vector>

class SectionHeader : public IStreamRW, public IJSONWritable {
public:
    uint64_t name;
    uint32_t virtual_size;
    uint32_t virtual_address;
    uint32_t size_of_raw_data;
    uint32_t pointer_to_raw_data;
    uint32_t pointer_to_relocations;
    uint32_t pointer_to_line_numbers;
    uint16_t number_of_relocations;
    uint16_t number_of_line_numbers;
    uint32_t characteristics;

    void ReadFromStream(std::istream& istr, std::ostream& out = std::cout, std::ostream& err = std::cerr) override {
        name = GetNextLEU64(istr, err);
        virtual_size = GetNextLEU32(istr, err);
        virtual_address = GetNextLEU32(istr, err);
        size_of_raw_data = GetNextLEU32(istr, err);
        pointer_to_raw_data = GetNextLEU32(istr, err);
        pointer_to_relocations = GetNextLEU32(istr, err);
        pointer_to_line_numbers = GetNextLEU32(istr, err);
        number_of_relocations = GetNextLEU16(istr, err);
        number_of_line_numbers = GetNextLEU16(istr, err);
        characteristics = GetNextLEU32(istr, err);
    }
    void WriteJSON(std::ostream& ostr, const int& flags, const int& depth) const override {
        IJSONWritable::WriteJSONASCIIStringLE(ostr, "Name", name, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned(ostr, "Virtual Size", virtual_size, flags, depth);
        IJSONWritable::WriteJSONHexString(ostr, "Virtual Address", virtual_address, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned(ostr, "Size Of Raw Data", size_of_raw_data, flags, depth);
        IJSONWritable::WriteJSONHexString(ostr, "Pointer To Raw Data", pointer_to_raw_data, flags, depth);
        IJSONWritable::WriteJSONHexString(ostr, "Pointer To Relocations", pointer_to_relocations, flags, depth);
        IJSONWritable::WriteJSONHexString(ostr, "Pointer To Line Numbers", pointer_to_line_numbers, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned(ostr, "Number Of Relocations", number_of_relocations, flags, depth);
        IJSONWritable::WriteJSONNumberUnsigned(ostr, "Number Of Line Numbers", number_of_line_numbers, flags, depth);
        IJSONWritable::WriteJSONHexString(ostr, "Characteristics", characteristics, flags, depth, true);
    }   
};

class SectionTable : public IStreamRW, public IJSONWritable {
public:
    uint16_t number_of_sections;

    std::vector<SectionHeader> sectionheaders;
    void ReadFromStream(std::istream& istr, std::ostream& out = std::cout, std::ostream& err = std::cerr) override {
        sectionheaders.resize(number_of_sections);
        for(int i = 0; i<number_of_sections; i++){
            out<<i<<std::endl;
            sectionheaders[i].ReadFromStream(istr, out, err);
        }
    }
    void WriteJSON(std::ostream& ostr, const int& flags, const int& depth) const override {
        for(int i = 0; i<sectionheaders.size(); i++){
            IJSONWritable::WriteJSONObjectAsElement(ostr, "SectionHeader", sectionheaders[i], flags, depth, (i==(sectionheaders.size()-1)));
        }
    }
};

#endif