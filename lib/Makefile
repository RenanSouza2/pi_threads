DIRS := $(wildcard */.)

CLU_FILE = ../mods/clu/bin/bin.o
LIB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))lib.o)
DGB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))debug.o) $(CLU_FILE)

DIR = lib$(word 2,$(subst lib, ,$(CURDIR)))

build: lib.o



lib.o: $(LIB_FILES)
	echo " linking object $(DIR)"
	ld -r -o $@ $^

debug.o: $(DGB_FILES)
	echo " linking debug $(DIR)"
	ld -r -o $@ $^



.PHONY: $(LIB_FILES)
$(LIB_FILES):
	$(MAKE) --directory=$(dir $@)

.PHONY: $(DGB_FILES)
$(DGB_FILES):
	$(MAKE) debug.o --directory=$(dir $@)



clean c:
	$(MAKE) _clean -s

_clean:
	echo "cleaning $(DIR)"
	rm -rf *.o
	for FILE in $(DGB_FILES) ; do	\
		$(MAKE) _clean --directory=$$(dirname $$FILE) || exit $$?;	\
	done



.PHONY: test
test t:
	$(MAKE) debug.o
	$(MAKE) _test -s

_test:
	for DIR in $(DIRS) ; do \
		( $(MAKE) _test --directory=$$DIR ) || exit $$? ; \
	done
