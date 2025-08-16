import sys

def cargar_config(archivo):
    with open(archivo) as f:
        lineas = [l.strip() for l in f if l.strip() and not l.startswith("#")]
    
    estados = lineas[0].split(":")[1].split(",")
    alfabeto = lineas[1].split(":")[1].split(",")
    inicio = lineas[2].split(":")[1]
    finales = lineas[3].split(":")[1].split(",")
    
    transiciones = {}
    for l in lineas[5:]:
        izq, der = l.split("->")
        estado, simbolo = izq.split(",")
        transiciones[(estado, simbolo)] = der
    
    return estados, alfabeto, inicio, finales, transiciones

def procesar_cadena(cadena, inicio, finales, transiciones):
    estado = inicio
    for c in cadena:
        if (estado, c) in transiciones:
            estado = transiciones[(estado, c)]
        else:
            return False
    return estado in finales

def main():
    if len(sys.argv) != 3:
        print("Uso: python afd.py config.txt cadenas.txt")
        return
    estados, alfabeto, inicio, finales, transiciones = cargar_config(sys.argv[1])
    with open(sys.argv[2]) as f:
        for linea in f:
            cadena = linea.strip()
            if not cadena: continue
            aceptada = procesar_cadena(cadena, inicio, finales, transiciones)
            print(f"{'acepta' if aceptada else 'NO acepta'}")

if __name__ == "__main__":
    main()

