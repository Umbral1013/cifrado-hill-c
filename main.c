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

#define MIN 0
#define MAX 25
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
	char *end; // Esto es solo para marcar el fin de los argumentos.

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

int modulo(int a, int b)
{
	/* El compilador de C no es tan listo para sacar
         * el modulo de un numero negativo.
         * Nosotros si, esperemos.
	 */
	return (a < 0) ? ( b - (-1*a % b) ) : (a % b);
}

int determinante(int n, int a[][n])
{
	int det = (a[0][0] * a[1][1]) - (a[0][1] * a[1][0]);
	det = modulo(det, BASE_MOD);
	return det;
}

void llenarEntradasLlave(int n, int llave[][n])
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
			int entrada = rand() % (MAX+1 - MIN) + MIN;
			llave[i][j] = entrada;
		}
}

int mcd(int a, int b)
{
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
	if ((a[0][0] == 0) || (a[1][1] == 0))
		// Si sus pivotes son 0, no se puede invertir.
		return 0;
	else if (mcd( BASE_MOD, determinante(n, a) ) != 1)
		// Significa que no son coprimos.
		return 0;
	else
		return 1;
}

void crearLlave(int n, int a[][n])
{
	// Debemos asegurarnos de que sea invertible.
	while ( !( esInvertible(n, a) ) ) {
		llenarEntradasLlave(n, a);
	}
}

void inicializarVector(int n, int u[n])
{
	int i;
	for (i=0; i < n; i++)
		u[i] = 0;
}

void vectorPorMatriz(int n, int u[n], int a[][n], int res[n])
{
	res[0] = a[0][0] * u[0] + a[0][1] * u[1];
	res[0] = modulo(res[0], BASE_MOD);
	res[1] = a[1][0] * u[0] + a[1][1] * u[1];
	res[1] = modulo(res[1], BASE_MOD);
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

    	/* Metodo "ingenuo" de encontrar el inverso del determinante.
    	 * Via https://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/
    	 * Seria mejor usar el algoritmo extendido de Euclides.
    	 */
    	int x, invDet;
    	for (x = MIN; x < BASE_MOD; x++)
    		if ( (det % BASE_MOD) * (x % BASE_MOD) % BASE_MOD == 1 )
			invDet = x;

    	b[0][0] = modulo(a[1][1]*invDet, BASE_MOD);
    	b[1][1] = modulo(a[0][0]*invDet, BASE_MOD);
    	b[0][1] = modulo( (-1 * a[0][1]*invDet), BASE_MOD);
    	b[1][0] = modulo( (-1 * a[1][0]*invDet), BASE_MOD);
}

void escribirLlave(int n, int m[][n], char *path)
{           
	// Abrimos el archivo con permisos de escritura.
        FILE *file = fopen(path, "w");
        if (file == NULL) {
                puts("No se pudo abrir el archivo.");
                return;
        }

	/* Escribir en el archivo. Por ahora, no escribimos fin de linea, eso
	 * ocasiona cosas raras...
	 */
        int i, j;
        for (i=0; i < n; i++)
        	for (j=0; j < n; j++)
        		fprintf(file, "%d,", m[i][j]);

        fclose(file);
}

void leerLlave(int n, int m[][n], char *path)
{
	/* Via: https://www.youtube.com/watch?v=eKCFnHcIxWc
	*/

        // Solamente leemos el archivo.
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

void formatearEntrada(char *str)
{
	// Cambia cada caracter de la cadena a mayusculas.
	char *c = str;
	while (*c) {
		*c = toupper( (unsigned char) *c );
		c++; // c es un puntero, lo estamos incrementando.
	}
}

void cifrar(int n, int llave[][n], char *s, char *path)
{
	crearLlave(n, llave);

	printf(">> Teclea la palabra que quieres cifrar: ");
	fgets(s, sizeof(s), stdin);

	// La entrada tiene que tener mas de dos caracteres, sin contar a '\0'.
	formatearEntrada(s);

	// Numero requerido para convertir a la clave.
	const int N_CONVERSION = 65; 

	int i;
	char cifrada[strlen(s) + 1];
	for (i=0; i < strlen(s) - 1; i++) {
		int u[n];
		int v[n];

		printf("%d: %c, %d\n", i, s[i], s[i] - N_CONVERSION);
		u[0] = s[i] - N_CONVERSION;
		u[1] = s[i+1] - N_CONVERSION;

		vectorPorMatriz(n, u, llave, v);
		printf("%d: %c, %d\n", i, v[i] + 'A', v[i]);

		cifrada[i] = 'A' + v[i];
		cifrada[i+1] = 'A' + v[i+1];
	}

	printf("Hecho! el mensaje cifrado es: %s\n", cifrada);

	mostrarMatriz(n, llave);
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

	formatearEntrada(s);

	int i = 0;
	char *c = s;
	char mensaje[sizeof(s)]; // Podria ser mejor.
	while (*c) {
		int u[n];
		int v[n];
		u[0] = (*c + 0) - 65;
		u[1] = (*c + 0) - 65;

		vectorPorMatriz(n, u, llave, v);
		mensaje[i] = 'A' + v[0];
		mensaje[i+1] = 'A' + v[1];

		i++;
	}

	printf("El mensaje es: %s\n", mensaje);
}
