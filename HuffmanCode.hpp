#ifndef HuffmanCode_h
#define HuffmanCode_h

class HuffmanCode{
public:
    char data;
    std::string code;
    
    HuffmanCode(char data, std::string code){
        this->data = data;
        this->code = code;
    }
};

#endif 
