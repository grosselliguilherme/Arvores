#ifndef THREE_HPP
#define THREE_HPP

#include <string>
#include <vector> //vetor de nodos
#include <filesystem> //acessar ou manipular arquivos e diretórios // build_node
namespace fs=std::filesystem; //atalho para nao ter que citar toda vez: std::filesystem...

//Tipo de nodo
enum class nodeType{    //usar enum para manter seguranca, e para sinalizar que os valores são fixos nessa constante
    ARQUIVO,            //se usa letras maiusculas quando usar enum
    DIRETORIO
};

struct node{
    std::string nome; 
    std::string caminho;
    nodeType tipo;
    size_t tamanho=0; //size_t inteiro sem sinal, usa pra representar contagens (ja q nao pode ser valor negativo)
    std::vector<node*> filhos;
};

//novo nodo
node* create_node(const std::string& nome, const std::string& caminho, nodeType tipo, size_t tamanho=0){
    node* nodo = new node;
    nodo->nome = nome;
    nodo->caminho = caminho;
    nodo->tipo = tipo;
    nodo->tamanho = tamanho;
    return nodo;
}

node* build_tree(const fs::path& caminho){
    if(!fs::exists(caminho) || fs::is_symlink(caminho)){ //se caminho invalido ou link, retorna
        return nullptr;
    }

    std::string nome=caminho.filename().string(); 
    std::string caminTotal=fs::absolute(caminho).string();

    //se é um arquivo:
    if(fs::is_regular_file(caminho)){
        size_t tamanho=fs::file_size(caminho);
        return create_node(nome, caminTotal, nodeType::ARQUIVO, tamanho);
    }

    //se é um diretorio
    if(fs::is_directory(caminho)){
        node* dir=create_node(nome, caminTotal, nodeType::DIRETORIO, 0);
        //começar a recursão para acessar arquivos e pastas e montar a arvore do diretorio
        for(const auto& entry : fs::directory_iterator(caminho)){
            node* filho=build_tree(entry.path());
            if(filho){ //se o nó é valido:
                dir->filhos.push_back(filho); //adiciona o nodo filho na arvore
                dir->tamanho+=filho->tamanho; //soma os tamanhs dos filhos
            }
        }
        return dir;
    }
    return nullptr;
}

//funcao de exibir a arvore
void show_tree(const node* nodo, const std::string& prefix="", bool ultimo=true){
    if(!nodo) return;

    std::cout << prefix; //prefix usado aqui nas indentações

    //conectores
    std::cout << (ultimo ? "└── " : "├── ");

    //nomes e dados:
    if(nodo->tipo==nodeType::ARQUIVO){
        std::cout << nodo->nome << " (" << nodo->tamanho << " bytes)\n";
    } else{
        size_t contaFilho = nodo->filhos.size();
        std:: cout << nodo->nome << " (" << contaFilho << (contaFilho == 1 ? " filho" : " filhos") << ", " << nodo->tamanho << " bytes)\n";

        //prefixo filhos
        std::string novoPrefixo = prefix + (ultimo ? "    " : "│   ");
        for(size_t i=0;i<contaFilho;i++){
            show_tree(nodo->filhos[i], novoPrefixo, i==contaFilho-1);   
        }
    }
}



#endif