#ifndef STREAMREADER_HPP
#define STREAMREADER_HPP

#include<stdint.h>
#include<iostream>

class StreamReaderStats{
public:
    static uintmax_t bytes_read;
};
uintmax_t StreamReaderStats::bytes_read = 0;

uint8_t GetNextU8(std::istream& istr, std::ostream& err = std::cerr){
    static std::streampos prev_pos;
    if(istr.eof()){err<<"Expected More Data, But EOF Reached: Previous streampos Was: "<<std::hex<<prev_pos<<std::dec<<std::endl;}
    prev_pos = istr.tellg();
    StreamReaderStats::bytes_read++;
    return istr.get();
}

uint16_t GetNextLEU16(std::istream& istr, std::ostream& err = std::cerr){
    uint16_t u = (uint16_t)GetNextU8(istr);
    u |= (((uint16_t)GetNextU8(istr)) << 8);
    return u;
}
uint32_t GetNextLEU32(std::istream& istr, std::ostream& err = std::cerr){
    uint32_t u = (uint32_t)GetNextLEU16(istr);
    u |= (((uint32_t)GetNextLEU16(istr)) << 16);
    return u;
}
uint64_t GetNextLEU64(std::istream& istr, std::ostream& err = std::cerr){
    uint64_t u = (uint64_t)GetNextLEU32(istr);
    u |= (((uint64_t)GetNextLEU32(istr)) << 32);
    return u;
}

#endif