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

    //menu
    while(true){
        int opcao;
        std::cout << "\n        MENU:        \n";
        std::cout << "1-Exibir árvore\n";
        std::cout << "2-Exportar árvore para HTML\n";
        std::cout << "0-SAIR!\n";
        std::cin >> opcao;

        switch(opcao){
            case 1:
                //exibir arvore - pasta/diretorio e filhos
                std::cout << raiz->caminho << " (" << raiz->filhos.size() << (raiz->filhos.size()==1 ? " filho, " : " filhos, ") << raiz->tamanho << " bytes\n";
                for(size_t i=0;i<raiz->filhos.size();i++){
                    show_tree(raiz->filhos[i], "", i==raiz->filhos.size()-1);
                }
                break;

            case 2:
               tree_to_html(raiz);
               break;
            case 0:
                return 0;
            default:
                std::cout << "Opção inválida!\n";   
        }
    }

    return 0;
}