#!/usr/bin/csh

foreach arg ($*)
switch ($arg)
case "--help":
case "-h":
	echo "Usage: source setup_custom_install_path.csh </absolute/install/path>"
	exit 0
    endsw
end

echo "setting the custom install dir to $1 ..."

setenv NPC_INSTALL_DIR $1
if ($?LIBRARY_PATH) then
  setenv LIBRARY_PATH ${NPC_INSTALL_DIR}/lib:$LIBRARY_PATH
else
  setenv LIBRARY_PATH ${NPC_INSTALL_DIR}/lib
endif

if ($?CPLUS_INCLUDE_PATH) then
  setenv CPLUS_INCLUDE_PATH ${NPC_INSTALL_DIR}/include:$CPLUS_INCLUDE_PATH
else
  setenv CPLUS_INCLUDE_PATH ${NPC_INSTALL_DIR}/include
endif

setenv LD_LIBRARY_PATH ${NPC_INSTALL_DIR}/lib:$LD_LIBRARY_PATH
setenv PATH ${NPC_INSTALL_DIR}/bin:$PATH
