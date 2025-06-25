#ifndef THREE_HPP
#define THREE_HPP

#include <string>
#include <vector> //vetor de nodos
#include <fstream> //trabalhar com arquivos; arvore em html; ofstream
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

//item 1 - estrutura da arvore:
//funcao de exibir a arvore
void show_tree(const node* nodo, const std::string& prefix="", bool ultimo=true){
    if(!nodo) return;
    std::cout << prefix; //prefix usado aqui nas indentações
    //conectores
    std::cout << (ultimo ? "└── " : "├── ");
    //nomes e dados:
    if(nodo->tipo==nodeType::ARQUIVO){ //se arquivo, nome e tamanho
        std::cout << nodo->nome << " (" << nodo->tamanho << " bytes)\n";
    } else{ //se diretorio, nome, tamanho e filhos
        size_t contaFilho = nodo->filhos.size();
        std::cout << nodo->nome << " (" << contaFilho << (contaFilho == 1 ? " filho" : " filhos") << ", " << nodo->tamanho << " bytes)\n";
        //prefixo filhos
        std::string novoPrefixo = prefix + (ultimo ? "    " : "│   ");
        for(size_t i=0;i<contaFilho;i++){
            show_tree(nodo->filhos[i], novoPrefixo, i==contaFilho-1);   
        }
    }
}

//item 2 - exportacao para html:
//monta arvore:
void export_tree_recursive(std::ofstream& out, const node* nodo, const std::string& prefix="", bool ultimo=true){//manter a recursividade, indentacao e etc... da arvore
    out << prefix; //out representa ofstream, para modificar e acessar arquivos.
    out << (ultimo ? "└── " : "├── ");
    if(nodo->tipo==nodeType::ARQUIVO){
        out << nodo->nome << " (" << nodo->tamanho << " bytes)\n";
    } else{
        size_t cont = nodo->filhos.size();
        out << nodo->nome << " (" << cont << (cont==1 ? " filho, " : " filhos, ") << nodo->tamanho << " bytes)\n";
        std::string novoPrefixo = prefix + (ultimo ? "    " : "│   ");
        for(size_t i=0;i<cont;i++){
            export_tree_recursive(out, nodo->filhos[i], novoPrefixo, i==cont-1);   
        }

    }
}

//transforma em arquivo:
void tree_to_html(const node* raiz, const std::string& salvaArquivo="arvore.html"){
    std::ofstream out(salvaArquivo);//abre o arquivo para escrever nele
    if(!out){
        std::cerr << "Erro ao criar arquivo\n";
        return;
    }
    out << "<!DOCTYPE html>\n<html>\n<head><meta charset=\"UTF-8\"><title>Árvore de Arquivos</title></head>\n<body>\n";
    out << "<pre>\n";

    //arvore:
    out << raiz->caminho << " (" << raiz->filhos.size() << (raiz->filhos.size() == 1 ? " filho, " : " filhos, ") << raiz->tamanho << " bytes)\n";
    for(size_t i=0;i<raiz->filhos.size();i++){
        export_tree_recursive(out, raiz->filhos[i], "", i==raiz->filhos.size()-1);   
    }
    out << "</pre>\n</body>\n</html>\n";
    out.close();//fecha o arquivo
    std::string comando = "xdg-open " + salvaArquivo + " &";
    system(comando.c_str());
}

//item 3 - pesquisas:
//3.1 - maior arquivo:
//encontrar o maior arquivo:
void largest_file(const node* nodo, size_t& max, std::vector<const node*>& maior){
    if(!nodo) return;
    if(nodo->tipo==nodeType::ARQUIVO){
        if(nodo->tamanho > max){
            max=nodo->tamanho;
            maior.clear();
            maior.push_back(nodo);
        } else if(nodo->tamanho==max){
            maior.push_back(nodo);
        }
    }
    for(const auto& filho : nodo->filhos){
        largest_file(filho, max, maior);
    }
}

//mostrar o maior arquivo:
void show_largest_file(const node* raiz){
    size_t max=0;
    std::vector<const node*> maior;
    largest_file(raiz, max, maior);
    std::cout << "Maior(es) arquivo(s):\n";
    for(const auto* arquivo : maior){
        std::cout << arquivo->caminho  << " (" << arquivo->tamanho << " bytes)\n";
    }
}

