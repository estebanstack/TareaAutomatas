import sys   # Para leer los parámetros desde la terminal

# -------------------------
# FUNCION: cargar_config
# Lee el archivo config.txt y devuelve la quíntupla (Q, Σ, δ, q0, F)
# -------------------------
def cargar_config(archivo):
    with open(archivo) as f:
        # strip() quita espacios/saltos de línea
        # ignora líneas vacías o que empiecen con "#"
        lineas = [l.strip() for l in f if l.strip() and not l.startswith("#")]
    
    # saca cada parte de la quíntupla
    estados = lineas[0].split(":")[1].split(",")     # Q = {q0,q1,q2}
    alfabeto = lineas[1].split(":")[1].split(",")    # Σ = {0,1}
    inicio = lineas[2].split(":")[1]                 # q0
    finales = lineas[3].split(":")[1].split(",")     # F = {q2}

    # δ: transiciones
    transiciones = {}
    for l in lineas[5:]:   # desde "transitions:" en adelante
        izq, der = l.split("->")     # ej: "q0,0" -> "q1"
        estado, simbolo = izq.split(",")  # separa origen y símbolo
        transiciones[(estado, simbolo)] = der  # guarda δ(origen,simbolo)=destino
    
    return estados, alfabeto, inicio, finales, transiciones

# -------------------------
# FUNCION: procesar_cadena
# Simula el AFD sobre una cadena dada
# -------------------------
def procesar_cadena(cadena, inicio, finales, transiciones, alfabeto):
    estado = inicio  # empezamos en q0
    for c in cadena:
        if c not in alfabeto:   # si símbolo no pertenece a Σ, rechaza
            return False
        if (estado, c) in transiciones:   # si hay transición definida
            estado = transiciones[(estado, c)]  # avanza al siguiente estado
        else:
            return False   # transición no definida → rechazo
    return estado in finales   # acepta si terminó en estado final

# -------------------------
# PROGRAMA PRINCIPAL
# -------------------------
def main():
    if len(sys.argv) != 3:
        print("Uso: python afd.py config.txt cadenas.txt")
        return

    # cargar quíntupla
    estados, alfabeto, inicio, finales, transiciones = cargar_config(sys.argv[1])
    
    # abrir archivo de cadenas
    with open(sys.argv[2]) as f:
        for linea in f:
            cadena = linea.strip()
            if not cadena: continue  # ignorar vacías
            aceptada = procesar_cadena(cadena, inicio, finales, transiciones, alfabeto)
            # salida
            print("acepta" if aceptada else "NO acepta")

if __name__ == "__main__":
    main()
