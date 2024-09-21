/*
 * Nombre del programa: Cifrado Hill.
 * Fecha: mar 03 sep 2024 17:11:23
 * Descripcion del programa: Cifra una cadena usando el metodo de cifrado
 * 	inventado por Lester S. Hill.
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

#define MIN 1
#define BASE_MOD 26

int determinante(int n, int a[][n]);
int esInvertible(int n, int a[][n]);
int mcd(int a, int b);
int modulo(int a, int b);
void cifrar(int n, int llave[][n], char *s, char *path);
void crearLlave(int n, int a[][n]);
void descifrar(int n, int llave[][n], char *s, char *path);
void escribirLlave(int n, int m[][n], char *path);
void invertirModularMatriz(int n, int a[][n], int b[][n]);
void leerLlave(int n, int m[][n], char *path);
void llenarEntradasLlave(int n, int a[][n]);
void mostrarMatriz(int n, int a[][n]);
void vectorPorMatriz(int n, int u[n], int a[][n], int v[n]);

int main(int argc, char **argv)
{
	if (argc != 2) {
		puts("Uso:");
		printf("%s <modo>\n", argv[0]);
		printf("%s 1 (cifrar)\n", argv[0]);
		printf("%s 2 (descifrar)\n", argv[0]);
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
			printf("%d\n", (('B' + 0) - 64) );
			printf("%d\n", mcd(32, 16)); // BORRAR.
			cifrar(N, llave, mensaje, path);
			break;
		case 2:
			descifrar(N, llave, mensaje, path);
			break;
		default: puts("Esa opcion no existe.");
	}

	exit(EXIT_SUCCESS);
}

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

void llenarEntradasLlave(int n, int a[][n])
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
			int e = rand() % (BASE_MOD+1 - MIN) + MIN;
			a[i][j] = e;
		}
}

int mcd(int a, int b)
{
        /* Asumimos que a es mayor o igual que b.
        */
	if (b > a) {
		int aux = a;
		a = b;
		b = aux;
	}

	if (b == 0)
		return a;

	int r = a % b;
	if (r == 0)
		return b;
	else if (r == 1)
		return r;
	else
		return mcd(b, r);
}

int esInvertible(int n, int a[][n])
{
	if ( (a[0][0] == 0) || (a[1][1] == 0) )
		return 0;
	else
		return 1;
}

void crearLlave(int n, int a[][n])
{
	// Debemos asegurarnos de que sea invertible.
	while ( !(esInvertible(n, a)) ) {
		llenarEntradasLlave(n, a);
	}
}

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

void invertirModularMatriz(int n, int a[][n], int b[][n])
{
    	int det = determinante(n, a);
    	int invDet = modulo(det*det, BASE_MOD);

    	b[0][0] = modulo(a[1][1]*invDet, BASE_MOD);
    	b[1][1] = modulo(a[0][0]*invDet, BASE_MOD);
    	b[0][1] = modulo( (-1 * a[0][1]*invDet), BASE_MOD);
    	b[1][0] = modulo( (-1 * a[1][0]*invDet), BASE_MOD);
}

void escribirLlave(int n, int m[][n], char *path)
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

void leerLlave(int n, int m[][n], char *path)
{
	// Via: https://www.youtube.com/watch?v=eKCFnHcIxWc

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
		fila++;
        }

        fclose(file);
}

void cifrar(int n, int llave[][n], char *s, char *path)
{
	crearLlave(n, llave);
	puts("Se ha generado la siguiente matriz llave:");
	mostrarMatriz(n, llave);

	printf(">> Teclea la palabra que quieres cifrar: ");
	fgets(s, sizeof(s), stdin);

	const int LEN = 50;
	char cifrada[LEN];
	int i=0;
	while (s[i+1] != '\0') {
		int u[n];
		int v[n];

		// Obtenemos el valor ordinal de la letra en cuestion.
		u[0] = (s[i] + 0) - 64;
		u[1] = (s[i+1] + 0) - 64;

		vectorPorMatriz(n, u, llave, v);
		cifrada[i] = 'A' + v[0];
		cifrada[i+1] = 'A' + v[1];

		i += 2;
	}

	printf("Hecho! el mensaje cifrado es: %s\n", cifrada);
	printf(">> Quieres guardar esta matriz llave? (s/N): ");
	if ( (getchar() == 's') || (getchar() == 'S') ) {
		escribirLlave(n, llave, path);
		printf("Se ha escrito la matriz llave en: %s\n", path);
	}
}

void descifrar(int n, int llave[][n], char *s, char *path)
{
	leerLlave(n, llave, path);

	int invLlave[n][n];
	invertirModularMatriz(n, llave, invLlave);

	// Cuando funcione bien, borrare esto.
	puts("Mostrando la matriz llave invertida:");
	mostrarMatriz(n, invLlave);

	printf(">> Teclea el mensaje cifrado: ");
	fgets(s, sizeof(s), stdin);

	const int LEN = 50;
	char mensaje[LEN];
	int i=0;
	while (s[i+1] != '\0') {
		int u[n];
		int v[n];

		u[0] = (s[i] + 0) - 64;
		u[1] = (s[i+1] + 0) - 64;
		vectorPorMatriz(n, u, llave, v);

		mensaje[i] = 'A' + v[0];
		mensaje[i+1] = 'A' + v[1];

		i += 2;
	}
	printf("El mensaje es: %s\n", mensaje);
}
