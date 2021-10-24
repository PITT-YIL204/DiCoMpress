import sys
import matplotlib.pyplot as plt
from pydicom import dcmread

ds = dcmread(sys.argv[1],force=True)


plt.imshow(ds.pixel_array, cmap=plt.cm.gray)
plt.show()