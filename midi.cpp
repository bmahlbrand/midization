#include "midi.h"
#include "swapBytes.h"

MidiFile::MidiFile() {
	_fileName = NULL;
	_trkSz = 0;
}

MidiFile::MidiFile(const char *fileName) {
	_fileName = NULL;
	_trkSz = 0;
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

// int 
// isEvent(uchar event) {
// 	uchar e = event & 0x0F;
// 	uchar f = event & 0xF0;
// 	// fprintf(stderr, "%x %x\n", e, event);
// 	switch(e) {
// 		case 0x8: //note off
// 			fprintf(stderr, "%s\n", "-");
// 			break;
// 		case 0x9: //note on
// 			fprintf(stderr, "%s\n", "+");
// 			break;
// 		case 0xA: //note aftertouch
// 			fprintf(stderr, "%s\n", "0xA");
// 			break;
// 		case 0xB: //controller
// 			fprintf(stderr, "%s\n", "0xB");
// 			break;
// 		case 0xC: //program change
// 			fprintf(stderr, "%s\n", "0xC");
// 			break;
// 		case 0xD: //channel aftertouch
// 			fprintf(stderr, "%s\n", "0xD");
// 			break;
// 		case 0xE: //pitch bend
// 			fprintf(stderr, "%s\n", "0xE");
// 			break;
// 		case 0xff: //metaEvent
// 			fprintf(stderr, "%s\n", "0xff");
// 			break;
// 		case 0x2f: //eot
// 			fprintf(stderr, "%s\n", "0x2f");
// 			break;
// 		default:
// 			// fprintf(stderr, "%s\n", "ERROR: bad event");
// 			break;
// 	}

// 	return 0;
// }

int 
isEvent(uchar event) {
	uchar e = event & 0xF0;
	uchar channel = event & 0x0F;
	// channel = channel >> 4;
	e = e >> 4;
	fprintf(stderr, "%x %x %x\n", e, channel, event);
	// switch(e) {
		if (e == 8 && channel == 0) {
			fprintf(stderr, "%s\n", "-");
		} else if (e == 9 && channel == 0){
			fprintf(stderr, "%s\n", "+");
		} else if (event == 0x2f) {
			fprintf(stderr, "%s\n", "eot");
		}
		// case 0xA: //note aftertouch
		// 	fprintf(stderr, "%s\n", "0xA");
		// 	break;
		// case 0xB: //controller
		// 	fprintf(stderr, "%s\n", "0xB");
		// 	break;
		// case 0xC: //program change
		// 	fprintf(stderr, "%s\n", "0xC");
		// 	break;
		// case 0xD: //channel aftertouch
		// 	fprintf(stderr, "%s\n", "0xD");
		// 	break;
		// case 0xE: //pitch bend
		// 	fprintf(stderr, "%s\n", "0xE");
		// 	break;
		// case 0xff: //metaEvent
		// 	fprintf(stderr, "%s\n", "0xff");
		// 	break;
		// case 0x2f: //eot
		// 	fprintf(stderr, "%s\n", "0x2f");
		// 	break;
		// default:
		// 	// fprintf(stderr, "%s\n", "ERROR: bad event");
		// 	break;
	// }

	return 0;
}
ulong 
getAsLong(uchar *bytes) {
	return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

ushort 
getAsShort(uchar *bytes) {
	return bytes[0] << 8 | bytes[1];
}

int
MidiFile::getTrackSize() {
	// return getAsShort();
}
int
MidiFile::setTrackSize() {
	// _trkSz = sz;
	return 0;
}

ushort
MidiFile::getBPM() {
	uchar buffer[2];
	for (int i = 0; i < 2; i++) {
		buffer[i] = getNextChar(_target);
	}
	
	// if (buffer[0] == 0)
	// 	_bpm = buffer[1];
	
	return getAsShort(buffer);
}

int
MidiFile::setBPM() {
	_fileHeader._bpm = getBPM();
	fprintf(stderr, "BPM: %d\n", _fileHeader._bpm);
	return 0;
}

ulong
MidiFile::getHeaderSize() {
	//read header size, should be 6
	uchar buffer[4];
	for (int i = 0; i < 4; i++) {
		buffer[i] = getNextChar(_target);
	}	

	return getAsLong(buffer);
}

int 
MidiFile::setHeaderSize() {
	_fileHeader._size = getHeaderSize();

	fprintf(stderr, "size %lu\n", _fileHeader._size);
	if (_fileHeader._size != 6)
		fprintf(stderr, "%s %lu\n", "Error, bad size", _fileHeader._size);

	assert(_fileHeader._size == 6);
	return 0;
}

int 
MidiFile::readFileHeader() {
	//read header
	for (int i = 0; i < 4; i++) {
		_fileHeader._hdr[i] = getNextChar(_target);
	}	_fileHeader._hdr[4] = '\0';

	// #ifdef DEBUG 
	fprintf(stderr, "%s\n", _fileHeader._hdr);
	// #endif

	if (strcmp((char*)_fileHeader._hdr, "MThd"))
		fprintf(stderr, "%s\n", "Error, bad header");

	setHeaderSize();

	return 0;
}

ushort 
MidiFile::getTrackCount() {
	return getNextShort(_target);
}

int 
MidiFile::setTrackCount() {
	_fileHeader._tracks = getTrackCount();
	fprintf(stderr, "tracks: %i\n", _fileHeader._tracks);
	return 0;
}

ushort 
MidiFile::getType() {
	return getNextShort(_target);;
}

int 
MidiFile::setType() {
	_fileHeader._type = getType();
	fprintf(stderr, "type: %u\n", _fileHeader._type);
	return 0;
}

int 
MidiFile::readTrack() {
	enum {START, META, ENDING} state;
	
	uchar trkHdr[4];
	
	uchar trkSz[4];
	for (int i = 0; i < 4; i++) {
		trkHdr[i] = getNextChar(_target);
	}	trkHdr[4] = '\0';

	fprintf(stderr, "%s\n", trkHdr);

	ulong size = 0;
	for (int i = 0; i < 4; i++) {
		trkSz[i] = getNextChar(_target);
		// fprintf(stderr, "%x\n", trkSz[i]);
		// size += trkSz[i];
	}

	size = getAsLong(trkSz);
	fprintf(stderr, "(alleged) track length: %lu\n", size);
	// fprintf(stderr, "%ld\n", atol((char*)trkSz));
	uchar c = 0;
	short prevNote = 0;

	int count = 0;
	// while ((c = getNextChar(midi)) != EOF) {
	for (int i = 0; i < size; i++) {
		c = getNextChar(_target);
		isEvent(c);
		if ((c & 0x0F) == 0x8) {
			// fprintf(stderr, "%c\n", '-');
		} else if ((c & 0x0F) == 0x9) { //note on
			if (prevNote > c) {
				_contour.push_back('U');
			} else if (prevNote < c && count > 0) {
				_contour.push_back('D');
			} else {
				_contour.push_back('R');
			}
			count++;
			prevNote = c;
			// int tmp = getNextChar(midi);
			// fprintf(stderr, "note: %i\n", tmp);
			// fprintf(stderr, "%c\n", '+');
		}
	}
	fprintf(stderr, "note on events: %i\n", count);
	return 0;
}

int
MidiFile::read(const char *fileName) {
	if (fileName == NULL)
		return 1;
	
	setFileName(fileName);
	_target = openFile(fileName);
	
	if (!_target)
		return 1;

	readFileHeader();
	setType();
	setTrackCount();
	setBPM();
	for (int i = 0; i < _fileHeader._tracks; i++)
		readTrack();

	fclose(_target);
   	return 0;
}		

void
MidiFile::printContour() {
	for( std::vector<uchar>::const_iterator i = _contour.begin(); i != _contour.end(); ++i)
   		fprintf(stderr, "%c", *i);
   	fprintf(stderr, "%c\n", '\n');
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

void
Note::printNote() {

}