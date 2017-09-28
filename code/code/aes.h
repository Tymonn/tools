#pragma once
#include <string>

class aes
{
public:
    aes();
    ~aes();

public:
    bool decrypt(const std::string& key, const std::string& encrypt, std::string& plain);
    bool encrypt(const std::string& key, const std::string& plain, std::string& encrypt);
};

