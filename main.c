/*
 * Nombre del programa:
 * Fecha: mar 03 sep 2024 17:11:23
 * Descripcion del programa:
 *
 * Nombres de los programadores:
 *   - Alan Octavio Ramirez Hernandez
 *   - Raquel Estefania Quevedo Rodriguez
 *   - Eduardo Alejandro Enriquez Velazquez
 *   - Lilia Sofia Betancourt Haro
 *   - Alexis Gabriel Centeno Uribe
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

// Solamente funcionara con matrices de 2x2.
float determinante(int n, int matriz[][n])
{
	int determinante;
	int a, b, c, d;

	a = matriz[0][0]; 
	b = matriz[1][1];
	c = matriz[0][1];
	d = matriz[1][0];

	// La diagonal principal por la diagonal secundaria.
	determinante = a*d - b*c;

	return determinante;
}

void llenarMatrizAleatorios(int n, int matriz[][n], int min, int max)
{
	// Impide que min sea mayor que max.
	if (min > max) return;

	int r;

	srand(time(NULL));
	for (int i=0; i < n; i++) {
		for (int j=0; j < n; j++) {
			/* Usar rand() no es buena idea, hablando en el
			 * contexto de la criptografia. De todos modos, esto es
			 * solo una demostracion.
			 */
			r = rand() % (max + 1 - min) + min;
			matriz[i][j] = r;
		}
	}

}

void crearMatrizLlave(int n, int matriz[][n])
{
	float det = 0.0;

	while (det == 0) {
		// 0 es A, y 25 es Z.
		llenarMatrizAleatorios(n, matriz, 0, 25);
		det = determinante(n, matriz);
	}
}

// Función para multiplicar un vector por la matriz. 
void vectorPorMatriz(int n, int vector[n], int matriz[][n], int resultado [n])
{
	for (int i=0; i < n; i++) {
                for (int j=0; j < n; j++) {
			resultado[i] += (vector[j] * matriz[i][j]);
		}
		// Tenemos 26 simbolos, de ahi el 26, A-Z.
		resultado[i] = resultado[i] % 26;
	}
}

// Función para mostrar la matriz 2x2.
void mostrarMatriz(int n, int matriz[][n])
{
	for (int i=0; i < n; i++) {
		for (int j =0; j < n; j++) {
			printf("\t%d", matriz[i][j]);
		}
                printf("\n");
	}
}

// Función para mostrar un vector.
void mostrarVector(int n, int vector[n])
{
	for (int i=0; i < n; i++) {
            	printf("%d\n",vector[i]);
	}
        printf("\n");
}

// Separa las palabras en pares de caracteres.
void separarPalabrasPar(int n, int vecMsgTmp[n], int *indice, char *palabra)
{
	for (int i=0; palabra[i] != '\0'; i+=2) {
		vecMsgTmp[i] = 
		printf("%c%c ", palabra[i], palabra[i+1]);
	}
	printf("\n");
}

void invertirMatriz(int n, int matriz[][n], int inversa[][n]) 
{
    	float det = determinante(n, matriz);

    	inversa[0][0] = matriz[1][1] / det;
    	inversa[0][1] = -1*matriz[0][1] / det;
    	inversa[1][0] = -1*matriz[1][0] / det;
    	inversa[1][1] = matriz[0][0] / det;
}

void escribirMatrizLlave(int n, int matrizLlave[][n], char *path)
{           
	// Escribir en el archivo.
        FILE *file = fopen(path, "w");
        if (file == NULL) {
                printf("No se pudo abrir el archivo.\n");
                return;
        }

        for (int i=0; i < n; i++) {
        	for (int j=0; j < n; j++) {
        		fprintf(file, "%d,", matrizLlave[i][j]);
        	}
        }
        fclose(file);
}

// Via: https://www.youtube.com/watch?v=eKCFnHcIxWc
void leerMatrizLlave(int n, int matrizLlave[][n], char *path)
{
	char linea[100];
	char *entrada;
        int fila, columna;

        // Leer el archivo.
        FILE *file = fopen(path, "r");
        if (file == NULL) {
                printf("No se pudo abrir el archivo.\n");
                return;
        }

        fila = 0;
        while (!feof(file)) {
        	columna = 0;
        	fscanf(file, "%s", linea);
		entrada = strtok(linea, ",");
		while (entrada != NULL) {
			matrizLlave[fila][columna] = atoi(entrada);
			columna++;
			entrada = strtok(NULL, ",");
		}
		printf("\n");
		fila++;
        }
        fclose(file);
}

