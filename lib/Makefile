DIRS := $(wildcard */.)

LIB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))lib.o)
DGB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))debug.o)
CLU_FILE = ../mods/clu/bin/clu.o
NUM_FILE = ../mods/number/lib/lib.o

DIR = lib$(word 2,$(subst lib, ,$(CURDIR)))



build: lib.o

dbg: debug.o



lib.o: $(LIB_FILES) $(NUM_FILE)
	echo " linking pi object $(DIR)"
	gcc -r -o $@ $^ -flto

debug.o: $(DGB_FILES) $(NUM_FILE) $(CLU_FILE)
	echo " linking pi debug $(DIR)"
	gcc -r -o $@ $^ -fsanitize=address,undefined,leak



.PHONY: $(LIB_FILES) $(NUM_FILE) $(CLU_FILE)
$(LIB_FILES) $(NUM_FILE) $(CLU_FILE):
	$(MAKE) --directory=$(dir $@)

.PHONY: $(DGB_FILES)
$(DGB_FILES):
	$(MAKE) dbg --directory=$(dir $@)



clean c:
	$(MAKE) _clean -s

_clean:
	echo "cleaning pi $(DIR)"
	rm -rf *.o
	for DIR in $(DIRS) ; do \
		( $(MAKE) _clean --directory=$$DIR ) || exit $$? ; \
	done
	$(MAKE) clean --directory $(dir ${CLU_FILE}) 
	$(MAKE) clean --directory $(dir ${NUM_FILE}) 



.PHONY: test
test t:
	$(MAKE) dbg -s
	$(MAKE) _test -s

_test:
	for DIR in $(DIRS) ; do \
		( $(MAKE) _test --directory=$$DIR ) || exit $$? ; \
	done
