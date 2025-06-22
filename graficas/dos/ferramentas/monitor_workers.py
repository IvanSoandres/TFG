import time
import threading
import requests
import matplotlib.pyplot as plt

url = "http://192.168.238.242/server-status?auto"
interval = 0.5  # segundos entre lecturas
timeout = 700

timestamps = []
busy_workers = []
start_time = time.time()
stop_flag = False

def esperar_input():
    global stop_flag
    input("Preme [Enter] en calquera momento para deter a monitorización...\n")
    stop_flag = True

# Iniciar o fío que espera a entrada do usuario
input_thread = threading.Thread(target=esperar_input)
input_thread.daemon = True
input_thread.start()

print("📡 Iniciando monitorización de Apache...")

while not stop_flag:
    try:
        response = requests.get(url, timeout=timeout)
        if response.status_code == 200:
            lines = response.text.splitlines()
            busy = next(int(line.split(":")[1].strip()) for line in lines if line.startswith("BusyWorkers"))
            elapsed = time.time() - start_time
            timestamps.append(elapsed)
            busy_workers.append(busy)
            print(f"[{elapsed:.1f}s] BusyWorkers: {busy}")
        else:
            print("❌ Resposta HTTP non válida.")
            break
    except Exception as e:
        print(f"[{time.time() - start_time:.1f}s] Fallo na conexión: {e}")
        break

    time.sleep(interval)

# Crear gráfica
plt.figure(figsize=(10, 6))
plt.plot(timestamps, busy_workers, marker='o', linestyle='-', color='crimson')
plt.xlabel("Tempo (segundos)", fontsize=12)
plt.ylabel("Traballadores ocupados (BusyWorkers)", fontsize=12)
plt.title("Escalada de uso dos workers de Apache ata o colapso", fontsize=14)
plt.grid(True)
plt.tight_layout()
plt.savefig("apache_busyworkers_galego.png")
plt.show()
