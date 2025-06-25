# Sistema de Arquivos em Árvore

## Descrição

O programa transforme em uma árvore um determinado caminho de arquivos (arquivos regulares e diretórios) e oferece as opções de exibição, exportação e pesquisa por meio de um menu interativo.

## Compilação

```bash
# GCC/Clang em GNU‑Linux
g++ arvore.cpp -o arvore
```


## Execução

```bash
# 1) Usar diretório atual
./arvore

# 2) Informar diretório inicial (exemplo)
./arvore /home/$USER/Documents
```

## Menu principal

```
1 – Exibir a árvore completa
2 – Exportar a árvore para HTML
3 – Pesquisas
0 – SAIR
```

### 1  Exibir árvore

Mostra no terminal a estrutura hierárquica:

```
/home/guilhermecpp/Documentos/ (4 filhos, 356850 bytes
├── Comando.txt (2737 bytes)
├── Capitulo.pdf (147398 bytes)
├── vazia (0 filhos, 0 bytes)
└── tde (5 filhos, 206715 bytes)
    ├── tde199442.zip (1450 bytes)
    ├── p01 (99272 bytes)
    ├── tde2205 (2 filhos, 2746 bytes)
    │   ├── digraph.cpp (1486 bytes)
    │   └── tde199442.cpp (1260 bytes)
    ├── tde (102848 bytes)
    └── p01.cpp (399 bytes)
```

### 2  Exportar para HTML

Cria `arvore.html` no diretório do executável (usa `<pre>` para manter indentação) e abre automaticamente com `xdg-open`.

### 3  Pesquisas (sub‑menu)

```
1 – Maior arquivo
2 – Arquivos > N bytes
3 – Pasta com mais arquivos diretos
4 – Arquivos por extensão
5 – Pastas vazias
0 – Voltar
```

| Opção | Saída exemplo                                                                         |
| ----- | --------------------------------------------------------------------------------------|
| 1     | `/home/guilhermecpp/Documentos/Capitulo.pdf (147398 bytes)`                           |
| 2     | `Arquivos > 120000 bytes /home/guilhermecpp/Documentos/Capitulo.pdf (147398 bytes)`   |
| 3     | `/home/guilhermecpp/Documentos/tde (5 filhos, 206715 bytes)`                          |
| 4     | `/home/guilhermecpp/Documentos/Comando.txt (2737 bytes)`                              |
| 5     | `/home/guilhermecpp/Documentos/vazia`                                                 |

## Arquitetura do código

- **main.cpp** → menu interativo, entrada de usuário.
- **tree.hpp** → estrutura `node`, construção da árvore, exibição, exportação HTML e funções de pesquisa.

## Observações

- Apenas arquivos regulares e diretórios são considerados (links simbólicos e arquivos especiais são ignorados).
- O programa usa `size_t` para suportar arquivos grandes (>4 GB em arquiteturas 64 bit).
- Testado no Ubuntu 22.04, GCC 13.3.0.