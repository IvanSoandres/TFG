#!/bin/bash

# Archivo temporal donde se guardarán los datos
DATA_FILE="connections.dat"
rm -f $DATA_FILE  # Elimina el archivo si existe

echo "Monitoreando conexiones... (Presiona Ctrl+C para detener)"

# Iniciar el monitoreo en tiempo real
while true; do
    TIMESTAMP=$(date +%s)  # Obtener el tiempo actual (epoch)
    CONNECTIONS=$(ss -tan state established '( sport = :80 or sport = :443 )' | wc -l)  # Contar conexiones activas
    echo "$TIMESTAMP $CONNECTIONS" >> $DATA_FILE  # Guardar datos en archivo
    sleep 1  # Esperar 1 segundo antes de la próxima medición
done

