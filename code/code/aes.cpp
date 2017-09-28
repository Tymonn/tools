#include "stdafx.h"
#include "aes.h"
#include <sstream>
#include "openssl/crypto/aes/aes.h"
#include <iomanip>

aes::aes()
{
}


aes::~aes()
{
}


bool aes::encrypt(const std::string& key, const std::string& src, std::string& encrypt) {
    std::string plain = src;
    if (key.empty() || plain.empty()) {
        return false;
    }

    int source_data_length = plain.length();
    int total_encrypted_data_length = source_data_length;
    int appended_data_length = 0;  //被填充到原数据后面的字节数
    int profix = 0;  //后缀字节数，0或者AES_BLOCK_SIZE，记录appended_data_length
    if (source_data_length % AES_BLOCK_SIZE != 0) {
        int minimal_length_encode = (source_data_length / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
        appended_data_length = minimal_length_encode - source_data_length;
        plain.append(appended_data_length, 0);
        total_encrypted_data_length += appended_data_length;
    }
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(AES_BLOCK_SIZE) << appended_data_length;
    plain.append(stream.str());
    total_encrypted_data_length += stream.str().size();

    unsigned char * input = new unsigned char[total_encrypted_data_length + 1];
    if (input == nullptr) {
        return false;
    }
    memset(input, 0, total_encrypted_data_length + 1);
    memcpy(input, plain.c_str(), total_encrypted_data_length);

    unsigned char *output = new unsigned char[total_encrypted_data_length + 1];
    if (output == nullptr) {
        delete[] input;
        return false;
    }
    memset(output, 0, total_encrypted_data_length + 1);

    AES_KEY aes_key;
    memset(&aes_key, 0, sizeof(AES_KEY));
    int success = AES_set_encrypt_key((const unsigned char*)key.c_str(), 256, &aes_key);
    if (success < 0) {
        return false;
    }

    unsigned char init_vector[AES_BLOCK_SIZE] = { 0 };
    if (key.length() >= 16) {
        for (int i = 0; i < AES_BLOCK_SIZE; ++i) {
            init_vector[i] = key[i];
        }
    }
    AES_cbc_encrypt((const unsigned char*)input, output, total_encrypted_data_length, &aes_key, init_vector, AES_ENCRYPT);

    encrypt.assign((char*)output, total_encrypted_data_length);
    delete[] input;
    delete[] output;

    return true;
}


bool aes::decrypt(const std::string& key, const std::string& encrypt, std::string& decrypt) {
    if (key.empty() || encrypt.empty()) {
        return false;
    }

    int data_length = encrypt.length();
    if (data_length % AES_BLOCK_SIZE != 0) {
        return false;
    }

    unsigned char * input = new unsigned char[data_length + 1];
    if (input == nullptr) {
        return false;
    }
    memset(input, 0, data_length + 1);
    memcpy(input, encrypt.c_str(), data_length);

    unsigned char *output = new unsigned char[data_length + 1];
    if (output == nullptr) {
        delete[] input;
        return false;
    }
    memset(output, 0, data_length + 1);

    AES_KEY aes_key;
    memset(&aes_key, 0, sizeof(AES_KEY));
    int success = AES_set_decrypt_key((const unsigned char*)key.c_str(), 256, &aes_key);
    if (success < 0) {
        return false;
    }

    unsigned char init_vector[AES_BLOCK_SIZE] = { 0 };
    if (key.length() >= 16) {
        for (int i = 0; i < AES_BLOCK_SIZE; ++i) {
            init_vector[i] = key[i];
        }
    }
    AES_cbc_encrypt((const unsigned char*)input, output, data_length, &aes_key, init_vector, AES_DECRYPT);
    std::string decrypt_data;
    decrypt_data.assign((char*)output, data_length);
    std::string profix = decrypt_data.substr(decrypt_data.length() - AES_BLOCK_SIZE);
    int appended_bytes = atoi(profix.c_str());
    std::string source_decrypted__data = decrypt_data.substr(0,decrypt_data.length() - AES_BLOCK_SIZE - appended_bytes);

    decrypt.assign(source_decrypted__data);
    delete[] input;
    delete[] output;

    return true;
}


