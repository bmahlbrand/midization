#include "midi.h"
#include "swapBytes.h"

MidiFile::MidiFile() {
	_fileName = NULL;
	_trkSz = 0;
	_bpm = 0;
	
}

MidiFile::MidiFile(const char *fileName) {
	_fileName = NULL;
	_trkSz = 0;
	_bpm = 0;
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

int isEvent(uchar event) {
	uchar e = event & 0x0F;
	// fprintf(stderr, "%c\n", e);
	switch(e) {
		case 0x8: //note off
			fprintf(stderr, "%s\n", "-");
			break;
		case 0x9: //note on
			fprintf(stderr, "%s\n", "+");
			break;
		case 0xA: //note aftertouch
			fprintf(stderr, "%s\n", "0xA");
			break;
		case 0xB: //controller
			fprintf(stderr, "%s\n", "0xB");
			break;
		case 0xC: //program change
			fprintf(stderr, "%s\n", "0xC");
			break;
		case 0xD: //channel aftertouch
			fprintf(stderr, "%s\n", "0xD");
			break;
		case 0xE: //pitch bend
			fprintf(stderr, "%s\n", "0xE");
			break;
		case 0xFF: //metaEvent
			fprintf(stderr, "%s\n", "0xFF");
			break;
		default:
			fprintf(stderr, "%s\n", "ERROR: bad event");
			break;
	}
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
	
	if (buffer[0] == 0)
		_bpm = buffer[1];
	fprintf(stderr, "%i\n", _bpm);
	uchar trkHdr[4];

	for (int i = 0; i < 4; i++) {
		trkHdr[i] = getNextChar(midi);
	}

	fprintf(stderr, "%s\n", trkHdr);

	char trkSz[4];
	ulong size = 0;
	for (int i = 0; i < 4; i++) {
		trkSz[i] = getNextChar(midi);
		fprintf(stderr, "%x\n", trkSz[i]);
		size += trkSz[i];
	}

	size = (trkSz[0] << 24) | (trkSz[1] << 16) | (trkSz[2] << 8) | trkSz[3]; //from char hex array to ulong
	fprintf(stderr, "%lu\n", size);
	// fprintf(stderr, "%ld\n", atol((char*)trkSz));
	char c = 0;
	short prevNote = 0;

	int count = 0;
	// while ((c = getNextChar(midi)) != EOF) {
	for (int i = 0; i < size; i++) {
		c = getNextChar(midi);
		isEvent(c);
		if ((c & 0x0F) == 0x8) {
			// fprintf(stderr, "%c\n", '-');
		} else if ((c & 0x0F) == 0x9) { //note on
			if (prevNote > c) {
				// fprintf(stderr, "%s\n", "up");
				// contour[count-1] = 'U';
				_contour.push_back('U');
			} else if (prevNote < c && count > 0) {
				// fprintf(stderr, "%s\n", "down");
				// contour[count-1] = 'D';
				_contour.push_back('D');
			} else {
				// fprintf(stderr, "%s\n", "same");
				// contour[count-1] = 'S';

				_contour.push_back('S');

			}
			count++;
			prevNote = c;
			// int tmp = getNextChar(midi);
			// fprintf(stderr, "note: %i\n", tmp);
			// fprintf(stderr, "%c\n", '+');
		}
	}
	fprintf(stderr, "%i\n", count);
	fclose(midi);
   	return 0;
}		

int
MidiFile::setTrackSize(ulong sz) {
	_trkSz = sz;
	return 0;
}

int
MidiFile::setBPM(ushort bpm) {
	_bpm = bpm;
	return 0;
}

void
MidiFile::printContour() {
	for( std::vector<uchar>::const_iterator i = _contour.begin(); i != _contour.end(); ++i)
   		fprintf(stderr, "%c", *i);
   	fprintf(stderr, "%c\n", '\n');
}
// readEvent(FILE *f) {
// 	char c = getNextChar(f);
// 	uchar evtType = c & 0x0F;

// 	// switch(evtType) {
// 	// 	case 0x09:

// 	// 		break;
// 	// 	case 0x08:

// 	// 		break;
// 	// }
// }

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

void
Note::printNote() {

}