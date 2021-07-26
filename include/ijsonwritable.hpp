#ifndef IJSONWRITABLE_HPP
#define IJSONWRITABLE_HPP

#include<iostream>
#include<vector>

enum JSONFormattingFlags {
    IGNORE_OBJECT_DEPTH = 0b1,
    NO_ELEMENT_NEWLINE = 0b10,
    NO_ELEMENT_TAB = 0b100
};

class IJSONWritable {
public:
    virtual void WriteJSON(std::ostream& ostr, const int& flags = 0, const int& depth = 0) const = 0;
   
};

#endif