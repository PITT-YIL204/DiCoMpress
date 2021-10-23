import vapoursynth as vs
import os
import subprocess
import time

core = vs.core
core.max_cache_size = 30000
core.num_threads = 8

dir = r"./"

res = None
for root, dirs, files in os.walk(dir):
    for file in files:
        if file.endswith('.dcm'):
            basename = os.path.splitext(file)
            dirname = os.path.dirname(file)
            matname = basename[0]+".mat"
            metaname = os.path.join(dirname,'encoded',os.path.basename(basename[0])+".meta")
            dcmname = os.path.join(dir,file)
            subprocess.run(["py", "write_mat.py", dcmname, matname])
            subprocess.run(["py", "save_meta.py", dcmname, metaname])
            if res == None:
                res = core.MAT.matread(matfile=matname)
            else:
                res += core.MAT.matread(matfile=matname)
            
            
#
#c1 = core.MAT.matread(matfile=m1)
#c2 = core.MAT.matread(matfile=m2)
#res = c1 + c2
res = core.fmtc.bitdepth(res,bits=12,flt=0)

#show(core, res, 0)
res.set_output()