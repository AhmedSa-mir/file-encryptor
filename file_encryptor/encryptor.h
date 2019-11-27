#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <iostream>

class Encryptor {

public:
    Encryptor(){}
    ~Encryptor(){}

    virtual void encrypt(const std::string &msg, unsigned char* enc_buff) = 0;
    virtual void decrypt(const std::string &msg, unsigned char* dec_buff) = 0;
};

#endif // ENCRYPTOR_H
