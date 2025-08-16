#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100   // tamaño máximo de arrays

// Estructura que guarda UNA transición δ
typedef struct {
    char origen[10];   // estado origen (ej: "q0")
    char simbolo;      // símbolo del alfabeto (ej: '0')
    char destino[10];  // estado destino (ej: "q1")
} Transicion;

int main(int argc, char *argv[]) {
    if (argc != 3) {   // debe recibir dos archivos como parámetros
        printf("Uso: %s config.txt cadenas.txt\n", argv[0]);
        return 1;
    }

    FILE *fconf = fopen(argv[1], "r");  // archivo de configuración
    FILE *fcad = fopen(argv[2], "r");   // archivo de cadenas
    if (!fconf || !fcad) {
        printf("Error abriendo archivos\n");
        return 1;
    }

    char linea[100];
    char finales[MAX][10], inicio[10];
    int num_finales = 0, num_trans = 0;
    Transicion trans[MAX];   // arreglo de transiciones

    // -------------------
    // PARSEO DE CONFIG.TXT
    // -------------------
    while (fgets(linea, sizeof(linea), fconf)) {
        if (linea[0] == '#' || linea[0] == '\n') continue; // ignorar comentarios y vacías

        if (strncmp(linea, "start:", 6) == 0) {
            sscanf(linea, "start:%s", inicio);  // guarda q0
        } 
        else if (strncmp(linea, "finals:", 7) == 0) {
            // separar finales por coma
            char *tok = strtok(linea+7, ",\n");
            while (tok) {
                strcpy(finales[num_finales++], tok);
                tok = strtok(NULL, ",\n");
            }
        } 
        else if (strstr(linea, "->")) {  // línea de transición
            char izq[20], der[20];
            sscanf(linea, "%[^-]->%s", izq, der);  // separa izq y der

            // izq está en formato "qX,a"
            char *coma = strchr(izq, ','); // busca la coma
            *coma = '\0';                  // corta en la coma
            strcpy(trans[num_trans].origen, izq); // origen
            trans[num_trans].simbolo = *(coma+1); // símbolo
            strcpy(trans[num_trans].destino, der); // destino
            num_trans++;
        }
    }

    // -------------------
    // PROCESAR CADENAS
    // -------------------
    while (fgets(linea, sizeof(linea), fcad)) {
        char cadena[100]; strcpy(cadena, linea);
        cadena[strcspn(cadena, "\n")] = 0; // quitar salto de línea
        if (strlen(cadena) == 0) continue; // ignorar vacías

        char estado[10]; strcpy(estado, inicio);  // estado actual = inicial

        // recorrer cada símbolo de la cadena
        for (int i = 0; i < strlen(cadena); i++) {
            char simbolo = cadena[i];
            int encontrado = 0;
            // buscar transición (origen, símbolo)
            for (int j = 0; j < num_trans; j++) {
                if (strcmp(trans[j].origen, estado) == 0 && trans[j].simbolo == simbolo) {
                    strcpy(estado, trans[j].destino); // actualizar estado
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) { strcpy(estado, ""); break; } // si no hay transición → rechaza
        }

        // revisar si estado final
        int aceptada = 0;
        for (int i = 0; i < num_finales; i++) {
            if (strcmp(estado, finales[i]) == 0) aceptada = 1;
        }

        // salida
        printf("%s\n", aceptada ? "acepta" : "NO acepta");
    }

    fclose(fconf);
    fclose(fcad);
    return 0;
}
