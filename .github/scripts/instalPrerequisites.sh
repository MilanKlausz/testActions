#!/bin/bash

# $@ is the operating sytem

if [[ $@ == "Linux" ]]; then
  sudo apt-get update 
  sudo apt-get install curl g++ build-essential python3-dev cmake libxerces-c-dev libexpat1-dev gfortran libhdf5-serial-dev git python3-venv python3-pip python3-tk mesa-common-dev libglu1-mesa-dev freeglut3-dev cm-super 
elif [[ $@ == "macOS"  ]]; then
  brew install cmake python python-tk expat xerces-c hdf5 freetype # git
else
  echo "Wrong operating system"
  exit 1 
fi


#TEMP handling dgdepfixer issues - it should be handled in dgdepfixer later on

#general
git config --global user.name "Lorem Ipsum" 
git config --global user.email "dolor.sit@ess.eu"

#Linux only part
if [[ $@ == "Linux" ]]; then
  mv $HOME/.profile $HOME/.profile_DISABLED  
  echo 'if [ -f $HOME/.bashrc ]; then . $HOME/.bashrc; fi' >> $HOME/.bash_profile   
fi
