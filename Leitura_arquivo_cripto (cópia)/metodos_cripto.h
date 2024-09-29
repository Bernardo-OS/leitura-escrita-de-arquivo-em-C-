#include <iostream>
#include <fstream>
#include <string>
using namespace std;

typedef struct{
	string nome;
	string sigla;
    string valor;
} Crypto;

void split(string vetor[], string str, string deli = " "){        
    int start = 0;
    int end = str.find(deli);
    int i = 0;
    while (end != -1) {
        vetor[i] = str.substr(start, end - start);
        i++;
        start = end + deli.size();
        end = str.find(deli, start);
    }
    vetor[i] = str.substr(start, end - start);
}

int conectarComBase(string nomeArquivo, Crypto lista[], int n){
	ifstream procuradorLeitura;
	int indice = 0;
	//abrir e associar o arquivo com seu procurador - formato leitura
	//string linha;
	procuradorLeitura.open(nomeArquivo);
	if(!procuradorLeitura)return indice;
	string linha;
	
	while(!procuradorLeitura.eof() && indice < n){
		//para cada linha percorrida do procurador, splitar por ";"
		getline(procuradorLeitura,linha);
		if(linha != ""){
			string vetorLinha[3];
			split(vetorLinha, linha, ";");
			
            lista[indice].nome = vetorLinha[0];
            lista[indice].sigla = vetorLinha[1];
			lista[indice].valor = vetorLinha[2];
			indice++;
		}
	}
	procuradorLeitura.close();
	// cout << "total registros.... " << indice << endl;
	return indice;
}
						   
void exibirLista(Crypto lista[], int n){
	for (int i = 0; i < n; i++){
		cout << "Crypto: " << lista[i].nome << " | Sigla: " << lista[i].sigla << " | valor: " << lista[i].valor << endl;
		//cout << lista[i].sigla;
	}
}

// void gravarArquivo(string nomeArquivo, string nome, string sigla, string valor){
// 	ofstream procuradorEscrita;
// 	procuradorEscrita.open(nomeArquivo, ios::out | ios::app);
// 	procuradorEscrita << nome << ";" << sigla << ";" << valor << endl;
// 	procuradorEscrita.close();
// }

void gravarArquivo(const string& filename, const string& nome, const string& sigla, string valor) {
    ofstream file(filename, ios::app); // Abre o arquivo em modo append
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo " << filename << endl;
        return;
    }
    file << nome << ";" << sigla << ";" << valor << endl;
    file.close();
}

//O método de remover dados em c++ é feitopegando o dado a ser removido;
//criando um arquivo temporário que guarda todos os dados copiado do arquivo original menos o correspondente ao removido;
//Depois renomeia o temporário para o nome original e remove o arquivo original
void removerArquivo(string nomeArquivo, string nome){
	ifstream procuradorLeitura;
	ofstream procuradorEscrita;
	procuradorLeitura.open(nomeArquivo);
	procuradorEscrita.open("temp.csv");
	//abertura de arquivos, com entrada e saída de valores
	string linha;
	while(!procuradorLeitura.eof()){
		getline(procuradorLeitura,linha);
		if(linha != ""){
			string vetorLinha[3];
			split(vetorLinha, linha, ";");
			if(vetorLinha[0] != nome){
				procuradorEscrita << vetorLinha[0] << ";" << vetorLinha[1] << ";" << vetorLinha[2] << endl;
			}
		}
	}
	//Lê cada linha até o final do arquivo; divide as linhas não vazias por ";"
	//Se o nome da criptomoeda for diferente do nome a ser removido, escreve no arquivo temporário "temp.csv"
	procuradorLeitura.close();
	procuradorEscrita.close();
	//Fecha os arquivos
	remove(nomeArquivo.c_str());
	rename("temp.csv", nomeArquivo.c_str());
	//Remove o arquivo original e renomeia o arquivo temporário para o nome original
}

int menu(Crypto lista[], int totalCadastrados, int n){
	int opcao;
	do {
		system("clear");
		//system("cls");
		cout << "MENU\n";
		cout << "1 - Listar criptomoedas\n";
		cout << "2 - Gravar criptomoeda\n";
        cout << "3 - Remover criptomoeda\n";
		cout << "4 - Sair\n";
		cin >> opcao;
		fflush(stdin);
	
		switch (opcao){
			case 1:
				if(totalCadastrados > 0){
					cout << "Listar criptomoedas...\n";
					exibirLista(lista,totalCadastrados);
				}else{
					cout << "lista vazia\n";
				}
				break;
			case 2:
            //Controle de duplicidade
				if(totalCadastrados < TAM){
                    bool ja_registrado = false;
					cout << "Cadastrar criptomoedas...\n";
					string nome;
                    string sigla;
                    string valor;
                    cout << "Nome da criptomoeda: ";
					cin >> nome;
					fflush(stdin);

					ifstream procuradorLeitura;
					//ofstream procuradorEscrita;
					procuradorLeitura.open("crypto.csv");
					//procuradorEscrita.open("temp.csv");
                    ifstream file("crypto.csv");
					string linha;
					while(!procuradorLeitura.eof()){
						getline(procuradorLeitura,linha);
						if(linha != ""){
							string vetorLinha[3];
							split(vetorLinha, linha, ";");
							if(vetorLinha[0] == nome){
								ja_registrado = true;
								break;
							}
						}
					}
        			file.close();

        		if (!ja_registrado) {
            		totalCadastrados++;
            		cout << "Sigla da criptomoeda: ";
            		cin >> sigla;
					fflush(stdin);
            		cout << "Valor da criptomoeda: ";
            		cin >> valor;
					fflush(stdin);
            		lista[totalCadastrados].nome = nome;
            		lista[totalCadastrados].sigla = sigla;
            		lista[totalCadastrados].valor = valor;
            		gravarArquivo("crypto.csv", nome, sigla, valor);
					conectarComBase("crypto.csv", lista, TAM);
        		}else{
					cout << "Criptomoeda já cadastrada";
                }
            	}else{
					cout << "Lista lotada\n";
				}
				break;				
			case 3:
				if(totalCadastrados > 0){
					cout << "Remover criptomoeda...\n";
					string nome;
					cout << "Nome da criptomoeda: ";
					cin >> nome;
					removerArquivo("crypto.csv", nome);
					totalCadastrados--;
				}else{
					cout << "Lista vazia\n";
				}
				break;
            case 4:
				break;
			default:
				cout << "Opcao invalida\n";
		}
		//system("pause");
		cout << "Pressione Enter para continuar...";
        cin.ignore();
        cin.get();
	} while(opcao != 4);
	return totalCadastrados;
}


