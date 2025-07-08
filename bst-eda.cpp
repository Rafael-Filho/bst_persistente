#include <iostream>
#include <vector>

	using namespace std;
	struct node; struct mod; 

	enum campos {pai,esq,dir,nada};

	struct node{
		int val;
		node * pai;
		node * esq;
		node * dir;
		vector<mod> mods;	//tam max: 6 (possui 3 ponteiros de retorno, e qtd de mods = 2*p)

		node(): esq(nullptr), dir(nullptr), pai(nullptr), val(0) {}

	};


	struct mod {
		int versao;
		campos campo;
		node* ptr;
		mod(): versao(0), ptr(nullptr){}
	};

	struct BST{
		node * raiz;
		vector<node*> versoes;	//vetor que mantém a raíz de todas as versões da árvore
		int versao;
		BST(): raiz(nullptr),versao(0){}
	};
	//agora enunciando algumas funcoes (elas são codependentes)
	void atualizar_no(BST * arvore, node* x, int versao, mod mod_atual);	//atualizando o nó quando o vetor de mods estoura
	void operacao(BST* arvore, node* x, node* apontado,  campos campo, int versao);	//inserindo os mods no vetor de mods do nó
	void ler_mods(node* x, node *aux, int versao);	//lendo nó x na versao 'versao'

	void copiar (node* a, node * b){	//a = b
		b->esq = a->esq; b->dir = a->dir;
		b->pai = a->pai; b->val = a->val;
	}


	void atualizar_no(BST * arvore, node* x, int versao, mod mod_atual){
		node* no_novo = new node();
		copiar(x, no_novo);
		for (int i = 0 ; i < x->mods.size() ; ++i){ 

			switch (x->mods[i].campo){	//aplicando todos os mods para copiar para o prox estado efemero
				case esq:
					no_novo->esq = x->mods[i].ptr;
					break;
				case dir:
					no_novo->dir = x->mods[i].ptr;
					break;
				case pai:
					no_novo->pai = x->mods[i].ptr;
					break;
			}
		}
		switch (mod_atual.campo){	 //quando os mods do nó estouram, a proxima operacao é guardada no prox estado efemero do no
				case esq:
					no_novo->esq = mod_atual.ptr;
					break;
				case dir:
					no_novo->dir = mod_atual.ptr;
					break;
				case pai:
					no_novo->pai = mod_atual.ptr;
					break;
				default:;
		}
		//agora mexendo nos ponteiros de retorno

		if(no_novo->pai != nullptr){
			node aux_pai;
			ler_mods(no_novo->pai, &aux_pai, versao);
			if (x== aux_pai.esq) operacao(arvore, no_novo->pai, no_novo, esq, versao);
			else if (x== aux_pai.dir)operacao(arvore, no_novo->pai, no_novo, dir, versao);
		}else{
		    arvore->raiz = no_novo;
		}
		
		if (no_novo->esq != nullptr) operacao(arvore, no_novo->esq, no_novo, pai, versao);
		if (no_novo->dir != nullptr) operacao(arvore, no_novo->dir, no_novo, pai, versao);


	}


	void ler_mods(node* x, node *aux, int versao){ //visita a versao 'versao' do campo nó x (copia x para aux e aplica o mod da versao desejada)
		copiar(x,aux);						   
		if (x->mods.empty()) return;
		for (int i = 0 ; i < x->mods.size() ; ++i){
			if (x->mods[i].versao > versao) break;
			
			switch (x->mods[i].campo){
				case esq:
					aux->esq = x->mods[i].ptr;
					break;
				case dir:
					aux->dir = x->mods[i].ptr;
					break;
				case pai:
					aux->pai = x->mods[i].ptr;
					break;
			}
		}
	}

	void operacao(BST* arvore, node* x, node* apontado,  campos campo, int versao){
		mod operar;
		operar.versao = versao;
		operar.campo = campo;
		operar.ptr = apontado;
		if (x->mods.size() < 6)	x->mods.push_back(operar);
		else atualizar_no(arvore, x, versao, operar);
	}

	node * minimo(node* x, int versao){
		if (x == nullptr) return nullptr;

		node aux;
		ler_mods(x, &aux, versao);
		node * p = x;
		
		while (aux.esq != nullptr){
		p = aux.esq;
		ler_mods(p, &aux, versao);
		}
		return p;
	}


	node * maximo(node* x, int versao){		
		node aux;
		ler_mods(x, &aux, versao);
		
		while (aux.dir != nullptr){
		x = aux.dir;
		ler_mods(x, &aux, versao);
		}
		return x;
	}

	node* sucessor(BST *arvore, int k, int versao){	
		if(arvore->versoes.empty() || versao >= arvore->versoes.size()) return nullptr;

		node * p = arvore->versoes[versao];
		if(p==nullptr)	return nullptr;

		node* ant = nullptr;
		node aux;
		while( p != nullptr){
			if(k < p->val){
			    ant = p;
				ler_mods(p,&aux, versao);
				p = aux.esq;
			}else{
				ler_mods(p,&aux, versao);
				p = aux.dir;
			}
		}
		if (ant != nullptr && ant->val > k) return ant;
		else return nullptr;
	}

	node* busca(BST *arvore, int k, int versao){
		if(arvore->versoes.empty() || versao >= arvore->versoes.size()) return nullptr;

		node * p = arvore->versoes[versao];
		if(p==nullptr)	return nullptr;

		node aux;
		while( p != nullptr){
			if(k == p->val){
				return p;
			}
			if(k < p->val){
				ler_mods(p,&aux, versao);
				p = aux.esq;
			}else{
				ler_mods(p,&aux, versao);
				p = aux.dir;
			}
		}return nullptr;
	}


	void inserir(BST * arvore, int k){
		int versao = arvore->versao;
		node* no_novo = new node();
		no_novo->val = k;
		if(arvore->raiz == nullptr){
			arvore->raiz = no_novo;
			}else{
			node * p = arvore->versoes[versao-1];
			node aux;
			node* ant = nullptr;
			while(p != nullptr){
				ant = p;
				if (k == p->val){		//verificacao de caso invalido
				cout << "Insercao invalida: no "<< k << " ja esta na arvore\n";
				return;
				}
				else if(k < p->val){
					ler_mods(p,&aux,versao);
					p = aux.esq;
				}else{
					ler_mods(p,&aux,versao);
					p = aux.dir;
				}
			}
			
			no_novo->pai = ant;
			if(k < ant->val ){
				//q1->esq = no_novo;
				operacao(arvore, ant ,no_novo ,esq ,arvore->versao);
			}else{
				//q1->dir = no_novo;
				operacao(arvore, ant ,no_novo ,dir ,arvore->versao);
			}
		}
		arvore->versoes.push_back(arvore->raiz);
		arvore->versao++;
	}

	void transplantar(BST * arvore, node* antigo, node* novo, int versao){
		//node * p  = arvore->versoes[versao];
		node aux;
		ler_mods(antigo, &aux, versao); //aux = u atualizado
		if (aux.pai == nullptr){
		    arvore->raiz = novo;
		}
		else{
			node aux_pai;
			ler_mods(aux.pai, &aux_pai, versao);
			if (antigo == aux_pai.esq) operacao(arvore, aux.pai, novo, esq, arvore->versao); //antigo->pai->esq = novo;	//se fosse filho esquerdo
			else operacao(arvore, aux.pai, novo, dir, arvore->versao);//antigo->pai->dir = novo;
			//novo->pai = antigo->pai;
		}
		if (novo!=nullptr) operacao(arvore, novo, aux.pai, pai, arvore->versao);
	}

	void remover(BST* arvore, int k){
		int versao = arvore->versao;
		node * x = busca(arvore, k, versao-1);
		if (x == nullptr){
			cout << "Remocao invalida: no " << k << " nao esta na arvore\n";
			return;
		}
		node aux_x;
		ler_mods(x, &aux_x, versao);	//aux é o x na versao chamada
		if(aux_x.esq == nullptr) transplantar(arvore, x, aux_x.dir, versao);
		else if (aux_x.dir == nullptr) transplantar(arvore, x, aux_x.esq,versao);
		else{
			node *y = sucessor(arvore,x->val,versao-1);;
			if(y->mods.size() == 6){	//caso MUITO especifico em que na hora de transplantar o y tem que ser atualizado e se perde pois nao tem nenhum ponteiro pra ele
			    mod dummy;
			    dummy.campo = nada;
			    atualizar_no(arvore, y, versao-1, dummy);
			    y = sucessor(arvore,x->val,versao-1);
			}
			node aux_y;
			ler_mods(y, &aux_y, versao);
			transplantar(arvore, y, aux_y.dir,versao);
			ler_mods(x, &aux_x, versao);
			operacao(arvore, y, aux_x.esq, esq, versao);	//aux_y.esq = aux_x.esq;
			operacao(arvore, y, aux_x.dir, dir, versao); 	//aux_y.dir = aux_x.dir;
			operacao(arvore, aux_x.esq, y, pai, versao);	//aux_x.esq->pai = y;
			if (aux_x.dir != nullptr) operacao(arvore, aux_x.dir, y, pai, versao);
			transplantar(arvore, x, y, versao);
		}
		arvore->versoes.push_back(arvore->raiz);
		arvore->versao++;
	}

	void imprimir(BST * arvore, int versao){
		node* p = arvore->versoes[versao];
		node * aux = minimo(p,versao);
		while(aux!=nullptr){
			cout<<"["<<aux->val<<"] ";
			aux = sucessor(arvore, aux->val,versao);
		}
		cout<<"\n";
	}