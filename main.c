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
			int e = rand() % (MAX+1 - MIN) + MIN;
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
    	int det = determinante(n, a);
    	int invDet = modulo(det*det, BASE_MOD);

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
        for (i=0; i < n; i++)
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

void inicializarVector(int n, int u[n])
{
	int i;
	for (i=0; i < n; i++)
		u[i] = 0;
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

void descifrar(int n, int llave[][n], char *s, char *path)
{
	leerMatrizLlave(n, llave, path);

	int invLlave[n][n];
	invertirMatriz(n, llave, invLlave);

	// Cuando funcione bien, borrare esto.
	puts("Mostrando la matriz llave invertida:");
	mostrarMatriz(n, invLlave);
	
	printf(">> Teclea el mensaje cifrado: ");
	fgets(s, sizeof(s), stdin);

	const int LEN = 50;
	char mensaje[LEN];
	int i=0;
	while (s[i++] != '\0') {
		int u[n];
		inicializarVector(n, u);
		int v[n];
		inicializarVector(n, v);

		u[0] = caracterANumero(s[i]);
		u[1] = caracterANumero(s[i++]);
		vectorPorMatriz(n, u, llave, v);

		mensaje[i] = 'A' + v[0];
		mensaje[i++] = 'A' + v[1];

		i += 2;
	}
	printf("El mensaje es: %s.\n", mensaje);
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		puts("Uso:");
		printf("%s 1 (Cifrar un mensaje).\n", argv[0]);
		printf("%s 2 (Descifrar un mensaje).\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	const int LEN = 50;
	const int N = 2;

	int llave[N][N];
	char mensaje[LEN];
	char path[] = "llave.txt";
	char *end;

	int modo = strtol(argv[1], &end, 10);
	switch (modo) {
		case 1:
			cifrar(N, llave, mensaje, path);
			break;
		case 2:
			descifrar(N, llave, mensaje, path);
			break;
		default: puts("Esa opcion no existe.");
	}

	exit(EXIT_SUCCESS);
}
