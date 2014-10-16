#include <vector>
#include "stdio.h"
#include "string.h"
#include <stdlib.h>   

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

typedef struct fileHeader {
	unsigned char _hdr[4];
	ushort _type;
	ulong _size;
	ushort _tracks;
	ushort _bpm;
} FileHeader;

 typedef struct tkChunk {
 	unsigned char _MTrk[4];
 	unsigned char _len[4] ;
 	unsigned char *_event;
 } TkChunk;

class Note {
	uchar note;
	ushort octave;
	void printNote();
	int setNote(uchar value);
};

class MidiFile {
public:
	MidiFile();
	MidiFile(const char *fileName);
	~MidiFile();
	FILE *_target;
	FileHeader _fileHeader;
	std::vector<uchar> _data;
	std::vector<uchar> _contour;
	const char *_fileName;
	int _trkSz;
	int _bpm;
	int setTrackSize(ulong sz);
	int setBPM(ushort bpm);
	void setFileName(const char *fileName);
	FILE *openFile(const char *fileName);
	int readFileHeader();
	int read(const char *fileName);
	ulong getNext(FILE *f);
	char getNextChar(FILE *f);
	ushort getNextShort(FILE *f);
	void printContour();
};

// typedef struct headerChunk {
// 	unsigned char[4] _MThd;
// 	unsigned char[4] _length;
// 	unsigned char[2] _format;
// 	unsigned char[2] _n;
// 	unsigned char[2] _division;
// } HeaderChunk;



// typedef struct midiFile {
// 	HeaderChunk *_header;

// } MidiFile;
