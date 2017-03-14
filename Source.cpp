/*
Nombre: practica 2
Autores: Cristhian Rodriguez Gomez y Luis Mathioux Abad
Fecha: 22/01/2017 19:45
Fecha límite de entrega: 22 de enero de 2017.
Descripcion:
*/

#include <iostream>
#include <string>
#include <fstream>
#include "PriorityQueue.h" // std::priority_queue
#include "TreeMap_AVL.h" 
#include <vector>         // std::vector
#include <functional>     // std::greate
#include <vector>
#include <math.h>
#include <iomanip> 
#include <windows.h> 
#include <cstdlib> 
#include <ctime> 

// DELCARACION DE METODOS Y FUNCIONES
using namespace std;

const int MAX = 6;
const char NOEXITE = '-';
const char CASILLA_INVALIDA = 'X'; //X x
const char CASILLA_INICIO = 'O'; // O o
const char CASILLA_FIN = '=';
const char CASILLA_VACIA = ' ';
const char CASILLA_VISITADA = 'v';
const char CASILLA_CERRADA = 'a';

typedef struct {
	int f;
	int c;
	double distanciaDesdeInicio;
	double distanciaADestino;
	double distanciaTotal;
	char simbolo;
}tCasilla;

bool operator <(tCasilla a, tCasilla b) {
	return a.distanciaTotal < b.distanciaTotal;
}
typedef int tLista[MAX];
typedef tCasilla tMatriz[MAX][MAX];

typedef struct {
	int nFilas;
	int nColumnas;

	tMatriz matriz;
	PriorityQueue <tCasilla>  abierta;
	TreeMap <int, tCasilla > cerrada;
	tCasilla inicio;
	tCasilla fin;
	tCasilla actual;
}tAlgoritmo;

typedef enum {   // screen colors 
	black,                 // 0 
	dark_blue,             // 1 
	dark_green,            // 2 
	dark_cyan,             // 3 
	dark_red,              // 4 
	dark_magenta,          // 5 
	dark_yellow,           // 6 
	light_gray,            // 7 
	dark_gray,             // 8 
	light_blue,            // 9 
	light_green,           // 10 
	light_cyan,            // 11 
	light_red,             // 12 
	light_magenta,         // 13 
	light_yellow,          // 14 
	white                  // 15 
}tColor;
/*
	Metodos para la salida por pantalla
*/
void mostrarMatriz(const tAlgoritmo &a);
void mostrarCasillasDireccionesPosibles(std::vector<tCasilla> &direcciones, tAlgoritmo & a);
void mostrarCasilla(const tCasilla &casilla);
void dibujarTablero(const tAlgoritmo &a);
void setColor(tColor &color);
void pintarCasilla(const tCasilla & casilla);
/*
	Metodos para el algoritmo
*/
void inicializarAlgoritmo(tAlgoritmo &a);
void inicializarMatriz(tAlgoritmo &a);
void modificarMatriz(tMatriz &m, tCasilla c);
void modificarCasilla(tCasilla &casilla, int f, int c, char simbolo);
bool posValida(int pos, int tam);
tCasilla irSiguiente(tAlgoritmo &a, int f, int c);
void ejecutarAlgoritmo(tAlgoritmo &a);
void recorremosDireccionesEInsertamosEnListaAbierta(std::vector<tCasilla> &direcciones, tAlgoritmo & a);
void calculamosDirecciones(tAlgoritmo & a, std::vector<tCasilla> &direcciones);
bool containsListaCerrada(tAlgoritmo & a, tCasilla &var);
tCasilla getMinListaAbierta(tAlgoritmo & a);
void popListaAbierta(tAlgoritmo & a);
void insertarEnListaCerrada(tCasilla &min, tAlgoritmo & a);
double calcularDistancia(tCasilla &o, tCasilla &d);

// Cargamos el trablero
bool cargarAlgoritmo(tAlgoritmo & a);
void cargarTablero(ifstream & fichero, tAlgoritmo & a);
int main() {

	tAlgoritmo a;
	


	// Inicializamos los datos del algoritmo
	//inicializarAlgoritmo(a);
	cargarAlgoritmo(a);

	// Dibujamos el tablero
	dibujarTablero(a);
	
	// Ejecutamos el algoritmo
	// ejecutarAlgoritmo(a);

	system("PAUSE");
	return 0;
}

