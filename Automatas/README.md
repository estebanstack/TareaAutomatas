## ¿QUE REALICÉ EN ESTE REPOSITORIO?
En este trabajo implementé un Autómata Finito Determinista (AFD) tanto en Python como en C.
La idea es que el programa reciba la definición formal de un AFD desde un archivo de configuración (config.txt) y luego lea un archivo con cadenas de prueba (cadenas.txt) para decidir si cada cadena es aceptada o NO aceptada por el autómata.

## Pero antes que todo ¿CÓMO FUNCIONA UNA AUTOMATA FINITO DETERMINISTA (AFD)?

Un AFD se define con una quíntupla $(𝑄,Σ,𝛿,𝑞0,𝐹)$

Q -> conjunto de estados (ejemplo: q0, q1, q2).

Σ -> alfabeto (símbolos que puede leer, por ejemplo: 0 y 1).

δ -> función de transición (qué hacer en cada estado con cada símbolo).

q0 -> estado inicial.

F -> conjunto de estados finales.

El autómata arranca en el estado inicial y va leyendo la cadena símbolo por símbolo. Cada vez que lee, consulta δ para ver a qué estado pasar. Al final:

Si termina en un estado final -> acepta.

Si no -> NO acepta.

## Archivos de entrada

# config.txt
Define la quíntupla del AFD:

    states:q0,q1,q2
    alphabet:0,1
    start:q0
    finals:q2
    transitions:
    q0,0->q1
    q0,1->q0
    q1,0->q2
    q1,1->q0
    q2,0->q2
    q2,1->q2

# cadenas.txt
Contiene las cadenas que se van a probar:

    00
    1011001
    1
    10101

## Implementación en Python
```python
    import sys
```
Primero importo sys para leer los parámetros de la línea de comandos: config.txt y cadenas.txt.
```python
    def cargar_config(archivo):
```
Posteriormente defino la función que abre y lee la quíntupla (estados, alfabeto, etc.) desde config.txt.
```python
    with open(archivo) as f:
        lineas = [l.strip() for l in f if l.strip() and not l.startswith("#")]
```
Despues abro el archivo y creo una lista de lineas: 
  - strip() borra los espacios y saltos de linea
  - ignora lineas vacias
  - ignora comentarios es decir las lineas que empiezan con "#".
    
```python
estados = lineas[0].split(":")[1].split(",")     # Q 
alfabeto = lineas[1].split(":")[1].split(",")    # Σ
        inicio = lineas[2].split(":")[1]                 # q0
        finales = lineas[3].split(":")[1].split(",")     # F
```
Acá asume el orden:
  - states:... en lineas[0]
  - alphabet:... en lineas[1]
  - start:... en lineas[2]
  - finals:... en lineas[3]

    
Extrae tambien cada parte separando por : y luego por ,

```python
    transiciones = {}
```
Creo el diccionario vacio para las transiciones en donde:
  - La clave está definida por (estado_origen, simbolo)
  - Y el valor por: estado_destino

```python
    for l in lineas[5:]:
        izq, der = l.split("->")
        estado, simbolo = izq.split(",")
        transiciones[(estado, simbolo)] = der
```
Acá como a partir de la linea 5 en adelante están las transiciones (debido a que en la linea cuatro simplemente dice "transitions") divide 
cada linea $qX, a -> qY$ en izquierda y derecha, en izquierda separa estado y simbolo por "," y guarda la transición de la forma: (estado, simbolo) = destino

```python
    return estados, alfabeto, inicio, finales, transiciones
```
Y acá devuelve todo lo necesario para simular el automata

```python
    def procesar_cadena(cadena, inicio, finales, transiciones):
      estado = inicio
```
Se crea la funcion procesar_cadena y se comienza en el estado inicial q0

```python
    for c in cadena:
        if (estado, c) in transiciones:
            estado = transiciones[(estado, c)]
        else:
            return False
```
Esta parte recorre simbolo por simbolo. Sí hay transicion definida para (estado_actual, simbolo) avanza al nuevo estado. Si no hay transición
rechaza

```python
    return estado in finales
```
Al terminar, acepta si el estado actual es final.

```python
    def main():
    if len(sys.argv) != 3:
        print("Uso: python afd.py config.txt cadenas.txt")
        return
```
Acá valido si hay 2 archivos como parametros

```python
        estados, alfabeto, inicio, finales, transiciones = cargar_config(sys.argv[1])
```
Carga la quintupla desde config.txt

```python
        with open(sys.argv[2]) as f:
        for linea in f:
            cadena = linea.strip()
            if not cadena: continue
            aceptada = procesar_cadena(cadena, inicio, finales, transiciones)
            print("ACEPTA" if aceptada else "NO ACEPTA")
```
En este fragmento:
  1. Abre cadenas.txt
  2. Para cada linea no vacía:
       - Simula el automata
       - Imprime solo "acepta" o "NO acepta"

```python
    if __name__ == "__main__":
      main()
```
Punto de entrada del script

Se ejecuta así:

    python afd.py config.txt cadenas.txt

## Implementacion en C
Primero importamos las cebidas librerias para lo que vamos a usar a continuacion 

```c
    #define MAX 100
```
Creamos esta constante MAX que será el tamaño maximo simple para todos los arreglos

