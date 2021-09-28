all:
	redo-ifchange all

debug:
	echo $@ > make.mode

opt: optimized
optimized:
	echo $@ > make.mode

run: all
	./beeworlds

clean:
	redo $@
clean-obj:
	redo $@
