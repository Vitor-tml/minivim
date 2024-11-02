#pragma once
#pragma clang diagnostic ignored "Wformat-secutity"
#include <ncurses.h>
#include <memory>
#include <vector>
#include "arquivos.hpp"

class Minivim {
private:
    FileManager *gArquivos;
    std::fstream *file;
    std::vector<std::string> lines;
    int x, y;
    char mode;
    std::string status;
public:
    Minivim(std::string&);
    ~Minivim();
    void run();
protected:
    void update();
    void statusLine();
    void input(int);
};