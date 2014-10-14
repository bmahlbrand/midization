#include "midi.h"

int main (int argc, char **argv) {
	const char *fileName = strdup(argv[1]);
	int n = 1;
	// little endian if true
	if(*(char *)&n == 1) {fprintf(stderr, "%s\n", "little");}

	MidiFile *midi = new MidiFile(fileName);
	midi->read(fileName);
	midi->printContour();
	return 0;
}