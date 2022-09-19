from iov_insert import *

# Parameters that must be specified:
GT_NAME = "OnlineGT"              # The Global Tag name
CALIB_TYPE = "Beam"               # The calibration type 
CALIB_FILE = "MyNewBeamFile.root" # The calibration file to upload
IOV_START = 4                     # Start of validity range for this calibration (inclusive of this)
# IOV_END is ignored for the online use case

iov_insert(GT_NAME, CALIB_TYPE, CALIB_FILE, IOV_START)
