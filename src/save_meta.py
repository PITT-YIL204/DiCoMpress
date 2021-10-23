import sys
import json
import pydicom
from pydicom import dcmread

ds = dcmread(sys.argv[1])
f = open(sys.argv[2],"w")
# print(dir(ds))

for attr in [attr for attr in dir(ds) if not callable(getattr(ds, attr)) and not attr.startswith("__") and attr != 'PixelData' and attr != 'pixel_array']:
	value = getattr(ds, attr)
	# print(f"{attr} {value}")

	if isinstance(value, pydicom.multival.MultiValue):
		value = list(value)
	elif isinstance(value, pydicom.valuerep.PersonName):
		value = str(value)

	f.write(f"{attr} {json.dumps(value)}\n")
	# print(json.dumps(getattr(ds, attr)))

f.close()