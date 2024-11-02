#include "minivim.hpp"
#define UNTITLENAME "untitled.gli"

Minivim::Minivim(std::string &filename){
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);

    gArquivos = FileManager::getInstance();
    if(filename.empty())
    {
        filename = UNTITLENAME; 
    }
    
    gArquivos->loadFile(filename);
    file = gArquivos->getFile(filename);
    
}

Minivim::~Minivim()
{
    refresh();
    endwin();
}

void Minivim::run(){
    printw("Hello World!\n");
    getch();
}