#include "midi.h"
#include "swapBytes.h"

MidiFile::MidiFile() {
	_fileName = NULL;
	_time = 0;
	_track = 0;
}

MidiFile::MidiFile(const char *fileName) {
	_fileName = NULL;
	_time = 0;
	_track = 0;
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

	// #ifdef DEBUG 
	fprintf(stderr, "%s\n", hdr);
	// #endif

	if (strcmp((char*)hdr, "MThd"))
		fprintf(stderr, "%s\n", "Error, bad header");

	// uchar buffer[4];
	// for (int i = 0; i < 4; i++) {
	// 	buffer[4] = getNextChar(midi);
	// }

	// hdrSz = getNext(midi);


	uchar buffer[4];
	for (int i = 0; i < 4; i++) {

		buffer[i] = getNextChar(midi);
		fprintf(stderr, "%x\n", buffer[i]);
	}	

	hdrSz = buffer[3];
	if (hdrSz != 6)
		fprintf(stderr, "%s %lu\n", "Error, bad size", hdrSz);

	type = getNextShort(midi);

	
	fprintf(stderr, "type: %u\n", type);
	int tracks = getNextShort(midi);
	fprintf(stderr, "tracks: %i\n", tracks);
	
	memset(buffer, '\0', 4);
	for (int i = 0; i < 2; i++) {
		buffer[i] = getNextChar(midi);
		fprintf(stderr, "bpm%x\n", buffer[i]);
	}
	
	uchar trkHdr[4];

	for (int i = 0; i < 4; i++) {
		trkHdr[i] = getNextChar(midi);
		fprintf(stderr, "%c\n", trkHdr[i]);
	}

	fprintf(stderr, "%s\n", trkHdr);

	char trkSz[4];

	for (int i = 0; i < 4; i++) {
		trkSz[i] = getNextChar(midi);
		fprintf(stderr, "%x\n", trkSz[i]);
	}

	// fprintf(stderr, "%ld\n", atol((char*)trkSz));
	char c = 0;
	short prevNote = 0;

	while ((c  = getNextChar(midi)) > 0) {
		if (c == 0x8) {
			fprintf(stderr, "%c\n", c);
		} else if (c == 0x9) { //note on
			fprintf(stderr, "%c\n", c);
		}
		fprintf(stderr, "%s\n", "something");
	}
//	for (int i = 0; i < trkSz)
}		

ushort
MidiFile::getNextShort(FILE *f) {
	ushort x;
	fread(&x, 2, 1, f);
	return swapBytes(x);
}

ulong
MidiFile::getNext(FILE *f) {
	ulong x;
	fread(&x, 4, 1, f);
	return swapBytes(x);
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

