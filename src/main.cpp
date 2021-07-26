#include "pefile.hpp"

int main(int argc, char** argv){
    std::ostream& out = std::cout;
    std::ostream& err = std::cerr;

    if(argc <= 1){
        std::cerr<<"Too Few Arguments Given."<<std::endl;
        return 1;
    }
    if(argc > 2){
        std::cerr<<"Too Many Arguments Given."<<std::endl;
        return 1;
    }

    std::ifstream istr(argv[1]);
    if(!istr.is_open()){
        std::cerr<<"Could Not Open File \""<<argv[1]<<"\""<<std::endl;
        return 1;
    }

    PEFile pe_file;
    pe_file.ReadFromStream(istr, out, err);

    pe_file.WriteJSONObject(out);

    return 0;
}