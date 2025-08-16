#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    char origen[10];
    char simbolo;
    char destino[10];
} Transicion;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s config.txt cadenas.txt\n", argv[0]);
        return 1;
    }

    FILE *fconf = fopen(argv[1], "r");
    FILE *fcad = fopen(argv[2], "r");
    if (!fconf || !fcad) {
        printf("Error abriendo archivos\n");
        return 1;
    }

    char linea[100];
    char estados[MAX][10], finales[MAX][10], inicio[10];
    int num_finales = 0, num_trans = 0;
    Transicion trans[MAX];

    while (fgets(linea, sizeof(linea), fconf)) {
        if (strncmp(linea, "start:", 6) == 0) {
            sscanf(linea, "start:%s", inicio);
        } else if (strncmp(linea, "finals:", 7) == 0) {
            char *tok = strtok(linea+7, ",\n");
            while (tok) {
                strcpy(finales[num_finales++], tok);
                tok = strtok(NULL, ",\n");
            }
        } else if (strstr(linea, "->")) {
            char izq[20], der[20];
            sscanf(linea, "%[^-]->%s", izq, der);
            char *coma = strchr(izq, ',');
            *coma = '\0';
            strcpy(trans[num_trans].origen, izq);
            trans[num_trans].simbolo = *(coma+1);
            strcpy(trans[num_trans].destino, der);
            num_trans++;
        }
    }

    while (fgets(linea, sizeof(linea), fcad)) {
        char cadena[100]; strcpy(cadena, linea);
        cadena[strcspn(cadena, "\n")] = 0;
        if (strlen(cadena) == 0) continue;

        char estado[10]; strcpy(estado, inicio);
        for (int i = 0; i < strlen(cadena); i++) {
            char simbolo = cadena[i];
            int encontrado = 0;
            for (int j = 0; j < num_trans; j++) {
                if (strcmp(trans[j].origen, estado) == 0 && trans[j].simbolo == simbolo) {
                    strcpy(estado, trans[j].destino);
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) { strcpy(estado, ""); break; }
        }

        int aceptada = 0;
        for (int i = 0; i < num_finales; i++) {
            if (strcmp(estado, finales[i]) == 0) aceptada = 1;
        }

       if (aceptada)
            printf("acepta\n");
        else
            printf("NO acepta\n");
    }

    fclose(fconf);
    fclose(fcad);
    return 0;
}

