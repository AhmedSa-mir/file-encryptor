#include "aes_encryptor.h"

AES_Encryptor::AES_Encryptor(std::string key, int size) : key(key), AES_KEYLENGTH(size)
{
}

AES_Encryptor::~AES_Encryptor()
{
}


void AES_Encryptor::encrypt(const std::string &msg, unsigned char* enc_buff)
{
    const size_t msg_size = msg.length();

    // AES key
    unsigned char aes_key[AES_KEYLENGTH];
    memset(aes_key, 0, AES_KEYLENGTH/8);
    strcpy((char*) aes_key, key.c_str());

    // AES Init Vector
    unsigned char iv[AES_BLOCK_SIZE];
    memset(iv, 0x00, AES_BLOCK_SIZE);

    // Buffer for original message
    unsigned char aes_input[msg_size];
    memset(aes_input, 0, msg_size/8);
    strcpy((char*) aes_input, msg.c_str());

    // Encryption
    AES_KEY enc_key;
    AES_set_encrypt_key(aes_key, AES_KEYLENGTH, &enc_key);
    AES_cbc_encrypt(aes_input, enc_buff, msg_size, &enc_key, iv, AES_ENCRYPT);
}


void AES_Encryptor::decrypt(const std::string &msg, unsigned char* dec_buff)
{
    const size_t enc_msg_size = msg.length();

    // AES key
    unsigned char aes_key[AES_KEYLENGTH];
    memset(aes_key, 0, AES_KEYLENGTH/8);
    strcpy((char*) aes_key, key.c_str());
    aes_key[0] = key[0];

    // AES Init Vector
    unsigned char iv[AES_BLOCK_SIZE];
    memset(iv, 0x00, AES_BLOCK_SIZE);

    // Buffer for encrypted message
    unsigned char enc_buff[enc_msg_size + 1];
    memset(enc_buff, 0, sizeof(enc_buff));
    std::copy(msg.begin(), msg.end(), enc_buff);
    enc_buff[msg.length()] = '\0';

    // Decryption
    AES_KEY dec_key;
    AES_set_decrypt_key(aes_key, AES_KEYLENGTH, &dec_key);
    AES_cbc_encrypt(enc_buff, dec_buff, enc_msg_size, &dec_key, iv, AES_DECRYPT);
}

