from iov_insert import *

# Parameters that must be specified:
GT_NAME = "PromptGT"              # The Global Tag name
CALIB_TYPE = "Beam"               # The calibration type 
CALIB_FILE = "MyNewBeamFile.root" # The calibration file to upload
IOV_START = 5                     # Start of validity range for this calibration (inclusive of this)
IOV_END = 6                       # End of validatity range for this calibration (exclusive of this, i.e. start of next calibration == end of this one)

iov_insert(GT_NAME, CALIB_TYPE, CALIB_FILE, IOV_START, IOV_END)
