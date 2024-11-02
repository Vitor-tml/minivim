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
    switch (c)
    {
    case KEY_UP:
        up();
        return;
    case KEY_LEFT:
        left();
        return;
    case KEY_RIGHT:
        right();
        return;
    case KEY_DOWN:
        down();
        return;
    }
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
        case 127:
        case KEY_BACKSPACE:
            if( x == 0 && y > 0){
                x = lines[y - 1].length();
                lines[y - 1] += lines[y];
                m_remove(y);
                up();
            }else if( x > 0 ){
                lines[y].erase(--x, 1);
            }
        break;
        case KEY_DC:
            if( x == lines[y].length() && y != lines.size() - 1){
                lines[y] += lines[y + 1];
            }else {
                lines[y].erase(x, 1);
            }
            break;
        case KEY_ENTER:
        case 10:
            if( x < lines[y].length()){
                m_insert(lines[y].substr(x, lines[y].length() - x), y + 1);
                lines[y].erase(x, lines[y].length() - x);
            }
            else{
                m_insert("", y + 1);
            }
            x = 0;
            down();
            break;
        case KEY_BTAB:
        case KEY_CTAB:
        case KEY_STAB:
        case KEY_CATAB:
        case 9:
            lines[y].insert(x, 2, ' ');
            x += 2;
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

std::string Minivim::m_tabs(std::string& line)
{
    std::size_t tab = line.find('\t');
    return (tab == line.npos)? line : m_tabs(line.replace(tab, 1, "    "));
}
void Minivim::m_remove(int number)
{
    lines.erase(lines.begin() + number);
}

void Minivim::m_insert(std::string line, int number)
{
    line = m_tabs(line);
    lines.insert(lines.begin() + number, line);
}
void Minivim::m_append(std::string& line)
{
    line = m_tabs(line);
    lines.push_back(line);
}

void Minivim::up()
{
    if( y > 0 ){
        y--;
    }
    if(x >= lines[y].length()){
        x = lines[y].length();
    }
    move(y, x);
}
void Minivim::left()
{
    if( x > 0 ){
        x--;
        move(y, x);
    }
}
void Minivim::right()
{
    if(x <= COLS && x <= lines[y].length() - 1){
        ++x;
        move(y, x);
    }
}
void Minivim::down()
{
    if( y < LINES && y < lines.size() - 1){
        --y;

    }
    if( x >= lines[y].length()){
        x = lines[y].length();
    }
    move(y, x);
}