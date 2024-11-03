#include "minivim.hpp"


Minivim::Minivim(std::string &f){
    gArquivos = FileManager::getInstance();

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, true);
    use_default_colors();
    x = y = 0;
    mode = 'n';
    status = "NORMAL";
    section = "";
    filename = f;

    gArquivos->loadFile(filename);
    file = gArquivos->getFile(filename);
    
    curs_set(1);  // Garanta que o cursor seja exibido
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
        
        move(y, x);  // Atualize a posição do cursor
        refresh();   // Atualize a tela
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
    section = " COLS: " + std::to_string(x) + " | ROWS: " + std::to_string(y) + " | Arquivo: " + filename;
    status.insert(0, " ");
}

void Minivim::statusLine()
{
    start_color();
    if(mode == 'n'){
        init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    }else {
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
    }
    attron(A_REVERSE);
    attron(A_BOLD);
    attron(COLOR_PAIR(1));
    for(int i {}; i < COLS; i++){
        mvprintw(LINES - 1, i, " ");
    }
    mvprintw((int)LINES - 1, 0, status.c_str());
    mvprintw(LINES - 1, COLS - section.length(), &section[0]);
    attroff(A_BOLD);
    attroff(COLOR_PAIR(1));
    attroff(A_REVERSE);
    
    move(y, x);  // Atualize a posição do cursor
    refresh();   // Atualize a tela
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
        case 'w':
            saveBuffer(filename);
            break;
        }
        break;
    case 'i':
        switch (c)
        {
        case 27:
        // case 'n':
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
        case KEY_ENTER: // Algum problema aqui com memória, refazer toda essa lógica
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
    move(y, x);  // Mantenha a posição do cursor
    refresh();   // Atualize a tela
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
    move(y, x);  // Atualize a posição do cursor
}

void Minivim::left()
{
    if( x > 0 ){
        x--;
        move(y, x);  // Atualize a posição do cursor
    }
}

void Minivim::right()
{
    if(x <= COLS && x <= lines[y].length() - 1){
        ++x;
        move(y, x);  // Atualize a posição do cursor
    }
}

void Minivim::down()
{
    if( y < LINES && y < lines.size() - 1){
        ++y;
    }
    if( x >= lines[y].length()){
        x = lines[y].length();
    }
    move(y, x);  // Atualize a posição do cursor
}


void Minivim::loadBuffer(std::string id)
{
    std::fstream *arquivo = gArquivos->getFile(id);
    if(arquivo->is_open()){
        while(!arquivo->eof()){
            std::string buffer;
            std::getline(*arquivo, buffer);
            m_append(buffer);
        }
    }else{
        std::cout << "MiniVim::loadBuffer::Erro ao abrir arquivo!\n";
    }
}

void Minivim::saveBuffer(std::string id)
{
    // Obtenha um ponteiro para o arquivo
    std::fstream *arquivo = gArquivos->getFile(id);

    if (arquivo) {
        // Verifique se o arquivo já está aberto
        if (arquivo->is_open()) {
            // Feche o arquivo
            arquivo->close();
        }

        // Reabra o arquivo em modo de saída (output) e truncamento
        arquivo->open(id, std::ios::out | std::ios::trunc);

        // Verifique se o arquivo foi aberto corretamente
        if (arquivo->is_open()) {
            // Percorra as linhas e salve no arquivo
            for (size_t i = 0; i < lines.size(); ++i) {
                *arquivo << lines[i] << '\n';   // Escreva a linha no arquivo com quebra de linha
            }
            // Feche o arquivo após a escrita
            arquivo->close();
        } else {
            // Erro ao abrir o arquivo
            std::cout << "MiniVim::saveBuffer:: Erro ao abrir o arquivo para escrita!\n";
        }
    } else {
        // Erro ao obter o ponteiro do arquivo
        std::cout << "MiniVim::saveBuffer:: Erro ao acessar o arquivo!\n";
    }
}