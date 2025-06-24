#include <iostream>
#include "tree.hpp"
//#include "filesystem_tree.hpp"
#include <filesystem>
namespace fs = std::filesystem;

int main(int argc, char* argv[]){
    fs::path startCaminho=(argc>1) ? argv[1] : "."; //funcao para saber se o usuario passou um caminho, se nao "." que significa pasta atual no Linux;
    
    node* raiz=build_tree(startCaminho);
    if(!raiz){ //se erro ao informar ou ao acessar raiz, infora mensagem de erro
        std::cerr << "Erro: caminho invalido.\n";
        return 1;
    }

    //exibir pasta/diretorio e filhos
    std::cout << raiz->caminho << " (" << raiz->filhos.size() << (raiz->filhos.size()==1 ? " filho, " : " filhos, ") << raiz->tamanho << " bytes\n";
    for(size_t i=0;i<raiz->filhos.size();i++){
        show_tree(raiz->filhos[i], "", i==raiz->filhos.size()-1);
    }

    //ver como exporta pra html

    return 0;
}