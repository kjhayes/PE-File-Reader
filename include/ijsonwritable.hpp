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
    void WriteJSONObject(std::ostream& ostr, const int& flags = 0, const int& depth = 0) const {
        ostr<<"{";
        
        WriteJSON(ostr, flags, depth);

        if((flags & JSONFormattingFlags::NO_ELEMENT_NEWLINE) == 0){
            ostr<<"\n";
            if((flags & JSONFormattingFlags::IGNORE_OBJECT_DEPTH) == 0){
                for(int i = 0; i < depth; i++){ostr<<"\t";}
            }
        }
        ostr<<"}";
    }
    
    static void WriteJSONObjectAsElement(std::ostream& ostr, const char* member_name, const IJSONWritable& obj, 
                                        const int& flags, int depth, const bool& is_last = false){
        if((flags & JSONFormattingFlags::NO_ELEMENT_NEWLINE) == 0){
            ostr<<"\n";
            if((flags & JSONFormattingFlags::IGNORE_OBJECT_DEPTH) == 0){
                for(int i = 0; i < depth; i++){ostr<<"\t";}
            }
        }
        if((flags & JSONFormattingFlags::NO_ELEMENT_TAB) == 0){ostr<<"\t";}
        ostr<<"\""<<member_name<<"\": ";
        obj.WriteJSONObject(ostr, flags, depth+1);
        if(!is_last){ostr<<", ";}
    }
    static void WriteJSONString(std::ostream& ostr, const char* member_name, const char* member_value, 
                                const int& flags, const int& depth, const bool& is_last = false){
        if((flags & JSONFormattingFlags::NO_ELEMENT_NEWLINE) == 0){
            ostr<<"\n";
            if((flags & JSONFormattingFlags::IGNORE_OBJECT_DEPTH) == 0){
                for(int i = 0; i < depth; i++){ostr<<"\t";}
            }
        }
        if((flags & JSONFormattingFlags::NO_ELEMENT_TAB) == 0){ostr<<"\t";}
        ostr<<"\""<<member_name<<"\": \""<<member_value<<"\"";
        if(!is_last){ostr<<", ";}    
    }
    template<class T>
    static void WriteJSONNumberUnsigned(std::ostream& ostr, const char* member_name, const T& member_value, 
                                        const int& flags, const int& depth, const bool& is_last = false){
        if((flags & JSONFormattingFlags::NO_ELEMENT_NEWLINE) == 0){
            ostr<<"\n";
            if((flags & JSONFormattingFlags::IGNORE_OBJECT_DEPTH) == 0){
                for(int i = 0; i < depth; i++){ostr<<"\t";}
            }
        }
        if((flags & JSONFormattingFlags::NO_ELEMENT_TAB) == 0){ostr<<"\t";}
        ostr<<"\""<<member_name<<"\": "<<(uintmax_t)member_value;    
        if(!is_last){ostr<<", ";}
    }
    template<class T>
    static void WriteJSONNumberSigned(std::ostream& ostr, const char* member_name, const T& member_value, 
                                      const int& flags, const int& depth, const bool& is_last = false){
        if((flags & JSONFormattingFlags::NO_ELEMENT_NEWLINE) == 0){
            ostr<<"\n";
            if((flags & JSONFormattingFlags::IGNORE_OBJECT_DEPTH) == 0){
                for(int i = 0; i < depth; i++){ostr<<"\t";}
            }
        }
        if((flags & JSONFormattingFlags::NO_ELEMENT_TAB) == 0){ostr<<"\t";}
        ostr<<"\""<<member_name<<"\": "<<(intmax_t)member_value;    
        if(!is_last){ostr<<", ";}
    }

    template<class T>
    static void WriteJSONHexString(std::ostream& ostr, const char* member_name, const T& member_value, 
                                           const int& flags, const int& depth, const bool& is_last = false){
        if((flags & JSONFormattingFlags::NO_ELEMENT_NEWLINE) == 0){
            ostr<<"\n";
            if((flags & JSONFormattingFlags::IGNORE_OBJECT_DEPTH) == 0){
                for(int i = 0; i < depth; i++){ostr<<"\t";}
            }
        }
        if((flags & JSONFormattingFlags::NO_ELEMENT_TAB) == 0){ostr<<"\t";}
        ostr<<"\""<<member_name<<"\": \"0x"<<std::hex<<(uintmax_t)member_value<<std::dec<<"\"";    
        if(!is_last){ostr<<", ";}
    }
    template<class T>
    static void WriteJSONASCIIStringLE(std::ostream& ostr, const char* member_name, const T& member_value, 
                                         const int& flags, const int& depth, const bool& is_last = false){
        if((flags & JSONFormattingFlags::NO_ELEMENT_NEWLINE) == 0){
            ostr<<"\n";
            if((flags & JSONFormattingFlags::IGNORE_OBJECT_DEPTH) == 0){
                for(int i = 0; i < depth; i++){ostr<<"\t";}
            }
        }
        if((flags & JSONFormattingFlags::NO_ELEMENT_TAB) == 0){ostr<<"\t";}
        ostr<<"\""<<member_name<<"\": \"";
        for(int i = 0; i < (sizeof(T)*0x8); i+=0x8){
            ostr<<((char)(member_value>>i));
        }
        ostr<<"\"";    
        if(!is_last){ostr<<", ";}
    }
};

#endif