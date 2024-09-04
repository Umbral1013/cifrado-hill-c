#include <stdio.h>

//Función para multiplicar un vector por la matriz 
void VectorPorMatriz(int vector[], int matriz[][2], int resultado [], int filas, int columnas) {
	for (int i = 0; i < filas; i++){
		resultado[i] = 0;
                for (int j = 0; j <columnas; j++){
			resultado[i] += vector[j] * matriz[i][j];
		}
	}
}

//Función para mostrar la matriz 2*2
void mostrarMatriz(int matriz[][2], int filas, int columnas){
	for (int i = 0; i < filas; i++){
		for (int j =0; j < columnas; j++){
			printf("%d ", matriz[i][j]);
		}
                printf("\n");
	}
}

//Función para mostrar un vector
void mostrarVector(int vector[], int tamaño) {
	for (int i = 0; i < tamaño; i++) {
            printf("%d ",vector[i]);
	}
        printf("\n");
}

int main() {
	int vector[2];
        int matriz[2][2];
        int resultado[2];

//Solicitar entrada para el vector
printf("Introduce los elemntos del vector (2 elementos):\n");
for (int i = 0; i<2; i++){
	printf("vector[%d]: ", i);
	scanf("%d", &vector[i]);
}
// Solicitar entrada para la matriz
    printf("Introduce los elementos de la matriz (2x2):\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            printf("matriz[%d][%d]: ", i, j);
            scanf("%d", &matriz[i][j]);
        }
    }

    // Mostrar la matriz y el vector
    printf("\nMatriz:\n");
    mostrarMatriz(matriz, 2, 2);

    printf("\nVector:\n");
    mostrarVector(vector, 2);

    // Multiplicar el vector por la matriz
    VectorPorMatriz(vector, matriz, resultado, 2, 2);

    // Mostrar el resultado
    printf("\nResultado de la multiplicación del vector por la matriz:\n");
    mostrarVector(resultado, 2);

    return 0;
}


