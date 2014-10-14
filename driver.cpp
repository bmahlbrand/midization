#include "midi.h"

int main (int argc, char **argv) {
	const char *fileName = strdup(argv[1]);
	
	MidiFile *midi = new MidiFile(fileName);
	midi->read(fileName);
	return 0;
}