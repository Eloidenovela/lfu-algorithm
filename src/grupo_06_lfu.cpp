/*
	Universidade Zambeze
	Faculdade de Ciencias e Tecnologia
	Curso de Licenciatura em Engenharia Informatica- Laboral
	Arquitetura de Computadores - 2023
	
	6.o grupo <---> Algoritmo LFU {
		-> Eliane Senda
		-> Eloide Novela
		-> Lourenco Madouro
	}
*/

#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

// === 
// definimos algumas macros para deixar o codigo um pouco intuitivo.
#define system_clear system("cls")
#define system_pause system("pause")
// ===

struct Pagina {
    unsigned contador = 0u;
    int id = 0;
    // Mais conteudo...
};

enum Estado { PRESENTE_NA_MV, NOVA };

bool buscar_pagina(vector<Pagina> memoria, int id, unsigned & indece) {
	
	for (auto j = 0u ; j < memoria.size() ; j++) {
		if (id == memoria[j].id) {
			indece = j;
			return true;
		}
	}
	
	return false;
}

bool trocar(vector<Pagina> & memoria_fisica, bool capacidade, struct Pagina & nova_pagina) {
    
	auto min_acess = memoria_fisica[0];
    vector<Pagina>::iterator it = memoria_fisica.begin();
    int pos = 0;
    unsigned sz = memoria_fisica.size();
    if (!capacidade) {
    	
    	// Caso nao haja espaco na memorio, o LFU deve procurar a pagina menos referenciada.
	    // Nesta implementacao, a busca e linear, por isso a complexidade de tempo e O(n).
		// -> onde o melhor caso da busca e O(1) e o pior caso e O(n).
		for (auto j = 0u ; j < memoria_fisica.size() ; j++) {
			
			if (min_acess.contador >= memoria_fisica[j].contador) {
				
				min_acess = memoria_fisica[j];
				pos = j;
			}
		}
		
		it = memoria_fisica.begin() + pos;
	    	
		memoria_fisica.erase(it);
	    memoria_fisica.insert(it, nova_pagina); // Add na posicao da pagina menos acessada.
	    min_acess.contador = 0; // A pagina menos acessada e zerada para ser alocada na memoria virtual.
		nova_pagina = min_acess; // A pagina menos acessada e guardada ja na memoria virtual.
	
	} else {
		// Caso haja espaco na memoria, o sistema simplemente adiciona a nova pagina, no final da lista.
		memoria_fisica.push_back(nova_pagina);
	}	
	
	if (capacidade && (memoria_fisica.size() == sz + 1)) {
		// Se houve espaco na memoria, o tamanho da lista deve ser incrementado.
		
		return true;
	} else if (!capacidade && (memoria_fisica.size() == sz)) {
		// Se nao houve espaco na memoria o tamanho da lista nao deve ser incrementado.
		
		return true;
	}
	
	return false;
}

string adicionar(vector<Pagina> & memoria_fisica, vector<Pagina> & memoria_virtual, const int MAX) {
    
	int id = int();
    unsigned indece = 0;
    bool capacidade = bool();
	Pagina nova_pagina = Pagina();
	Estado estado_pagina = NOVA;
    
	cout << "Digite o novo id: ";
    cin >> id;
    
    nova_pagina.id = id;
    
    {
    	// Esta seccao faz uma busca nas duas memorias
	    if (buscar_pagina(memoria_fisica, id, indece)) {
	    	// Caso a pagina esteja na memoria fisica, o LFU nao e invocado. Simplesmente e incrementado o contador da pagina.
	    	memoria_fisica[indece].contador += 1;
	    	
			return "Sucesso: pagina ja existente!";	
				
		} else if (buscar_pagina(memoria_virtual, id, indece)) {
			// Este e o caso em que a pagina requistada pelo sistema operativo esteja na memoria virtual.
			
			estado_pagina = PRESENTE_NA_MV;
		}	
	}
	
	{
		// Estas condicoes verificam o estado da memoria, isto e, se ha espaco ou nao.
		if ((int) memoria_fisica.size() >= MAX) {
			
			capacidade = false; // No caso em que a memoria nao tem capacidade para adicionar uma nova pagina, recorrendo assim ao LFU.
		 
		} else {
		
			capacidade = true; // No caso em que a memoria fisica tem capacidade para adicionar mais paginas, o sistema nao invoca o LFU.
		}		  				
	}

	switch (estado_pagina) {
		case PRESENTE_NA_MV: {
            // Caso em que a pagina esteje na memoria virtual.
            if (trocar(memoria_fisica, capacidade, memoria_virtual[indece])) {
			
				return "Sucesso: pagina adicionada"; 
			
			} else {
				
				return "Erro: a pagina existe na memoria_virtual, mas nao consegui  adiciona-la na memoria_virtual!";
			}
							
			break;
		}
		case NOVA: {
			
			if (trocar(memoria_fisica, capacidade, nova_pagina)) {
				if (!capacidade)
					memoria_virtual.push_back(nova_pagina);
				
				return "Sucesso: pagina nova adicionada!";
			
			} else {
			
				return "Erro: pagina nova nao adicionada!";
			}
			
			break;
		}
	}
    	
    return "Falha: erro desconhecido!";
}

void mostrar_status(vector<Pagina> memoria) {
	cout << "ID\tFrequencia\n";
	for (auto & pagina : memoria) {
		cout << pagina.id << "\t" << pagina.contador << endl;
	}
}

bool acessar(vector<Pagina> & memoria) {
    int id = int();
	unsigned indece = 0;
    cout << "Referenciar pelo id da pagina: ";
    cin >> id;

	if (buscar_pagina(memoria, id, indece)) {
		memoria[indece].contador += 1;
		return true;
	} 

    return false;
}

const int MAX_PAGES = 4;

int main( ) {
	
    vector<Pagina> memoria_fisica = {{0, 1}, {0, 2}, {0, 3}};
    vector<Pagina> memoria_virtual = {{0, 5}, {0, 4}, {0, 7}, {0, 22}, {0, 99}};
    short entrada;
    
    while (true) {
   	   system_clear;
        cout << "1. Referenciar\n"
             << "2. Adicionar\n"
             << "3. Mostrar Status\n"
             << "0. Sair\n"
			 << "Entrada: ";
        cin >>  entrada;
        switch (entrada) {
	        case 1:
	            system_clear;
                mostrar_status(memoria_fisica);
	            if (acessar(memoria_fisica)) {
                    cout << "Sucesso: Pagina acessada\n";
                } else {
                    cout << "Erro: Pagina nao acessada\n";
                }

	            system_pause;
	            break;
	        case 2:
	            system_clear;	        	
	        	cout << adicionar(memoria_fisica, memoria_virtual, MAX_PAGES) << endl;
	        	system_pause;
				break;
	        case 3:
	    		system_clear;
	        	cout << "1. Memoria Fisica\n";
	        	cout << "2. Memoria Virtual\n";
	        	cout << "Entrada: ";
	        	cin >> entrada;
	        	switch (entrada) {
					case 1:
						system_clear;						
						cout << "Status da Memoria Fisica\n";
						mostrar_status(memoria_fisica);
						system_pause;
						break;
					case 2:
						system_clear;						
						cout << "Status da Memoria Virtual\n";
						mostrar_status(memoria_virtual);
						system_pause;
						break;
				}
  	            break; 
   			case 0:
   				exit(0);
        }
    }

    return 0;
}
