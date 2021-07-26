#ifndef ISTREAMRW_HPP
#define ISTREAMRW_HPP

#include<iostream>

class IStreamRW {
public:
    virtual void ReadFromStream(std::istream& istr, std::ostream& out = std::cout, std::ostream& err = std::cerr) = 0;
    //void WriteToStream(std::istream& istr, std::ostream& out = std::cout, std::ostream& err = std::err) = 0;
};

#endif