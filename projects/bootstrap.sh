#!/bin/bash

#Location where dg_code_framework is checked out (modify it if you use a
#different location):
DGCODE_FMWK_DIR="$GITHUB_WORKSPACE/dgcode_framework"

#Setup locations for where to keep your own project packages (the magic code
#below defaults this to being below the directory of this bootstrap file):
export DGCODE_PROJECTS_DIR="$( cd -P "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

#Expert users might even want to override where to put build output or the final
#installation area (replace "auto" with an actual path if desired):
#export DGCODE_INSTALL_DIR="/Users/milanklausz/devDgcode/inst"
export DGCODE_INSTALL_DIR="auto"
export DGCODE_BUILD_DIR="auto"

#List paths to directories containing packages you want to be built along with
#the Framework and Project packages. (This may be useful for e.g., dependencies
#in large legacy repositories) 
export DGCODE_PKG_PATH="$GITHUB_WORKSPACE/dgcode_validation"

#Finish up by sourcing the main bootstrap.sh file from the dgcode framework:
. $DGCODE_FMWK_DIR/bootstrap.sh