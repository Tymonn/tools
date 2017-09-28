// code.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "aes.h"
#include "base64.h"

const std::string default_key = "AE0pp&#@qq.com+=!681258IPLGF?<>@";

int main()
{
    std::cout << "First choose your command: 1 for encrypt and 2 for decrypt. Type exit() to quit." << std::endl;
    std::string cmd;
    std::cin >> cmd;
    if (cmd == "exit()") {
        return 0;
    }

    if (cmd == "1") {
        std::cout << "input your data and key, and its decrypted data would be printed. Type exit() to quit." << std::endl;
        while (true) {
            std::cout << "input your 32 bits key here:(or the default will be used.)";
            std::string key;
            std::cin >> key;

            if (key == "exit()") {
                break;
            }

            if (key.size() != 32) {
                key = default_key;
            }

            std::cout << "input your data file path here: ";
            std::string file;
            std::cin >> file;

            if (file == "exit()") {
                break;
            }
            std::ifstream ifile;
            ifile.open(file, std::ifstream::binary);
            if (!ifile.is_open()) {
                std::cout << "Your file cannot be opened correctly,sorry.";
                break;
            }
            std::stringstream stream;
            stream << ifile.rdbuf();
            std::string source(stream.str());

            std::cout << "input your result file path here: ";
            std::string path;
            std::cin >> path;
            std::ofstream result_file;
            result_file.open(path, std::ifstream::binary);
            if (!result_file.is_open()) {
                std::cout << "Your file cannot be opened correctly,sorry.";
                break;
            }
            aes a;
            std::string encrypt;
            a.encrypt(key, source, encrypt);
            base64 b;
            std::string encode = b.base64_encode((const unsigned char*)encrypt.c_str(), encrypt.size());

            result_file.write(encode.c_str(), encode.size());
            std::cout << "succeed.";
        }
    }
    else if (cmd == "2") {
        std::cout << "input your data file path and key, and its decrypted data would be printed to the specified result file. Type exit() to quit." << std::endl;
        while (true) {
            std::cout << "input your 32 bits key here:(or the default will be used.)";
            std::string key;
            std::cin >> key;

            if (key == "exit()") {
                break;
            }

            if (key.size() != 32) {
                key = default_key;
            }

            std::cout << "input your data file path here: ";
            std::string file;
            std::cin >> file;

            if (file == "exit()") {
                break;
            }
            std::ifstream ifile;
            ifile.open(file, std::ifstream::binary);
            if (!ifile.is_open()) {
                std::cout << "Your file cannot be opened correctly,sorry.";
                break;
            }
            std::stringstream stream;
            stream << ifile.rdbuf();
            std::string source(stream.str());

            std::cout << "input your result file path here: ";
            std::string path;
            std::cin >> path;
            std::ofstream result_file;
            result_file.open(path, std::ifstream::binary);
            if (!result_file.is_open()) {
                std::cout << "Your file cannot be opened correctly,sorry.";
                break;
            }
            base64 b;
            std::string decode = b.base64_decode(source);
            aes a;
            std::string plain;
            a.decrypt(key, decode, plain);

            result_file.write(plain.c_str(), plain.size());
            std::cout << "succeed.";
        }
    }    

    return 0;
}

