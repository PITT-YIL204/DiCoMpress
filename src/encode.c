#include <stdlib.h>
#include <stdio.h>

int main(void) {
	system("mkdir encoded");
	system("vspipe.exe -c y4m encode.vpy raw");
	system("x265 --asm avx2 --y4m -D 12 --no-open-gop --keyint -1 --preset placebo --deblock -1:-1 --ctu 32 --qg-size 8 --me 3 --subme 4 --merange 40 --b-intra --no-rect --no-amp --ref 5 --weightb --bframes 7 --rc-lookahead 66 --rd 5 --no-sao --crf 13.0 --aq-mode 1 --aq-strength 0.8 --psy-rdoq 1.0 --rdoq-level 2 --pbratio 1.1 --cbqpoffs -2 --crqpoffs -2 --qcomp 0.66 --no-strong-intra-smoothing  --output \"chain.hevc\" raw");
	remove("raw");
	system("mv chain.hevc ./encoded/chain.hevc");
	system("mv *.max ./encoded");
	system("rm *.mat");

	return 0;
}