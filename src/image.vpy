import vapoursynth as vs

core = vs.core
core.max_cache_size = 40000

a=r'encoded\chain.hevc'


if not("strname" in globals()):
	strname = a



#src = core.lsmas.LWLibavSource(source=strname)
src = core.ffms2.Source(strname)

src = core.fmtc.bitdepth(src,bits=32,flt=1)
    
src.set_output()