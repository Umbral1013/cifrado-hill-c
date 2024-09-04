#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Solamente funcionara con matrices de 2x2. */
float determinante(int n, int matriz[n][n])
{
	int determinante;
	int a, b, c, d;

	a = matriz[0][0]; 
	b = matriz[1][1];
	c = matriz[0][1];
	d = matriz[1][0];

	/* La diagonal principal por la diagonal secundaria. */
	determinante = a*d - c*b;

	return determinante;
}

void llenarMatrizAleatorios(int n, int matriz[n][n])
{
	const int max = 29; 
	const int min = 1;

	int r;

	/* Llenamos una matriz con numeros al azar con numeros entre 1 y 29. */
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

void crearMatrizLlave(int n, int matriz[n][n])
{
	int det = 0;

	while (det == 0) {
		llenarMatrizAleatorios(n, matriz);
		det = determinante(n, matriz);
	}
}

/* Esto solo me permite probar que si funciona. No es necesario copiarlo al
 * main del programa final.
 */
int main(void)
{
	const int n = 2;
	int miMatriz[n][n];

	crearMatrizLlave(n, miMatriz);

	puts("Muestro una posible matriz llave.");
	for (int i=0; i < n; i++) {
		for (int j=0; j < n; j++) {
			printf(" %d ", miMatriz[i][j]);
		}
		puts("");
	}
	return 0;
}
