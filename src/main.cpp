#include "minivim.hpp"
#include "arquivos.hpp"
#include <memory>

int main(int argc, char **argv)
{
    std::string nome;
    
    nome = (argc > 1)? argv[1] : "";
    auto minivim = std::make_shared<Minivim>(nome);
    minivim->run();   
    
    std::cout << "Fim\n";
    return 0;
}