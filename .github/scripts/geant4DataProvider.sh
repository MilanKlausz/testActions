#!/bin/bash

 ## https://cern.ch/geant4-data/datasets/G4NDL.4.5.tar.gz
 ## https://cern.ch/geant4-data/datasets/G4EMLOW.7.3.tar.gz
 ## https://cern.ch/geant4-data/datasets/G4PhotonEvaporation.5.2.tar.gz
 ## https://cern.ch/geant4-data/datasets/G4RadioactiveDecay.5.2.tar.gz
 ## https://cern.ch/geant4-data/datasets/G4NEUTRONXS.1.4.tar.gz
 ## https://cern.ch/geant4-data/datasets/G4PII.1.3.tar.gz
 ## https://cern.ch/geant4-data/datasets/G4RealSurface.2.1.1.tar.gz
 ## https://cern.ch/geant4-data/datasets/G4SAIDDATA.1.1.tar.gz
 ## https://cern.ch/geant4-data/datasets/G4ABLA.3.1.tar.gz
 ## https://cern.ch/geant4-data/datasets/G4ENSDFSTATE.2.2.tar.gz

## declare an array variable
declare -a arrName=("G4NDL.4.5" 
                    "G4EMLOW.7.3"
                    "G4PhotonEvaporation.5.2"
                    "G4RadioactiveDecay.5.2"
                    "G4NEUTRONXS.1.4"
                    "G4PII.1.3"
                    "G4RealSurface.2.1.1"
                    "G4SAIDDATA.1.1"
                    "G4ABLA.3.1" 
                    "G4ENSDFSTATE.2.2"
                    )

declare -a unTarName=("G4NDL4.5" 
                      "G4EMLOW7.3"
                      "PhotonEvaporation5.2"
                      "RadioactiveDecay5.2"
                      "G4NEUTRONXS1.4"
                      "G4PII1.3"
                      "RealSurface2.1.1"
                      "G4SAIDDATA1.1"
                      "G4ABLA3.1" 
                      "G4ENSDFSTATE2.2"
                      )

BASE_URL="https://cern.ch/geant4-data/datasets/"

#DATA_DIR="dgdepfixer_install/extras/geant4-10.04.p03/share/Geant4-10.4.3/data"
TRUE_DATA_DIR="Geant4data"
TAR_FILE_DIR="Geant4data_tar_files"

if [[ ! -e "$HOME/$TAR_FILE_DIR" ]]; then 
   mkdir "$HOME/$TAR_FILE_DIR" 
fi

if [[ ! -e "$HOME/$TRUE_DATA_DIR" ]]; then 
   mkdir "$HOME/$TRUE_DATA_DIR" 
fi

#if [[ ! -e "$HOME/$DATA_DIR" ]]; then 
#   mkdir "$HOME/$DATA_DIR" 
#fi

for i in "${!arrName[@]}"
do
   if [[ ! -d "$HOME/$TRUE_DATA_DIR/${unTarName[i]}" ]]; then 
      echo "$BASE_URL${arrName[i]}.tar.gz"
      curl -L "$BASE_URL${arrName[i]}.tar.gz" > "$HOME/$TAR_FILE_DIR/${arrName[i]}.tar.gz"
      tar -xf "$HOME/$TAR_FILE_DIR/${arrName[i]}.tar.gz" -C "$HOME/$TRUE_DATA_DIR"
   else 
      echo "$HOME/$TRUE_DATA_DIR/${unTarName[i]} exists"
   fi
done

#ln -s -f "$HOME/$TRUE_DATA_DIR/"* "$HOME/$DATA_DIR/"

