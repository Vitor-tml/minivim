#pragma once
#pragma clang diagnostic ignored "Wformat-secutity"
#include <ncurses.h>
#include <memory>
#include "arquivos.hpp"

class Minivim {
private:
    FileManager *gArquivos;
    std::fstream *file;
public:
    Minivim(std::string&);
    ~Minivim();
    void run();
};