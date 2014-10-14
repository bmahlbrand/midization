all: midiDriver

midiDriver:
	g++ -o midiDriver driver.cpp midi.cpp

clean:
	rm -f *.o midiDriver