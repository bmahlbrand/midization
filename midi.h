#include <vector>
#include "stdio.h"
#include "string.h"

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

class MidiFile {
public:
	MidiFile(void);
	MidiFile(const char *fileName);
	~MidiFile();

	std::vector<uchar> _data;
	const char *_fileName;
	int track;
	int time;
	void setFileName(const char *fileName);
	int openFile(FILE &file);
	int read(const char *fileName);
	int isNoteOn(int track, int i);
	int isNoteOff(int track, int i);
	int isMeta(int track, int i);
	int getTrack(int track, int i);
};

// typedef struct headerChunk {
// 	unsigned char[4] _MThd;
// 	unsigned char[4] _length;
// 	unsigned char[2] _format;
// 	unsigned char[2] _n;
// 	unsigned char[2] _division;
// } HeaderChunk;

// typedef struct trackChunk {
// 	unsigned char[4] _MTrk;
// 	unsigned char[4] _length;
// 	unsigned char *_trackEvent;
// } TrackChunk;

// typedef struct midiFile {
// 	HeaderChunk *_header;

// } MidiFile;