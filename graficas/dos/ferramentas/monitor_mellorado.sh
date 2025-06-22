#!/bin/bash

# Nome do ficheiro de saída
OUTFILE="conexions_apache.csv"
echo "tempo_segundos;conexions_activas" > "$OUTFILE"

INICIO=$(date +%s)

while true; do
  AGORA=$(date +%s)
  TEMPO=$((AGORA - INICIO))

  # Contar conexións activas no porto 80 (HTTP)
  CONEXIONS=$(ss -Htan state established '( sport = :80 )' | wc -l)

  echo "$TEMPO;$CONEXIONS" >> "$OUTFILE"

  # Esperar 5 segundos entre medicións
  sleep 5
done
