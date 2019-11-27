#ifndef AES_ENCRYPTOR_H
#define AES_ENCRYPTOR_H

#include "openssl/aes.h"
#include "encryptor.h"
#include <string.h>

class AES_Encryptor : public Encryptor {

public:
    AES_Encryptor(std::string key, int size);
    AES_Encryptor(){}
    ~AES_Encryptor();

    virtual void encrypt(const std::string &msg, unsigned char* enc_buff);
    virtual void decrypt(const std::string &msg, unsigned char* dec_buff);

private:
    const int AES_KEYLENGTH = 256;
    const std::string key = "12345678912345678912345678912345";
};

#endif // AES_ENCRYPTOR_H
