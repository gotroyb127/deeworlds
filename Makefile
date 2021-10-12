NPROC = $$(nproc)

all:
	redo-ifchange -j ${NPROC} $@

debug:
	echo $@ > make.mode

opt: optimized
optimized:
	echo $@ > make.mode

run: all
	./beeworlds

gdb: debug all
	gdb ./beeworlds

clean:
	redo $@
clean-obj:
	redo $@
