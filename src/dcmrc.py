import sys
import os
import json
import pydicom
from pydicom.dataset import Dataset, FileDataset
from pydicom.uid import ExplicitVRLittleEndian
import pydicom._storage_sopclass_uids
import numpy as np

f = open(sys.argv[1]+".mat","r")
[rows,cols] = f.readline().split();
arr = list(map(lambda x: float(x), f.readline().split()))
image2d = np.array(arr, dtype=float)
f.close()

f = open(sys.argv[2]+".max","r")
vmax = int(f.readline().strip())
image2d -= np.amin(image2d)
image2d *= vmax
image2d = np.ceil(image2d).astype(np.uint16)

meta = pydicom.Dataset()
meta.MediaStorageSOPClassUID = pydicom._storage_sopclass_uids.MRImageStorage
meta.MediaStorageSOPInstanceUID = pydicom.uid.generate_uid()
meta.TransferSyntaxUID = pydicom.uid.ExplicitVRLittleEndian  

ds = Dataset()
ds.file_meta = meta

ds.is_little_endian = True
ds.is_implicit_VR = False
ds.Rows = rows
ds.Columns = cols


f = open(sys.argv[2]+".meta","r")
lines = f.readlines()
for line in lines[:-2]:
    [attr,valstr] = line.split(" ",1)
    setattr(ds,attr,json.loads(valstr))

pydicom.dataset.validate_file_meta(ds.file_meta, enforce_standard=True)

ds.PixelData = image2d.tobytes()

dn = os.path.dirname(sys.argv[1])
bn = os.path.basename(sys.argv[1])
ds.save_as(os.path.join(dn,"decoded",bn+".dcm"))