/* Via: https://stackoverflow.com/a/1469939
 * Regresa -1 si c no es un caracter alfanumerico.
 */
int caracterANumero(char c)
{
    	int n = -1;
    	static const char * const alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    	char *p = strchr(alphabet, toupper((unsigned char)c));

    	if (p) {
        	n = p - alphabet;
    	}

    	return n;
}

void inicializarVector(int n, int vector[n])
{
	for (int i=0; i < n; i++) {
		vector[i] = 0;
	}
}

void cifrar(int n, int matrizLlave[][n], char *palabra, char *path)
{
	const int len = 50;

	int vectorMensaje[n]; 
	int vectorCifrado[n];
	char palabraCifrada[len];
	int i, j;

	crearMatrizLlave(n, matrizLlave);
	puts("Esta es la matriz llave:");
	mostrarMatriz(n, matrizLlave);

	printf("Teclea la palabra que quieres cifrar: ");
	fgets(palabra, sizeof(palabra), stdin);

	i = 0;
	j = 0;
	while (palabra[i+1] != '\0') {
		inicializarVector(n, vectorMensaje);
		inicializarVector(n, vectorCifrado);
		vectorMensaje[0] = caracterANumero(palabra[i]);
		vectorMensaje[1] = caracterANumero(palabra[i+1]);
		vectorPorMatriz(n, vectorMensaje, matrizLlave, vectorCifrado);
		palabraCifrada[i] = 'A' + vectorCifrado[0];
		palabraCifrada[i+1] = 'A' + vectorCifrado[1];
		j++;
		i += 2;
	}

	printf("Hecho! La palabra cifrada es: %s.\n", palabraCifrada);
	escribirMatrizLlave(n, matrizLlave, path);
	printf("Se ha escrito la matriz llave en: %s\n", path);
}

void descifrar(int n, int matrizLlave[][n], char *palabraCifrada, char *path)
{
	const int len = 50;

	int matrizLlaveInversa[n][n];
	int vectorMensaje[n]; 
	int vectorCifrado[n];
	char palabra[len];
	int i, j;

	leerMatrizLlave(n, matrizLlave, path);
	puts("Esta es la matriz llave recibida:");
	mostrarMatriz(n, matrizLlave);
	invertirMatriz(n, matrizLlave, matrizLlaveInversa);

	printf("Teclea la palabra cifrada: ");
	getchar();
	fgets(palabraCifrada, sizeof(palabraCifrada), stdin);

	i = 0;
	j = 0;
	while (palabra[i+1] != '\0') {
		inicializarVector(n, vectorMensaje);
		inicializarVector(n, vectorCifrado);
		vectorCifrado[0] = caracterANumero(palabraCifrada[i]);
		vectorCifrado[1] = caracterANumero(palabraCifrada[i+1]);
		vectorPorMatriz(n, vectorCifrado, matrizLlaveInversa, vectorMensaje);
		palabra[i] = 'A' + vectorMensaje[0];
		palabra[i+1] = 'A' + vectorMensaje[1];
		j++;
		i += 2;
	}

	printf("Hecho! La palabra descifrada es: %s.\n", palabra);
}

int main(void)
{
	const int len = 50;
	const int n = 2;
	int matrizLlave[n][n];
	char palabra[len];
	char path[] = "llave.txt";
	int eleccion;

	eleccion = 0;
	printf("Quieres cifrar o descifrar un mensaje?\n");
	printf("1) Cifrar\n");
	printf("2) Descifrar\n");
	scanf("%d", &eleccion);
	getchar();
	
	switch (eleccion) {
		case 1: cifrar(n, matrizLlave, palabra, path); break;
		case 2: descifrar(n, matrizLlave, palabra, path); break;
		default: printf("Esa opcion no existe.");
	}

	return 0;
}
