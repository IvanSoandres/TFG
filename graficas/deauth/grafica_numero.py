import matplotlib.pyplot as plt
from datetime import datetime

# Leer el log de paquetes de deautenticación
def leer_log_paquetes(archivo_log, intervalo=10):
    tiempos = []
    with open(archivo_log, "r") as log_file:
        for linea in log_file:
            tiempos.append(float(linea.strip()))

    # Agrupar por intervalos de tiempo
    conteo_paquetes = {}
    for tiempo in tiempos:
        intervalo_tiempo = int(tiempo // intervalo) * intervalo
        if intervalo_tiempo in conteo_paquetes:
            conteo_paquetes[intervalo_tiempo] += 1
        else:
            conteo_paquetes[intervalo_tiempo] = 1

    return sorted(conteo_paquetes.items())

# Archivo de log
archivo_log = "log_paquetes.txt"

# Leer el log y obtener los datos
intervalos, paquetes = zip(*leer_log_paquetes(archivo_log))

# Graficar los paquetes por intervalo
plt.bar(intervalos, paquetes, width=8, color='blue')
plt.xlabel('Tempo (segundos)')
plt.ylabel('Número de paquetes')
plt.title('Paquetes de deautenticación por intervalo')
plt.show()
