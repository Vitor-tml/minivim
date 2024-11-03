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
    size_t x, y;
    char mode;
    std::string filename, status, section, nomeArquivo;

    void m_remove(int);
    std::string m_tabs(std::string &);
    void m_insert(std::string, int);
    void m_append(std::string&);
public:
    Minivim(std::string&);
    ~Minivim();
    void run();
    void loadBuffer(std::string);
    void saveBuffer(std::string);
protected:
    void update();
    void statusLine();
    void input(int);
    void print();

    void up();
    void left();
    void right();
    void down();

};