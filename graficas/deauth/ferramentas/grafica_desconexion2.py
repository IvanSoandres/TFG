import matplotlib.pyplot as plt

# Leer el log de desconexión
def leer_log_desconexion(archivo_log):
    tiempos = []
    estados = []

    with open(archivo_log, "r") as log_file:
        for linea in log_file:
            partes = linea.strip().split(" - ")
            if len(partes) < 2:
                continue

            timestamp, estado = partes
            timestamp = int(timestamp)

            tiempos.append(timestamp)
            estados.append(1 if estado == "Conectado" else 0)  # 1 = Conectado, 0 = Desconectado

    return tiempos, estados

# Archivo de log
archivo_log = "log_desconexion.txt"

# Leer el log y obtener los datos
tiempos, estados = leer_log_desconexion(archivo_log)

# Normalizar los tiempos para que comiencen en 0
tiempo_inicial = tiempos[0]
tiempos_normalizados = [t - tiempo_inicial for t in tiempos]

# Calcular la duración total del ataque
duracion_total = tiempos_normalizados[-1]

# Crear la gráfica de líneas
plt.figure(figsize=(10, 6))
plt.plot(tiempos_normalizados, estados, drawstyle="steps-post", color="blue", linewidth=2, label="Estado de conexión")

# Personalizar la gráfica
plt.yticks([0, 1], ["Desconectado", "Conectado"])  # Etiquetas para el eje Y
plt.xlabel("Tempo (segundos)")
plt.ylabel("Estado de conexión")
plt.title(f"Estado de conexión durante o ataque (Duración total: {duracion_total} segundos)")
plt.grid(True, linestyle="--", alpha=0.7)
plt.legend(loc="upper right")

# Ajustar los límites del eje X
plt.xlim(0, duracion_total)

# Mostrar la gráfica
plt.show()
