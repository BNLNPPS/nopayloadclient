#!/bin/bash

if [[ ( $@ == "--help") ||  $@ == "-h" || ($# != 1)]]
then
	echo "Usage: source $0 </absolute/install/path>"
	exit 0
fi

echo "setting the custom install dir to $1 ..."

NPC_INSTALL_DIR=$1
LIBRARY_PATH=${NPC_INSTALL_DIR}/lib:$LIBRARY_PATH_PATH
CPLUS_INCLUDE_PATH=${NPC_INSTALL_DIR}/include:$CPLUS_INCLUDE_PATH
PATH=${NPC_INSTALL_DIR}/bin:$PATH
