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
        print();
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
    mvprintw((int)LINES - 1, 0, status.c_str());
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
        case 'i':
            mode = 'i';
            break;
        case 'q':
            mode = 'q';
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
            lines[y].insert(x, 1, c);
            ++x;
        }
        break;
    };

    for(size_t i {}; i < lines.size(); ++i){
        mvprintw(0, i, lines[i].c_str());
    }
}

void Minivim::print()
{
    for(size_t i{}; i < (size_t)LINES - 1; ++i){
        if(i >= lines.size()){
            move(i, 0);
            clrtoeol();
        }else {
            mvprintw(i, 0, lines[i].c_str());
            clrtoeol();
        }
    }
    move(y, x);
}