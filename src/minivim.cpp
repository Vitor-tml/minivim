#include "minivim.hpp"

Minivim::Minivim(std::string &f)
{
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

    curs_set(1); // Garanta que o cursor seja exibido
}

Minivim::~Minivim()
{
    refresh();
    endwin();
}

void Minivim::run()
{
    while (mode != 'q')
    {
        print();
        update();
        statusLine();
        int c = getch();
        input(c);

        // Mova o cursor para a posição correta, levando em conta as bordas
        move(y + 1, x + 1);  
        refresh();  // Atualize a tela
    }
}

void Minivim::update()
{
    switch (mode)
    {
    case 27:
    case 'n':
        status = "NORMAL ";
        break;
    case 'i':
        status = "INSERT ";
        break;
    case 'q':
        break;
    };
    section = " COLS: " + std::to_string(x) + " | ROWS: " + std::to_string(y) + " | Arquivo: " + filename + " ";
    status.insert(0, " ");
}

void Minivim::statusLine()
{
    for (int i = 1; i < COLS - 1; i++){
        mvaddch(LINES - 1, i, ACS_HLINE);
    }
    mvprintw((int)LINES - 1, 2, status.c_str());
    mvprintw(LINES - 1, COLS - 2 - section.length(), &section[0]);

    move(y + 1, x + 1);  
    refresh();  // Atualize a tela
}

void Minivim::input(int c)
{
    // Setas de movimentação
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

    // Controle de estado
    switch (mode)
    {
    case 27:
    case 'n':
        normalMode(c);
        break;
    case 'i':
        insertMode(c);
    };
}
void Minivim::print()
{
    // Desenhar o conteúdo do editor dentro da caixa
    for (size_t i = 0; i < (size_t)LINES - 2; ++i)
    {
        if (i >= lines.size())
        {
            move(i + 1, 2); // Mover um para baixo devido à borda
            clrtoeol();
        }
        else
        {
            mvprintw(i + 1, 1, lines[i].c_str()); // Mover para a direita
            clrtoeol();
        }
    }

    // Desenhar a borda
    for (size_t i = 0; i < (size_t)LINES - 1; ++i)
    {
        for (size_t j = 0; j < (size_t)COLS; ++j)
        {
            if (i == 0 || i == LINES - 1) // Linhas superior e inferior
            {
                mvaddch(i, j, ACS_HLINE);
            }
            if (j == 0 || j == COLS - 1) // Coluna esquerda e direita
            {
                mvaddch(i, j, ACS_VLINE);
            }
        }
    }

    // Desenhar os cantos da caixa
    mvaddch(0, 0, ACS_ULCORNER);           // Canto superior esquerdo
    mvaddch(0, COLS - 1, ACS_URCORNER);    // Canto superior direito
    mvaddch(LINES - 1, 0, ACS_LLCORNER);   // Canto inferior esquerdo
    mvaddch(LINES - 1, COLS - 1, ACS_LRCORNER); // Canto inferior direito

    // Ajustar a posição do cursor para dentro da caixa
    move(y + 1, x + 1); // Adiciona 1 para compensar a borda
    refresh();          // Atualize a tela
}


// O restante da classe permanece inalterado

std::string Minivim::m_tabs(std::string &line)
{
    std::size_t tab = line.find('\t');
    return (tab == line.npos) ? line : m_tabs(line.replace(tab, 1, "    "));
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
void Minivim::m_append(std::string &line)
{
    line = m_tabs(line);
    lines.push_back(line);
}

void Minivim::up()
{
    if (y > 0)
    {
        y--;
    }
    if (x >= lines[y].length())
    {
        x = lines[y].length();
    }
    move(y + 1, x + 1);  // Ajustar para a posição correta na caixa
}

void Minivim::left()
{
    if (x > 0)
    {
        x--;
        move(y + 1, x + 1);  // Ajustar para a posição correta na caixa
    }
}

void Minivim::right()
{
    if (x < lines[y].length())
    {
        ++x;
        move(y + 1, x + 1);  // Ajustar para a posição correta na caixa
    }
}

void Minivim::down()
{
    if (y < lines.size() - 1)
    {
        ++y;
    }
    if (x >= lines[y].length())
    {
        x = lines[y].length();
    }
    move(y + 1, x + 1);  // Ajustar para a posição correta na caixa
}

void Minivim::loadBuffer(std::string id)
{
    std::fstream *arquivo = gArquivos->getFile(id);
    if (arquivo->is_open())
    {
        while (!arquivo->eof())
        {
            std::string buffer;
            std::getline(*arquivo, buffer);
            m_append(buffer);
        }
    }
    else
    {
        std::cout << "MiniVim::loadBuffer::Erro ao abrir arquivo!\n";
    }
}

void Minivim::saveBuffer(std::string id)
{
    std::fstream *arquivo = gArquivos->getFile(id);

    if (arquivo)
    {
        if (arquivo->is_open())
        {
            arquivo->close();
        }

        arquivo->open(id, std::ios::out | std::ios::trunc);

        if (arquivo->is_open())
        {
            for (size_t i = 0; i < lines.size(); ++i)
            {
                *arquivo << lines[i] << '\n';
            }
            arquivo->close();
        }
        else
        {
            // Erro ao abrir o arquivo
            std::cout << "MiniVim::saveBuffer:: Erro ao abrir o arquivo para escrita!\n";
        }
    }
    else
    {
        // Erro ao obter o ponteiro do arquivo
        std::cout << "MiniVim::saveBuffer:: Erro ao acessar o arquivo!\n";
    }
}

void Minivim::normalMode(int c)
{
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
}

void Minivim::insertMode(int c)
{
    switch (c)
        {
        case 27:
            mode = 'n';
            break;
        case 127:
        case KEY_BACKSPACE:
            if (x == 0 && y > 0)
            {
                x = lines[y - 1].length();
                lines[y - 1] += lines[y];
                m_remove(y);
                up();
            }
            else if (x > 0)
            {
                lines[y].erase(--x, 1);
            }
            break;
        case KEY_DC:
            if (x == lines[y].length() && y != lines.size() - 1)
            {
                lines[y] += lines[y + 1];
            }
            else
            {
                lines[y].erase(x, 1);
            }
            break;
        case KEY_ENTER: // Algum problema aqui com memória, refazer toda essa lógica
        case 10:
            if (x < lines[y].length())
            {
                m_insert(lines[y].substr(x, lines[y].length() - x), y + 1);
                lines[y].erase(x, lines[y].length() - x);
            }
            else
            {
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
}