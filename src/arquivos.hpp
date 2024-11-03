#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

class FileManager {
private:
    // Padrão de projeto Singleton
    static FileManager *singleton;
    FileManager(); 
    ~FileManager(); 
    // Mapa para armazenar os arquivos
    std::unordered_map<std::string, std::fstream*> files;   

public:
    static FileManager* getInstance();
    void loadFile(const std::string&);
    std::fstream* getFile(const std::string&);
};
