#include "minivim.hpp"
#include "arquivos.hpp"
#include <memory>
#define UNTITLENAME "untitled.gli"

void printHelp();
int main(int argc, char **argv)
{
    std::string nome;
    if(argc > 1)
    {
        for (int i = 1; i < argc; ++i) {
            if (std::string(argv[i]) == "-help") {
                printHelp();
                return 0; // Encerrar o programa
            }
        }
        for (int i = 1; i < argc; ++i){
            if(std::string(argv[i]).find('-') != std::string::npos){
                std::cout << "Argumento Invalido\n";
                return 0;
            }
        }
    }
    nome = (argc > 1)? argv[1] : UNTITLENAME;
    auto minivim = std::make_shared<Minivim>(nome);
    minivim->loadBuffer(nome);
    minivim->run();   
    
    std::cout << "Fim\n";
    return 0;
}

void printHelp()
{
    std::cout << "MiniVim - Editor de Texto\n";
    std::cout << "Uso:\n";
    std::cout << "  minivim <arquivo>\n\n";
    std::cout << "Comandos:\n";
    std::cout << "  i       - Modo de inserção\n";
    std::cout << "  ESC     - Modo normal\n";
    std::cout << "  q       - Sair do MiniVim\n";
    std::cout << "  w       - Salvar o arquivo\n";
    std::cout << "Setas - Mover o cursor\n";
}