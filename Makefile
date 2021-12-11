REDO = redo

all:
	${REDO} $@

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
	${REDO} $@
clean-obj:
	${REDO} $@

.PHONY: all debug opt optimized run gdb clean clean-obj
