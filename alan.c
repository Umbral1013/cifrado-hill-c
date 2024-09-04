#include <stdio.h>

int main() {
    int flag;
    char path[]="test.txt";

    do {
        printf("Que desea hacer? Ingrese 1 para escribir o 0 para leer:\n");
        scanf("%i", &flag);

        if (flag == 1) {
            // Escribir en el archivo
            FILE *file = fopen(path, "w");
            if (file == NULL) {
                printf("No se pudo abrir el archivo.\n");
                return 1;
            }

            fprintf(file, "Se edito el txt");
            fprintf(file, "\nEsta es la segunda línea del txt\n");
            fclose(file);
            printf("Texto escrito en el archivo.\n");

        } else if (flag == 0) {
            // Leer el archivo
            FILE *file = fopen(path, "r");
            if (file == NULL) {
                printf("No se pudo abrir el archivo.\n");
                return 1;
            }

            char line[100];
            printf("----------------Contenido del archivo----------------\n");
            while (fgets(line, sizeof(line), file) != NULL) {
                printf("%s", line);
            }
            printf("-----------------------------------------------------\n");
            fclose(file);

        } else {
            printf("Opción no valida.\n");
        }

        printf("\nDesea continuar? Ingrese 1 para escribir, 0 para leer o cualquier otro numero para salir: \n");
        scanf("%i", &flag);

    } while (flag == 1 || flag == 0);

    return 0;
}
