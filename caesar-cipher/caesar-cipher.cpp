#pragma execution_character_set( "utf-8" )

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <codecvt>
#include <io.h>
#include <fcntl.h>
#include <windows.h>


const std::wstring alphabet_uppercase = L"ABCDEFGHIJKLMNOPQRSTUVWXYZÆØÅ";
const std::wstring alphabet_lowercase = L"abcdefghijklmnopqrstuvwxyzæøå";
const std::wstring alphabet = alphabet_uppercase + alphabet_lowercase;

std::wstring get_cipher_alphabet(int n) {
    std::wstring cipher_alphabet;
    const int size = min(alphabet_uppercase.size(), alphabet_lowercase.size());
    if (n > size)
        n %= size;

    if (n == 0)
        return alphabet;

    if (n > 0) {
        cipher_alphabet 
            = alphabet_uppercase.substr(n) += alphabet_uppercase.substr(0, n)
            += alphabet_lowercase.substr(n) += alphabet_lowercase.substr(0, n);
    }
    else {
        n = -n;
        cipher_alphabet
            = alphabet_uppercase.substr(size - n, n) += alphabet_uppercase.substr(0, size - n)
            += alphabet_lowercase.substr(size - n, n) += alphabet_lowercase.substr(0, size - n);
    }
    return cipher_alphabet;
}


std::wstring cipher(std::wstring in, int n) {
    std::wstring cipherAlphabet = get_cipher_alphabet(n);

    std::wstring::iterator it = in.begin();
    while (it != in.end()) {
        int index = alphabet.find(*it);
        if (index != std::string::npos) {
            *it = cipherAlphabet.at(index);
        }
        it++;
    }

    return in;
}

std::wstring decipher(std::wstring in, int n) {
    return cipher(in, -n);
}

std::wstring read_file(std::wstring filename) {
    // file input, interpret as UTF-8
    std::wifstream in(filename);
    in.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));

    std::wstringstream wss;
    wss << in.rdbuf();
    if (wss.str().size() > 0) {
        // stringstream has garbage character at beginning
        return wss.str().substr(1);
    }

    std::wcout << "File not found: " << filename << std::endl;
    return std::wstring();
}

int wmain(int argc, wchar_t *argv[], char* envp[])
{
    if (argc < 3) {
        std::cout << "Usage: caesar-cipher KEY FILENAME [-d|--decipher]" << std::endl;
        return 1;
    }
    // fix console output on windows cmd
    _setmode(_fileno(stdout), _O_U16TEXT);
    SetConsoleOutputCP(CP_UTF8);

    // parse arguments
    int cipherKey = std::stoi(argv[1]);
    std::wstring cipherFilename = std::wstring(argv[2]);
    bool decipherContent = argc >= 4 && ((wcscmp(argv[3], L"-d") == 0 || wcscmp(argv[3], L"--decipher") == 0));

    std::wstring fileContent = read_file(cipherFilename);
    if (fileContent.size() == 0)
        return 2;

    std::wstring output = decipherContent ? decipher(fileContent, cipherKey)
                                           : cipher(fileContent, cipherKey);
    std::wcout << output << std::endl;
}