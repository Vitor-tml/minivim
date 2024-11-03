#include "minivim.hpp"
#include "arquivos.hpp"
#include <memory>
#define UNTITLENAME "untitled.gli"

int main(int argc, char **argv)
{
    std::string nome;
    
    nome = (argc > 1)? argv[1] : UNTITLENAME;
    auto minivim = std::make_shared<Minivim>(nome);
    minivim->loadBuffer(nome);
    minivim->run();   
    
    std::cout << "Fim\n";
    return 0;
}