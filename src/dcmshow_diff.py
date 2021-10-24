import sys
import matplotlib.pyplot as plt
from pydicom import dcmread
import numpy as np

ds1 = dcmread(sys.argv[1],force=True)
ds2 = dcmread(sys.argv[2],force=True)

a1 = ds1.pixel_array.astype(float)
a2 = ds2.pixel_array.astype(float)

diff = np.absolute(a1-a2)
print(f"maximum percent difference: {np.amax(diff) / np.amax(ds1.pixel_array)}")

plt.imshow(diff, cmap=plt.cm.gray)
plt.show()