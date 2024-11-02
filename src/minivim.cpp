#include "minivim.hpp"
#define UNTITLENAME "untitled.gli"

Minivim::Minivim(std::string &filename){
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);

    x = y = 0;
    mode = 'n';
    status = "NORMAL";
    gArquivos = FileManager::getInstance();
    if(filename.empty())
    {
        filename = UNTITLENAME; 
    }
    
    gArquivos->loadFile(filename);
    file = gArquivos->getFile(filename);

    lines.push_back("");
}

Minivim::~Minivim()
{
    refresh();
    endwin();
}

void Minivim::run(){
    while(mode != 'q'){
        update();
        statusLine();
        int c = getch();
        input(c);
    }
}

void Minivim::update()
{
    switch (mode)
    {
    case 27:    
    case 'n':
        status = "NORMAL";
        break;
    case 'i':
        status = "INSERT";
        break;    
    case 'q':
        break;
    };
}

void Minivim::statusLine()
{
    attron(A_REVERSE);
    mvprintw(LINES - 1, 0, status.c_str());
    attroff(A_REVERSE);
}

void Minivim::input(int c)
{
    switch (mode)
    {
    case 27:    
    case 'n':
        switch (c)
        {
        case 27:
        case 'n':
            mode = 'n';
            break;
        case 'i':
            mode = 'i';
            break;
        }
        break;
    case 'i':
        switch (c)
        {
        case 27:
        case 'n':
            mode = 'n';
            break;
        default:
            std::string s(1, c);
            lines.push_back(s);
        }
        break;   
    case 'q':
        break;
    };

    for(size_t i {}; i < lines.size(); ++i){
        mvprintw(0, i, lines[i].c_str());
    }
}