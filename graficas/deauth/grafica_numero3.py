import matplotlib.pyplot as plt

# Leer el log de paquetes de deautenticación
def leer_log_paquetes(archivo_log):
    tiempos = []
    with open(archivo_log, "r") as log_file:
        for linea in log_file:
            tiempos.append(float(linea.strip()))
    return tiempos

# Agrupar los paquetes por intervalos de tiempo
def agrupar_por_intervalos(tiempos, intervalo=3):
    tiempo_inicial = tiempos[0]
    tiempos_normalizados = [t - tiempo_inicial for t in tiempos]
    max_tiempo = tiempos_normalizados[-1]
    
    # Crear intervalos
    intervalos = range(0, int(max_tiempo) + intervalo, intervalo)
    conteo_paquetes = [0] * len(intervalos)
    
    # Contar paquetes en cada intervalo
    for t in tiempos_normalizados:
        indice = int(t // intervalo)
        if indice < len(conteo_paquetes):
            conteo_paquetes[indice] += 1
    
    return intervalos, conteo_paquetes

# Archivo de log
archivo_log = "log_paquetes.txt"

# Leer el log y obtener los datos
tiempos = leer_log_paquetes(archivo_log)

# Agrupar los paquetes por intervalos de 10 segundos
intervalos, conteo_paquetes = agrupar_por_intervalos(tiempos, intervalo=3)

# Crear la gráfica de líneas
plt.figure(figsize=(10, 6))
plt.plot(intervalos, conteo_paquetes, color="blue", linewidth=2, label="Paquetes de deautenticación")

# Personalizar la gráfica
plt.xlabel("Tiempo (segundos)")
plt.ylabel("Número de paquetes")
plt.title("Frecuencia de paquetes de deautenticación por intervalo de tiempo")
plt.grid(True, linestyle="--", alpha=0.7)
plt.legend(loc="upper left")

# Mostrar la gráfica
plt.show()
