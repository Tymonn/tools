#pragma once
#include <string>

class base64
{
public:
    base64();
    ~base64();

    std::string base64_encode(unsigned char const*, unsigned int len);
    std::string base64_decode(std::string const& s);
    bool is_base64(unsigned char c);
};