//3.2 arquivo maior que N:
//acha maior arquivo:
void bigger_than(const node* nodo, size_t limite, std::vector<const node*>& resultado){
    if(!nodo) return;
    if(nodo->tipo==nodeType::ARQUIVO && nodo->tamanho > limite){
        resultado.push_back(nodo);
    }
    for(const auto& filho : nodo->filhos){
        bigger_than(filho, limite, resultado);
    }
}

//mostra maior arquivo:
void show_bigger_than(const node* raiz){
    size_t N;
    std::cout << "Digite um valor N (tamanho mínimo de bytes):\n";
    std::cin >> N;
    std::vector<const node*> resultado;
    bigger_than(raiz, N, resultado);
    std::cout << "Arquivos com mais do que " << N << " bytes (N=" << N << ")\n";
    for(const auto* arquivo : resultado){
        std::cout << arquivo->caminho << " (" << arquivo->tamanho << " bytes)\n";
    }
}

//3.3 pasta com mais arquivos:
//acha a pasta com mais arquivos
void dir_most_files(const node* nodo, size_t& maisArquivos, std::vector<const node*>& resultado){
    size_t cont=nodo->filhos.size();
    if(cont > maisArquivos){
        maisArquivos=cont;
        resultado.clear();
        resultado.push_back(nodo);
    } else if(cont==maisArquivos && cont > 0){
        resultado.push_back(nodo);
    }
    for(const auto& filho : nodo->filhos){
        dir_most_files(filho, maisArquivos, resultado);
    }
}

//exibe a pasta com mais arquivos:
void show_dir_most_files(const node* raiz){
    size_t filhosMax=0;
    std::vector<const node*> resultado;
    dir_most_files(raiz, filhosMax, resultado);
    std::cout << "Pasta(s) com mais arquivos (diretos):\n";
    for (const auto* pasta : resultado) {
        std::cout << pasta->caminho << " (" << pasta->filhos.size() << (pasta->filhos.size() == 1 ? " filho, " : " filhos, ") << pasta->tamanho << " bytes)\n";
    }
}

//3.4 arquivos por extensão:
//verificar se o arquivo possui extensão:
bool extension(const std::string& arquivo, const std::string& ext){
    if(ext.size() > arquivo.size()) return false;
    return arquivo.substr(arquivo.size() - ext.size())==ext; //std::substr(), usado para retirar o trecho final do nome do arquivo, para comparar com a extensão que foi solicitada pelo usuario
}

//agora usando bool extension, comparar os arquivos para mostrar somente os com a extensão que foi solicitada:
void file_extension(const node* nodo, const std::string& ext, std::vector<const node*>& resultado){
    if(!nodo) return;
    if(nodo->tipo==nodeType::ARQUIVO && extension(nodo->nome, ext)){
        resultado.push_back(nodo);
    }
    for(const auto& filho : nodo->filhos){
        file_extension(filho, ext, resultado);
    }
}

//mostrar os arquivos com extensao especificada:
void show_by_extension(const node* raiz){
    std::string ext;
    std::cout << "Digite a extensão: (ex.: .pdf)\n";
    std::cin >> ext;
    std::vector<const node*> resultado;
    file_extension(raiz, ext, resultado);
    std::cout << "Arquivos por extensão (" << ext << "):\n";
    for(const auto* arquivo : resultado){
        std::cout << arquivo->caminho << " (" << arquivo->tamanho << " bytes)\n";
    }
}

//3.5 Pastas vazias:
//encontrar a pastas vazias:
void empty(const node* nodo, std::vector<const node*>& resultado){
    if(!nodo || nodo->tipo!=nodeType::DIRETORIO) return;
    if(nodo->filhos.empty()){
        resultado.push_back(nodo);
    }
    for(const auto& filho : nodo->filhos){
        empty(filho, resultado);
    }
}

//mostrar pastas vazias:
void show_empty(const node* raiz){
    std::vector<const node*> resultado;
    empty(raiz, resultado);
    std::cout << "Pastas vazias:\n";
    for(const auto* p : resultado){
        std::cout << p->caminho << "\n";
    }
}

#endif