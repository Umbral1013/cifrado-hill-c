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

#define MIN 0
#define MAX 26
#define BASE_MOD 26

int modulo(int a, int b)
{
	/* El compilador de C no es tan listo para sacar
         * el modulo de un numero negativo.
	 */
	if (a < 0)
		return b - (-1*a % b);
	else
		return a % b;
}

int determinante(int n, int a[][n])
{
	int det = (a[0][0] * a[1][1]) - (a[0][1] * a[1][0]);
	det = modulo(det, BASE_MOD);
	return det;
}

void llenarMatrizAleatorios(int n, int a[][n])
{
	/* Usar rand() no es buena idea, hablando en el
	 * contexto de la criptografia. De todos modos, esto es
	 * solo una demostracion.
	 */
	srand(time(NULL));
	
	int i, j;
	for (i=0; i < n; i++)
		for (j=0; j < n; j++) {
			// e de entrada.
			int e = rand() % (max+1 - min) + min;
			a[i][j] = e;
		}
}

void crearMatrizLlave(int n, int a[][n])
{
	int det = 0;
	while (det == 0) {
		llenarMatrizAleatorios(n, a);
		det = determinante(n, a);
	}
}

/* a es la matriz.
 * u es el vector.
 * v es el resultado (un vector).
 */
void vectorPorMatriz(int n, int u[n], int a[][n], int v[n])
{
	int i, j;
	for (i=0; i < n; i++) {
                for (j=0; j < n; j++)
			v[i] += u[j] * a[i][j];
		
		v[i] = modulo(v[i], BASE_MOD);
	}
}

void mostrarMatriz(int n, int a[][n])
{
	int i, j;
	for (i=0; i < n; i++) {
		for (j=0; j < n; j++)
			printf("\t%d", a[i][j]);
		
                printf("\n");
	}
}

void mostrarVector(int n, int u[n])
{
	int i;
	for (i=0; i < n; i++)
            	printf("%d\n", u[i]);
	
        printf("\n");
}

/* a es la matriz original.
 * b es a invertida.
 */
void invertirMatriz(int n, int a[][n], int b[][n])
{
    	int det = determinante(n, matriz);
    	int invDet = modulo(det*det, BASE_MOD);

	printf("Determinante: %d.\n", det);
	printf("Inverso del determinante: %d.\n", invDet);

    	b[0][0] = modulo(a[1][1]*invDet, BASE_MOD);
    	b[1][1] = modulo(a[0][0]*invDet, BASE_MOD);
    	b[0][1] = modulo( (-1 * a[0][1]*invDet), BASE_MOD);
    	b[1][0] = modulo( (-1 * a[1][0]*invDet), BASE_MOD);
}

void escribirMatrizLlave(int n, int m[][n], char *path)
{           
        FILE *file = fopen(path, "w");
        if (file == NULL) {
                puts("No se pudo abrir el archivo.");
                return;
        }

	// Escribir en el archivo.
        int i, j;
        for (i=0; i < n; i++) {
        	for (j=0; j < n; j++)
        		fprintf(file, "%d,", m[i][j]);

        fclose(file);
}

// Via: https://www.youtube.com/watch?v=eKCFnHcIxWc
void leerMatrizLlave(int n, int m[][n], char *path)
{
        // Leer el archivo.
        FILE *file = fopen(path, "r");
        if (file == NULL) {
                puts("No se pudo abrir el archivo.");
                return;
        }
	
        int fila = 0;
        while ( !feof(file) ) {
		char linea[100];
        	fscanf(file, "%s", linea);
		char *entrada;
		entrada = strtok(linea, ",");
		
		int columna = 0;
		while (entrada != NULL) {
			m[fila][columna] = atoi(entrada);
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
	char guardarLlave = 'n';

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

	printf(">> Quieres guardar esta matriz llave? (s/N): ");
	scanf("%c", &guardarLlave);
	getchar();
	if (guardarLlave == 's') {
		escribirMatrizLlave(n, matrizLlave, path);
		printf("Se ha escrito la matriz llave en: %s\n", path);
	}
}

void descifrar(int n, int matrizLlave[][n], char *palabraCifrada, char *path)
{
	const int len = 50;

	int i, j;
	int matrizLlaveInversa[n][n];
	int vectorCifrado[n];
	int vectorMensaje[n];
	char palabra[len];

	leerMatrizLlave(n, matrizLlave, path);
	puts("Mostrando matriz llave recibida:");
	mostrarMatriz(n, matrizLlave);
	puts("Mostrando la matriz llave invertida:");
	invertirMatriz(n, matrizLlave, matrizLlaveInversa);
	mostrarMatriz(n, matrizLlaveInversa);
	
	printf("Teclea la palabra cifrada: ");
	fgets(palabraCifrada, sizeof(palabraCifrada), stdin);

	i = 0;
	j = 0;
	while (palabraCifrada[i+1] != '\0') {
		inicializarVector(n, vectorCifrado);
		inicializarVector(n, vectorMensaje);
		vectorCifrado[0] = caracterANumero(palabraCifrada[i]);
		vectorCifrado[1] = caracterANumero(palabraCifrada[i+1]);
		vectorPorMatriz(n, vectorCifrado, matrizLlaveInversa, vectorMensaje);
		palabra[i] = 'A' + vectorMensaje[0];
		palabra[i+1] = 'A' + vectorMensaje[1];
		i += 2;
		j++;
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
	puts("Cifrado Hill.");
	puts("Quieres cifrar o descifrar un mensaje?");
	puts("1) Cifrar");
	puts("2) Descifrar");
	printf(">> ");
	scanf("%d", &eleccion);
	getchar();
	
	switch (eleccion) {
		case 1: cifrar(n, matrizLlave, palabra, path); break;
		case 2: descifrar(n, matrizLlave, palabra, path); break;
		default: printf("Esa opcion no existe.");
	}

	return 0;
}