void inicializarAlgoritmo(tAlgoritmo &a) {
	tCasilla c;

	// Inicializar tamaño de filas y columnas
	a.nColumnas = MAX;
	a.nFilas = MAX;

	// Inicializamos la matriz
	inicializarMatriz(a);

	// Casillas Inaccesibles
	modificarCasilla(c, 1, 2, CASILLA_INVALIDA);
	modificarMatriz(a.matriz, c);
	modificarCasilla(c, 3, 2, CASILLA_INVALIDA);
	modificarMatriz(a.matriz, c);
	modificarCasilla(c, 4, 2, CASILLA_INVALIDA);
	modificarMatriz(a.matriz, c);
	modificarCasilla(c, 2, 2, CASILLA_INVALIDA);
	modificarMatriz(a.matriz, c);
	modificarCasilla(c, 5, 1, CASILLA_INVALIDA);
	modificarMatriz(a.matriz, c);

	// Casillas de inicio 
	modificarCasilla(c, 1, 1, CASILLA_INICIO);
	modificarMatriz(a.matriz, c);
	
	//a.abierta.push(c);
	insertarEnListaCerrada(c, a);

	a.actual = c;
	a.inicio = c;

	// Casilla fin
	modificarCasilla(c, 2, 5, CASILLA_FIN);
	modificarMatriz(a.matriz, c);

	a.fin = c;

}
double calcularDistancia(tCasilla &o, tCasilla &d) {

	int baseC = abs(d.c - o.c);
	int baseF = abs(d.f - o.f);

	double resultadoC = pow(baseC, 2);
	double resultadoF = pow(baseF, 2);

	double suma = resultadoC + resultadoF;
	double distancia = sqrt(suma);
	return distancia;
}
bool posValida(int pos, int tam) {
	return (pos >= 1 && pos < tam);
}
void ejecutarAlgoritmo(tAlgoritmo &a) {
	cout << "<---- Actual: " << a.actual.f << a.actual.c << endl;
	
	vector<tCasilla> direcciones;

	calculamosDirecciones(a, direcciones);
	//dibujarTablero(a.matriz);

	recorremosDireccionesEInsertamosEnListaAbierta(direcciones, a);

	// Me quedo con el minimo
	tCasilla min = getMinListaAbierta(a);
	mostrarCasilla(min);

	// Guardo en la lista cerrada
	insertarEnListaCerrada(min, a);

	// Modificamos el valor actual
	a.actual = min;

	//mostrarCasillasDireccionesPosibles(direcciones, a);

	// Si Hay elementos en la lista abierta o la Distancia a destino es Cero :Salimos
	if (a.abierta.empty() || a.actual.distanciaADestino == 0) {
		exit;
	}
	else {
		popListaAbierta(a);
		ejecutarAlgoritmo(a);
	}


}
void mostrarCasillasDireccionesPosibles(std::vector<tCasilla> &direcciones, tAlgoritmo & a)
{
	for each (tCasilla var in direcciones)
	{
		if (var.simbolo != NOEXITE) {
			mostrarCasilla(a.matriz[var.f][var.c]);
		}
	}
}
void recorremosDireccionesEInsertamosEnListaAbierta(std::vector<tCasilla> &direcciones, tAlgoritmo & a)
{
	for each (tCasilla casilla in direcciones)
	{
		if (casilla.simbolo != NOEXITE ) {
			
			double dInicio = calcularDistancia(a.actual, casilla);
			double dDestino = calcularDistancia(casilla, a.fin);

			a.matriz[casilla.f][casilla.c].distanciaDesdeInicio = dInicio;
			a.matriz[casilla.f][casilla.c].distanciaADestino = dDestino;
			a.matriz[casilla.f][casilla.c].distanciaTotal = dInicio + dDestino;


			if (!containsListaCerrada(a, a.matriz[casilla.f][casilla.c])) {
				
				a.abierta.push(a.matriz[casilla.f][casilla.c]);

				if (a.matriz[casilla.f][casilla.c].simbolo != CASILLA_FIN)
					a.matriz[casilla.f][casilla.c].simbolo = CASILLA_VISITADA;

				// Actualizamos el tablero
				dibujarTablero(a);
				
			}
		}
	}
}
void calculamosDirecciones(tAlgoritmo & a, std::vector<tCasilla> &direcciones)
{
	tCasilla n, o, s, e, ne, no, se, so;

	n = irSiguiente(a, -1, 0);
	s = irSiguiente(a, +1, 0);
	o = irSiguiente(a, 0, -1);
	e = irSiguiente(a, 0, +1);
	no = irSiguiente(a, -1, -1);
	so = irSiguiente(a, +1, -1);
	ne = irSiguiente(a, -1, +1);
	se = irSiguiente(a, +1, +1);

	direcciones.push_back(n);
	direcciones.push_back(s);
	direcciones.push_back(o);
	direcciones.push_back(e);
	direcciones.push_back(no);
	direcciones.push_back(so);
	direcciones.push_back(ne);
	direcciones.push_back(se);
}
bool containsListaCerrada(tAlgoritmo & a, tCasilla &var)
{
	return a.cerrada.contains(var.f * 10 + var.c);
}
tCasilla getMinListaAbierta(tAlgoritmo & a)
{
	tCasilla c = a.abierta.top();

	return c;
}
void popListaAbierta(tAlgoritmo & a)
{
	a.abierta.pop();
}
void insertarEnListaCerrada(tCasilla &min, tAlgoritmo & a)
{	
	int clave = min.f * 10 + min.c;
	a.cerrada.insert(clave, min);

	if (min.simbolo != CASILLA_INICIO)
		min.simbolo = CASILLA_CERRADA;

	modificarMatriz(a.matriz, min);
}
tCasilla irSiguiente(tAlgoritmo &a, int f, int c) {
	tCasilla siguienteC;
	int f1, c1;

	f1 = a.actual.f + f;
	c1 = a.actual.c + c;

	if (posValida(f1, a.nFilas) && posValida(c1, a.nColumnas)) {
		if (a.matriz[f1][c1].simbolo != CASILLA_INVALIDA) {
			siguienteC = a.matriz[f1][c1];
			siguienteC.simbolo = CASILLA_VISITADA;
		}
		else
			siguienteC.simbolo = NOEXITE;
	}
	else {
		siguienteC.simbolo = NOEXITE;
	}
	return siguienteC;
}
void inicializarMatriz(tAlgoritmo &a) {
	tCasilla c;
	c.distanciaTotal = -1;
	c.distanciaADestino = -1;
	c.distanciaDesdeInicio = -1;

	for (int i = a.nFilas - 1; i >= 1; i--)
	{
		for (int j = a.nColumnas - 1; j >= 1; j--)
		{
			c.c = j;
			c.f = i;

			c.simbolo = CASILLA_VACIA;
			a.matriz[i][j] = c;
		}
	}
}
void mostrarMatriz(const tAlgoritmo &a) {

	for (int i = a.nFilas - 1; i >= 1; i--)
	{
		for (int j = 1; j < a.nColumnas; j++)
		{
			if (j == 1)cout << i << "|";
			cout << a.matriz[i][j].simbolo;
		}
		cout << endl;
	}

	for (int i = 1; i < a.nColumnas; i++) {

		if (i == 1)cout << "  ";
		cout << "-";
	}
	cout << endl;

	for (int i = 1; i < a.nColumnas; i++) {
		if (i == 1)cout << "  ";
		cout << i;
	}
	cout << endl;
}
void modificarMatriz(tMatriz &m, tCasilla c) {
	m[c.f][c.c] = c;
}
void modificarCasilla(tCasilla &casilla, int f, int c, char simbolo) {
	casilla.f = f;
	casilla.c = c;
	casilla.simbolo = simbolo;
}
void mostrarCasilla(const tCasilla &casilla) {
	cout << casilla.f << casilla.c << " " << casilla.simbolo << endl;
	//cout << " DTotal:" << casilla.distanciaTotal << " destino:" << casilla.distanciaADestino << endl;
}
void dibujarTablero(const tAlgoritmo &a)
{
	Sleep(400);
	system("cls");
	
	int N = MAX;
	const char supIzq = char(218);
	const char horizontal = char(196);
	const char supCruce = char(194);
	const char supDer = char(191);
	const char vertical = char(179);
	const char verIzqCruce = char(195);
	const char cruz = char(197);
	const char verDerCruce = char(180);
	const char infIzq = char(192);
	const char infCruce = char(193);
	const char infDer = char(217);
	const char pieza = char(219);
	tColor blanco;
	blanco = white;


	// PARTE SUPERIOR
	setColor(blanco);
	cout << setw(4) << supIzq;
	for (int i = 1; i < a.nColumnas - 1; i++) {
		cout << horizontal  << horizontal << supCruce;
	}
	cout << horizontal  << horizontal << supDer << endl;

	// PARTE DEL MEDIO
	for (int i = a.nFilas -1; i >= 1; i--) {
		
		cout << setw(2) << i ;
		cout << setw(2) << vertical;
		for (int j = 1; j < a.nColumnas; j++) {
			// CASILLA
			pintarCasilla(a.matriz[i][j]);
			cout << vertical;   // pieza
		}
		cout << endl;

		// VERTICAL CRUCE 
		if (i > 1) {
			cout << setw(4) << verIzqCruce;

			for (int i = 1; i < a.nColumnas - 1; i++) {
				cout << horizontal  << horizontal << cruz;
			}
			cout << horizontal  << horizontal << verDerCruce << endl;
		}
	}
	// PARTE INFERIOR
	cout << setw(4) << infIzq;
	for (int i = 1; i < a.nColumnas - 1; i++) {
		cout << horizontal  << horizontal << infCruce;
	}
	cout << horizontal  << horizontal << infDer << endl;

	cout << char(32) << char(32);
	for (int i = 1; i < a.nColumnas; i++) {
		cout << setw(4) << i;
	}
	cout << endl;
	
}
void setColor(tColor &color) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, color);
}
void pintarCasilla(const tCasilla &casilla) {
	tColor color, blanco;

	blanco = white;
	if (casilla.simbolo == CASILLA_INICIO) {
		color = light_green;
	}
	else if (casilla.simbolo == CASILLA_FIN)
	{
		color = light_blue;
	}
	else if (casilla.simbolo == CASILLA_INVALIDA)
	{
		color = light_red;
	}
	else if (casilla.simbolo == CASILLA_VISITADA)
	{
		color = light_yellow;
	}
	else if (casilla.simbolo == CASILLA_CERRADA)
	{
		color = black;
	}
	else {
		color = white;
	}
	setColor(color);
	// pintamos la ficha
	cout << char(219) << char(219);
	//volvemos a pintar en negro
	setColor(blanco);

}
bool cargarAlgoritmo(tAlgoritmo & a) {
	ifstream fichero_entrada;
	string nomFichEntrada = "mapa.txt";
	int nivel;
	bool exito = false;

	// ### pedirString(nomFichEntrada, "Introduce el nombre del fichero (ej: nombre.txt):");

	// Abrimos el fichero
	fichero_entrada.open(nomFichEntrada);

	if (!fichero_entrada.is_open()) {
		cout << "ERROR: no se pudo abrir el archivo : " << nomFichEntrada << endl;
	}
	else {

		// Cargamos el tablero
	
		cargarTablero(fichero_entrada, a);

		// Cerramos el fichero
		fichero_entrada.close();
	}

	return exito;
}
void cargarTablero(ifstream & fichero, tAlgoritmo & a)
{
	tCasilla casilla;
	string linea;
	int f = 0, c = 0;
	int colMax = 0;

	getline(fichero, linea);// primera linea del tablero

	while (linea.length() != 0) {

		for (c = 0; c < linea.length(); c++)
		{
			tCasilla casilla;
			
			// Guardamos la casilla
		
			a.matriz[f][c].simbolo = linea[c];
			a.matriz[f][c].f = f;
			a.matriz[f][c].c = c;

			pintarCasilla(a.matriz[f][c]);
			// Si es la casilla de inicio,
			if (linea[c] == CASILLA_INICIO){
				a.inicio = a.matriz[f][c];
			}
				
			// Si es mayor que la maxima columna, lo guardamos
			if (c >= colMax)
				colMax = c;

		}
		cout << endl;
		f++;
		getline(fichero, linea); //siguiente linea
	}

	// Guardamos la fila y columna
	a.nFilas = f;
	a.nColumnas = colMax + 1;
}