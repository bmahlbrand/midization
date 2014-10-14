#include "midi.h"

MidiFile::Midifile() {

}

MidiFile::MidiFile(const char *fileName) {

}

MidiFile::~MidiFile() {

}

int
MidiFile::openFile(FILE &file, const char *fileName) {
	file = fopen(fileName, "r");

	if (file)
		return 1;
	return 0;
}

void 
MidiFile::setFileName(const char *fileName) {
	_fileName = strdup(fileName);
}

int
MidiFile::read(const char *fileName) {
	if (fileName != NULL)
		setFileName(fileName);
	FILE *file = NULL;
	
	if (!openFile(file, fileName))
		return 0;

	ulong hdr;
	ushort type;
	uchar dataByte;

	
}



MidiFile::isNoteOn(int track, int i) {

}

MidiFile::isNoteOff(int track, int i) {

}

MidiFile::isMeta(int track, int i) {

}

MidiFile::getTrack(int track, int i) {

}

