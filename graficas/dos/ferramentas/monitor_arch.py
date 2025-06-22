import time
import threading
import requests
import matplotlib.pyplot as plt

url = "http://192.168.238.242/nginx_status"
interval = 0.5  # segundos entre lecturas
timeout = 200
stop_flag = False

timestamps = []
active_connections = []

start_time = time.time()

def esperar_input():
    global stop_flag
    input("Preme [Enter] para deter a monitorización...\n")
    stop_flag = True

# Inicia o fío para permitir parada manual
input_thread = threading.Thread(target=esperar_input)
input_thread.daemon = True
input_thread.start()

print("Iniciando monitorización das conexións activas de Nginx...")

while not stop_flag:
    try:
        response = requests.get(url, timeout=timeout)
        if response.status_code == 200:
            lines = response.text.strip().splitlines()
            active = int(lines[0].split(":")[1].strip())  # Active connections
            elapsed = time.time() - start_time
            timestamps.append(elapsed)
            active_connections.append(active)
            print(f"[{elapsed:.1f}s] Conexións activas: {active}")
        else:
            print("❌ Resposta HTTP non válida.")
            break
    except Exception as e:
        print(f"[{time.time() - start_time:.1f}s] Fallo na conexión: {e}")
        break

    time.sleep(interval)

# Crear gráfica
plt.figure(figsize=(10,6))
plt.plot(timestamps, active_connections, marker='o', linestyle='-', color='navy')
plt.xlabel("Tempo (segundos)", fontsize=12)
plt.ylabel("Conexións activas", fontsize=12)
plt.title("Monitorización de conexións activas en Nginx", fontsize=14)
plt.grid(True)
plt.tight_layout()
plt.savefig("nginx_active_connections.png")
plt.show()
