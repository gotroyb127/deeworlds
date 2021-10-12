NPROC = $$(nproc)

all:
	redo-ifchange -j ${NPROC} $@

debug:
	echo $@ > make.mode

opt: optimized
optimized:
	echo $@ > make.mode

run: all
	./deeworlds

gdb: debug all
	gdb ./deeworlds

clean:
	redo $@
clean-obj:
	redo $@
