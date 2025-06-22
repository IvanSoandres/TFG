import pandas as pd
import matplotlib.pyplot as plt

# Cargar datos
df = pd.read_csv("conexions_apache.csv", sep=';')

# Asegurar que non haxa valores negativos
df["conexions_activas"] = df["conexions_activas"].clip(lower=0)

# Crear gráfica
plt.figure(figsize=(10, 6))
plt.plot(df["tempo_segundos"], df["conexions_activas"], marker='o', linestyle='-')

# Etiquetas en galego
plt.title("Conexións activas ao Apache2 ao longo do tempo")
plt.xlabel("Tempo transcorrido (segundos)")
plt.ylabel("Número de conexións activas")
plt.grid(True)

# Gardar ou amosar a gráfica
plt.savefig("grafica_conexions_apache.png")
plt.show()
