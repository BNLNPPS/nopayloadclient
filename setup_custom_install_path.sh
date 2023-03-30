#!/bin/bash

if [[ ( $@ == "--help") ||  $@ == "-h" || ($# != 1)]]
then
	echo "Usage: source $0 </absolute/install/path>"
	exit 0
fi

echo "setting the custom install dir to $1 ..."

export NPC_INSTALL_DIR=$1
export LIBRARY_PATH=${NPC_INSTALL_DIR}/lib:$LIBRARY_PATH
export CPLUS_INCLUDE_PATH=${NPC_INSTALL_DIR}/include:$CPLUS_INCLUDE_PATH
export PATH=${NPC_INSTALL_DIR}/bin:$PATH
