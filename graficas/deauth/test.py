import matplotlib.pyplot as plt

# Datos de ejemplo (tiempos entre reconexiones)
tiempos = [2, 3, 2, 4, 5, 3, 2]  # Tiempos en segundos

plt.hist(tiempos, bins=5, edgecolor='black')
plt.xlabel('Tiempo entre reconexiones (segundos)')
plt.ylabel('Frecuencia')
plt.title('Distribución de tiempos de reconexión')
plt.show()
