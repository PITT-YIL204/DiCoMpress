import sys
import os
from pydicom import dcmread
import numpy as np
# from pydicom.data import get_testdata_file

# fpath = get_testdata_file(sys.argv[1])
ds = dcmread(sys.argv[1])

f = open(os.path.splitext(sys.argv[1])[0]+".mat","w")
f.write(f"{ds.Rows} {ds.Columns}\n")
array = np.frombuffer(ds.PixelData, np.uint16) #we only work with uint16 for demo
armax = np.amax(array)
for i in range(ds.Rows * ds.Columns):
	f.write(f"{array[i]/armax} ")

# f.write(f"\n{armax}")
f.close()

f = open(os.path.splitext(sys.argv[1])[0]+".max","w")
f.write(f"{armax}")