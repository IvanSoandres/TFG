import matplotlib.pyplot as plt
from datetime import datetime

# Leer el log de paquetes de deautenticación
def leer_log_paquetes(archivo_log):
    tiempos = []
    with open(archivo_log, "r") as log_file:
        for linea in log_file:
            tiempos.append(float(linea.strip()))
    return tiempos

# Archivo de log
archivo_log = "log_paquetes.txt"

# Leer el log y obtener los datos
tiempos = leer_log_paquetes(archivo_log)

# Normalizar los tiempos para que comiencen en 0
tiempo_inicial = tiempos[0]
tiempos_normalizados = [t - tiempo_inicial for t in tiempos]

# Crear la gráfica
plt.figure(figsize=(10, 6))
plt.hist(tiempos_normalizados, bins=20, color="red", edgecolor="black")
plt.xlabel("Tempo (segundos)")
plt.ylabel("Número de paquetes de deautenticación")
plt.title("Frecuencia de paquetes de deautenticación durante o ataque")
plt.grid(True, linestyle="--", alpha=0.7)

# Mostrar la gráfica
plt.show()
