#include <iostream>
#include <vector>
#include <fstream> 
#include <sstream>
#include <string>
#include "bst-eda.cpp"

using namespace std;
enum instrucoes{INC, REM, SUC, IMP};


int main() {
    BST* arvore = new BST();
    
    ifstream arquivo("comandos.txt");
    string instrucao;

    while(getline(arquivo, instrucao)){ //lendo linha por linha
        string tipo;
        int num;
        int vers= -1;
        istringstream iss(instrucao);   //vai pegando palavras separadas por espaço " "
        iss >> tipo;

        if (tipo == "INC"){
            iss >> num;
            inserir(arvore, num);
        }
        else if (tipo == "REM") {
            iss >> num;
            remover(arvore, num);
        }
        else if (tipo == "SUC"){
            iss >>num;
            iss >> vers;
            if (vers < 0 || vers > arvore->versao) cout<<"ERRO: versao do sucessor "<< vers << " invalida\n";
            else{
                cout << "SUC " << num << " " << vers;
                node * x = sucessor(arvore, num, vers);
                if (x == nullptr) cout << "\nnao existe sucessor do numero "<< num <<" na versao " << vers <<"\n";
                else cout<< "\n" << x->val << "\n";
            }
        }
        else if (tipo == "IMP"){
            iss >> vers;
            if (vers < 0 || vers > arvore->versao-1) cout<<"ERRO: versao de impressao "<< vers << " invalida\n";
            else{ 
                imprimir(arvore,vers);
                cout<<"\n";
            }
        }else{
            cout<<"ERRO: instrucao < "<< tipo << " > invalida\n";
        }
    }

    return 0;
}