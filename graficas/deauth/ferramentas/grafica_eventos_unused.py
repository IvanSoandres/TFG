import matplotlib.pyplot as plt

# Leer el log de reconexión
def leer_log_reconexion(archivo_log):
    tiempos_reconexion = []
    ultimo_tiempo = None

    with open(archivo_log, "r") as log_file:
        for linea in log_file:
            tiempo, estado = linea.strip().split(" - ")
            tiempo = int(tiempo)
            if estado == "Conectado" and ultimo_tiempo is not None:
                tiempos_reconexion.append(tiempo - ultimo_tiempo)
            ultimo_tiempo = tiempo

    return tiempos_reconexion

# Archivo de log
archivo_log = "log_desconexion.txt"  # Usamos el mismo log de la métrica 1

# Leer el log y obtener los tiempos entre reconexiones
tiempos_reconexion = leer_log_reconexion(archivo_log)

# Graficar la distribución de tiempos entre reconexiones
plt.hist(tiempos_reconexion, bins=5, edgecolor='black', color='green')
plt.xlabel('Tiempo entre reconexiones (segundos)')
plt.ylabel('Frecuencia')
plt.title('Distribución de tiempos de reconexión')
plt.show()

