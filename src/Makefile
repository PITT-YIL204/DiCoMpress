CC := gcc
INCLUDE := -I../include
LIB := -L../lib
OPTIMIZE := -O2 
WARNINGS := -Wall -Wextra -Werror -pedantic
CSTD := -std=c11
DYNAMIC := -fPIC -static -static-libgcc -static-libstdc++
CFLAG := $(INCLUDE) $(LIB) $(WARNINGS) $(OPTIMIZE) $(CSTD)
VSLIB := -lvapoursynth -lvsscript


exports := encode.exe decode.exe decmat.exe libvsmat.dll encode.vpy image.vpy dcmrc.py write_mat.py save_meta.py dcmshow.py
.PHONY: all encoder decoder

all: encoder decoder cexport
	cexport $(exports)
	@echo done

encoder: libvsmat encode

decoder: decmat decode

libvsmat: vsmat.c
	$(CC) $(CFLAG) $(DYNAMIC) $< -o $@.dll -s -shared "-Wl,--subsystem,windows"

encode: encode.o
	$(CC) $(CFLAG) $< -o $@

decode: decode.o
	$(CC) $(CFLAG) $< -o $@

decmat: decmat.o
	$(CC) $(CFLAG) $< $(VSLIB) -o $@

cexport: export.o
	$(CC) $(CFLAG) $< -o $@

%.o: %.c
	$(CC) $(CFLAG) -c $< -o $@

echo: 
	echo $(CFLAG)

clean:
	rm -f *.o
	rm -f exports
	rm -f cexport.exe