#!/bin/bash

TRUE_DATA_DIR="Geant4data"

export G4NEUTRONHPDATA="`cd $HOME/$TRUE_DATA_DIR/G4NDL4.5 > /dev/null ; pwd`"
export G4LEDATA="`cd $HOME/$TRUE_DATA_DIR/G4EMLOW7.3 > /dev/null ; pwd`"
export G4LEVELGAMMADATA="`cd $HOME/$TRUE_DATA_DIR/PhotonEvaporation5.2 > /dev/null ; pwd`"
export G4RADIOACTIVEDATA="`cd $HOME/$TRUE_DATA_DIR/RadioactiveDecay5.2 > /dev/null ; pwd`"
export G4NEUTRONXSDATA="`cd $HOME/$TRUE_DATA_DIR/G4NEUTRONXS1.4 > /dev/null ; pwd`"
export G4PIIDATA="`cd $HOME/$TRUE_DATA_DIR/G4PII1.3 > /dev/null ; pwd`"
export G4REALSURFACEDATA="`cd $HOME/$TRUE_DATA_DIR/RealSurface2.1.1 > /dev/null ; pwd`"
export G4SAIDXSDATA="`cd $HOME/$TRUE_DATA_DIR/G4SAIDDATA1.1 > /dev/null ; pwd`"
export G4ABLADATA="`cd $HOME/$TRUE_DATA_DIR/G4ABLA3.1 > /dev/null ; pwd`"
export G4ENSDFSTATEDATA="`cd $HOME/$TRUE_DATA_DIR/G4ENSDFSTATE2.2 > /dev/null ; pwd`"