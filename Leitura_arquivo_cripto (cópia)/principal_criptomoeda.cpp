#include <iostream>
#include <fstream>
#include <string>

#define TAM 100
using namespace std;
#include "metodos_cripto.h"

int main(){
	//conectar com a base
	Crypto lista[TAM];
	int totalCadastrados = 0;
	totalCadastrados = conectarComBase("crypto.csv", lista, TAM);
	totalCadastrados = menu(lista, totalCadastrados, TAM);
	return 1;
}