#!/bin/bash

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <nom_manga> <numero_volume> <page_debut>"
    exit 1
fi

nom_manga="$1"
numero_volume="$2"
page_debut="$3"

if [ ! -d "images/${nom_manga}-${numero_volume}" ]; then
    mkdir -p "images/${nom_manga}-${numero_volume}"
fi

echo "Téléchargement du volume $numero_volume du manga $nom_manga à partir de la page $page_debut"

for ((i=0; i<30; i++)); do
    num_page=$(printf "%03d" $((page_debut + i)))
    ./bin/dl-im "http://opfrcdn.xyz/uploads/manga/$nom_manga/chapters/Volume%20$numero_volume/${num_page}.jpg" "images/${nom_manga}-${numero_volume}/${num_page}"
done
