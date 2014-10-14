all: midiDriver

midiDriver:
	g++ -o midiDriver driver.cpp midi.cpp swapBytes.cpp -m32

clean:
	rm -f *.o midiDriver