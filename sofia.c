#include <stdio.h>

void invertir_matriz_2x2(int matriz[2][2], int inversa[2][2]) 
{
    int determinante = matriz[0][0] * matriz[1][1] - matriz[0][1] * matriz[1][0];

    // Verificamos si la matriz es invertible 
    if (determinante == 0) 
    {
        printf("La matriz no es invertible.\n");
        return;
    }
    inversa[0][0] = matriz[1][1] / determinante;
    inversa[0][1] = -matriz[0][1] / determinante;
    inversa[1][0] = -matriz[1][0] / determinante;
    inversa[1][1] = matriz[0][0] / determinante;
}

int main() {
    int matriz[2][2], inversa[2][2];

    printf("Ingrese los elementos de la matriz 2x2:\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            scanf("%d", &matriz[i][j]);
        }
    }

    invertir_matriz_2x2(matriz, inversa);

    printf("La matriz inversa es:\n");
    for (int i = 0; i < 2; i++) 
    {
        for (int j = 0; j < 2; j++) 
	{
            printf("%d ", inversa[i][j]);
        }
        printf("\n");
    }

    return 0;
}
