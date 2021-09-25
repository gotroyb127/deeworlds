all:
	redo-ifchange all

run: all
	./beeworlds

clean:
	redo clean