```c
    typedef struct {
    char origen[10];
    char simbolo;
    char destino[10];
    } Transicion;
```
Creamos la estructura para una sola transicion en donde: 
  - "origen" es el nombre del estado de origen
  - "simbolo" es un caracter del alfabeto
  - "destino" es el nombre del estado de destino

```c
    if (argc != 3) {
        printf("Uso: %s config.txt cadenas.txt\n", argv[0]);
        return 1;
    }
```
Dentro de la funcion main verifico que se pasaron los dos archivos como parametro

```c
    FILE *fconf = fopen(argv[1], "r");
    FILE *fcad = fopen(argv[2], "r");
    if (!fconf || !fcad) {
        printf("Error abriendo archivos\n");
        return 1;
    }
```
Acá abre config.txt y cadenas.txt, si falla arroja error

```c
    char linea[100];
    char finales[MAX][10], inicio[10];
    int num_finales = 0, num_trans = 0;
    Transicion trans[MAX];
```
Se crean los buffers y estructuras las cuales:
  - linea: es un buffer para leer las lineas del archivo
  - finales: es la lista de nombres de estados finales
  - inicio: nombre del estado inicial
  - num_finales: es cuantos finales se guardan
  - trans: es el arreglo de transiciones
  - num_trans: son cuantas transiciones se cargan

```c
    while (fgets(linea, sizeof(linea), fconf)) {
```
Se lee config.txt linea por linea

```c
    if (strncmp(linea, "start:", 6) == 0) {
      sscanf(linea, "start:%s", inicio);
```
Si la linea comienza con "start:", extrae el nombre del estado inicial (cómo q0) con sscanf

```c
        } else if (strncmp(linea, "finals:", 7) == 0) {
            char *tok = strtok(linea+7, ",\n");
            while (tok) {
                strcpy(finales[num_finales++], tok);
                tok = strtok(NULL, ",\n");
            }
```
Si es "finals:", parte el resto por comas e intros de linea. Luego va copiando cada nombre a la matriz "finales[][]" e incrementa num_finales

```c
        } else if (strstr(linea, "->")) {
            char izq[20], der[20];
            sscanf(linea, "%[^-]->%s", izq, der);
```
Si la linea contiene "->", se trata como transición. "sscanf("<%[^-]->%s")" se encarga de que en "izq" vaya todo lo anterior al "-" (es decir $qX,a$),
y "der" hace que lo de la derecha sea $qY$

```c
            char *coma = strchr(izq, ',');
            *coma = '\0';
            strcpy(trans[num_trans].origen, izq);
            trans[num_trans].simbolo = *(coma+1);
            strcpy(trans[num_trans].destino, der);
            num_trans++;
```
En "izq" busco la coma que separa origen y simbolo, luego corto "izq" en la coma para quedarme con el origen. El simbolo queda justo despues de la coma: 
"*(coma+1)". Y por ultimo copio a la estructura Transicion y aumento num_trans

Sigue leyendo hasta terminar config.txt

```c
   while (fgets(linea, sizeof(linea), fcad)) {
      char cadena[100]; strcpy(cadena, linea);
      cadena[strcspn(cadena, "\n")] = 0;
      if (strlen(cadena) == 0) continue;
```
Ahora se lee cada cadena de cadenas.txt, copio al buffer "cadena" y le quito el salto de linea, si está vacía se salta

```c
        char estado[10]; strcpy(estado, inicio);
```
Se empieza desde q0

```c
        for (int i = 0; i < strlen(cadena); i++) {
            char simbolo = cadena[i];
            int encontrado = 0;
```
Recorro la cadena simbolo por simbolo 

```c
            for (int j = 0; j < num_trans; j++) {
                if (strcmp(trans[j].origen, estado) == 0 && trans[j].simbolo == simbolo) {
                    strcpy(estado, trans[j].destino);
                    encontrado = 1;
                    break;
                }
            }
```
Busco linealmente la transicion que coincida con:
  - origen == estado_actual
  - simbolo == simbolo actual de la cadena

Si lo encuentro, cambio el estado destino y sigo recorriendo

```c
            if (!encontrado) { strcpy(estado, ""); break; }
        }
```
Si no hay transicion para ese par, rechazo (dejo el estado vacio y finalizo)

```c
        int aceptada = 0;
        for (int i = 0; i < num_finales; i++) {
            if (strcmp(estado, finales[i]) == 0) aceptada = 1;
        }
```
Al terminar, verifico si estado coincide con algun estado final

```c
        printf("%s\n", aceptada ? "acepta" : "NO acepta");
```
Si se queda en el estado final imprime acepta de lo contrario imprime NO acepta. Y sigue con la cadena posterior

```c
    fclose(fconf);
    fclose(fcad);
    return 0;
}
```
Cierro los archivos y termina el programa

Se compila y ejecuta así: 

    gcc afd.c -o afd
    ./afd config.txt cadenas.txt

## EJEMPLO DE SALIDA
Con el AFD del ejemplo y las cadenas de prueba:

    acepta
    acepta
    NO acepta
    NO acepta

## CONCLUSION
Con este proyecto entendí cómo un autómata se traduce de la teoría a un programa real.

El archivo config.txt representa la definición formal (la quíntupla).

Los programas en Python y C son los que hacen la simulación.

Al final, el autómata decide de manera automática si una cadena pertenece o no al lenguaje que reconoce.






