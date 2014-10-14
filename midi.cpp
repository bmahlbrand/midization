#include "midi.h"

MidiFile::MidiFile() {

}

MidiFile::MidiFile(const char *fileName) {
	_fileName = NULL;
	time = 0;
	track = 0;
}

MidiFile::~MidiFile() {

}

FILE *
MidiFile::openFile(const char *fileName) {
	FILE *file = fopen(fileName, "r");

	if (file)
		return file;
	return NULL;
}

void 
MidiFile::setFileName(const char *fileName) {
	_fileName = strdup(fileName);
}

int
MidiFile::read(const char *fileName) {
	if (fileName != NULL)
		setFileName(fileName);
	FILE *midi = openFile(fileName);
	if (!midi)
		return 0;

	ulong hdrSz;
	ushort type;
	uchar dataByte;
	uchar hdr[4];

	for (int i = 0; i < 4; i++) {
		hdr[i] = getNextChar(midi);
	}

	fprintf(stderr, "%s\n", hdr);
}


char
MidiFile::getNextChar(FILE *f) {
	return fgetc(f);
}

int
MidiFile::isNoteOn(int track, int i) {

}

int
MidiFile::isNoteOff(int track, int i) {

}

int
MidiFile::isMeta(int track, int i) {

}

int
MidiFile::getTrack(int track, int i) {

}

