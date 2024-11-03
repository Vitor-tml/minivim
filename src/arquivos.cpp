#include "arquivos.hpp"

FileManager* FileManager::singleton = nullptr;

FileManager::FileManager() {}

FileManager::~FileManager() 
{
    for (const auto& elemento : files) {
        if (elemento.second) {
            elemento.second->close();
            delete elemento.second;
        }
    }
}

FileManager* FileManager::getInstance() 
{
    if (singleton == nullptr) {
        singleton = new FileManager;
    }
    return singleton;
}

void FileManager::loadFile(const std::string& filename) 
{
    std::fstream *novoArquivo = new std::fstream(filename, std::ios::in | std::ios::out | std::ios::app);
    
    if (!novoArquivo->is_open()) {
        std::cerr << "FileManager::loadFile::Problema ao abrir o arquivo: " << filename << '\n';
        delete novoArquivo;
    } else {
        files[filename] = novoArquivo;
    }
}

std::fstream* FileManager::getFile(const std::string& id) 
{
    auto it = files.find(id);
    if (it == files.end()) {
        std::cerr << "FileManager::getFile::Arquivo " << id << " nao pode ser aberto.\n";
        return nullptr;
    } else {
        return it->second;
    }
}