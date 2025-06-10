#!/bin/bash

# Archivo de log
log_file="log_desconexion.txt"
echo "" > $log_file  # Limpiar el archivo de log

# Dirección IP del router (para verificar conectividad)
router_ip="192.168.196.50"  # Cambia esto por la IP de tu router

# Función para verificar si el equipo está conectado a la WiFi
verificar_conexion() {
    # Verificar si la interfaz WiFi está asociada a un ESSID
    if iw dev wlan0 link | grep -q "Connected to"; then
        # Verificar conectividad con el router
        if ping -c 1 -W 1 $router_ip > /dev/null 2>&1; then
            echo "Conectado"
        else
            echo "Sin conectividad"
        fi
    else
        echo "Desconectado"
    fi
}

# Bucle principal
while true; do
    estado=$(verificar_conexion)
    timestamp=$(date +%s)
    echo "$timestamp - $estado" >> $log_file

    # Si el estado es "Desconectado", verificar si fue por deautenticación
    if [[ $estado == "Desconectado"* ]]; then
        # Capturar tráfico WiFi para detectar paquetes de deautenticación
        timeout 5 tcpdump -i wlan0 -c 1 -w /tmp/deauth.pcap 'wlan.fc.type_subtype == 0x000c' > /dev/null 2>&1
        if [ $? -eq 0 ]; then
            echo "$timestamp - Desconexión por deautenticación" >> $log_file
        fi
    fi

    sleep 1  # Verificar cada segundo
done
