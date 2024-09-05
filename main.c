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

#define BASE_MOD 26

int modulo(int a, int b)
{
	int resultado = 0;
	
	/* Si el determinante es negativo, tenemos que hacer esto
	 * para conseguirlo. El compilador de C no es tan listo.
	 */
	if (a < 0) {
		resultado = b - (-1*a % b);
	} else {
		resultado = a % b;
	}

	return resultado;
}

int determinante(int n, int matriz[][n])
{
	int determinante = 0;

	determinante = (matriz[0][0]*matriz[1][1]) - (matriz[0][1]*matriz[1][0]);
	determinante = modulo(determinante, BASE_MOD);
	return determinante;
}

void llenarMatrizAleatorios(int n, int matriz[][n], int min, int max)
{
	// Impide que min sea mayor que max.
	if (min > max) return;

	int entrada;

	srand(time(NULL));
	for (int i=0; i < n; i++) {
		for (int j=0; j < n; j++) {
			/* Usar rand() no es buena idea, hablando en el
			 * contexto de la criptografia. De todos modos, esto es
			 * solo una demostracion.
			 */
			entrada = rand() % (max + 1 - min) + min;
			matriz[i][j] = entrada;
		}
	}
}

void crearMatrizLlave(int n, int matriz[][n])
{
	const int min = 0;
	const int max = 25;

	int det = 0;

	while (det == 0) {
		llenarMatrizAleatorios(n, matriz, min, max);
		det = determinante(n, matriz);
	}
}

void vectorPorMatriz(int n, int vector[n], int matriz[][n], int resultado [n])
{
	for (int i=0; i < n; i++) {
                for (int j=0; j < n; j++) {
			resultado[i] += (vector[j] * matriz[i][j]);
		}
		resultado[i] = modulo(resultado[i], BASE_MOD);
	}
}

void mostrarMatriz(int n, int matriz[][n])
{
	for (int i=0; i < n; i++) {
		for (int j=0; j < n; j++) {
			printf("\t%d", matriz[i][j]);
		}
                printf("\n");
	}
}

void mostrarVector(int n, int vector[n])
{
	for (int i=0; i < n; i++) {
            	printf("%d\n",vector[i]);
	}
        printf("\n");
}

/* Probablemente podria ser mas eficiente, pero esta fue la solucion a la que
 * llegue.
 */
void invertirMatriz(int n, int matriz[][n], int inversa[][n]) 
{
    	int det = determinante(n, matriz);
    	int inversoDet = 0;

	printf("Determinante: %d.\n", det);
	inversoDet = modulo(det*det, BASE_MOD);
	printf("Inverso del determinante: %d.\n", inversoDet);

	// Esto es lo que esta mal, creo. Le falta algo.
    	inversa[0][0] = modulo(matriz[1][1] * inversoDet, BASE_MOD);
    	inversa[1][1] = modulo(matriz[0][0] * inversoDet, BASE_MOD);
    	inversa[0][1] = modulo(-1 * matriz[0][1] * inversoDet, BASE_MOD);
    	inversa[1][0] = modulo(-1 * matriz[1][0] * inversoDet, BASE_MOD);
}

void escribirMatrizLlave(int n, int matrizLlave[][n], char *path)
{           
        FILE *file = fopen(path, "w");
        if (file == NULL) {
                printf("No se pudo abrir el archivo.\n");
                return;
        }

	// Escribir en el archivo.
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
