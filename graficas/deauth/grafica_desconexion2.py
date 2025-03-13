import matplotlib.pyplot as plt

# Leer el log de desconexión
def leer_log_desconexion(archivo_log):
    tiempo_conectado = 0
    tiempo_desconectado = 0
    ultimo_estado = None
    ultimo_tiempo = None

    with open(archivo_log, "r") as log_file:
        for linea in log_file:
            partes = linea.strip().split(" - ")
            if len(partes) < 2:
                continue

            timestamp, estado = partes
            timestamp = int(timestamp)

            if ultimo_tiempo is not None:
                delta_tiempo = timestamp - ultimo_tiempo
                if ultimo_estado == "Conectado":
                    tiempo_conectado += delta_tiempo
                elif ultimo_estado == "Desconectado":
                    tiempo_desconectado += delta_tiempo

            ultimo_estado = estado
            ultimo_tiempo = timestamp

    return tiempo_conectado, tiempo_desconectado

# Archivo de log
archivo_log = "log_desconexion.txt"

# Leer el log y obtener los tiempos
tiempo_conectado, tiempo_desconectado = leer_log_desconexion(archivo_log)

# Datos para la gráfica de tarta
etiquetas = ["Conectado", "Desconectado"]
valores = [tiempo_conectado, tiempo_desconectado]
colores = ["green", "red"]
explode = (0, 0.1)  # Resaltar el sector "Desconectado"

# Crear la gráfica de tarta
plt.figure(figsize=(8, 6))
plt.pie(valores, explode=explode, labels=etiquetas, colors=colores, autopct='%1.1f%%', startangle=140, shadow=True)
plt.title("Tempo de conexión vs desconexión durante o ataque")
plt.axis("equal")  # Para que la gráfica sea un círculo perfecto

# Mostrar la gráfica
plt.show()
