FLAGS = -std=gnu2x -Wall -Wextra -Werror -Wfatal-errors -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wformat=2 -Wnull-dereference -Wconversion -Wsign-conversion -fstack-protector-strong -D _POSIX_C_SOURCE=199309L
FLAGS_PRD = -O2 -march=native -fstack-protector-strong -fstack-clash-protection
FLAGS_DBG = -D DEBUG -O0 -g3 -ggdb -fno-omit-frame-pointer -fsanitize=address,undefined,leak

LIB = ../lib/lib.o
DGB = ../lib/debug.o

DIR = src



build: main.o

dbg: debug.o



main.o: main.c $(LIB)
	echo "building pi exec $(DIR)"
	gcc -o $@ $^ $(FLAGS) $(FLAGS_PRD)

debug.o: main.c $(DGB)
	echo "building pi debug $(DIR)"
	gcc -o $@ $^ $(FLAGS) $(FLAGS_DBG)



.PHONY: $(LIB)
$(LIB):
	$(MAKE) --directory=$(dir $@)

.PHONY: $(DGB)
$(DGB):
	$(MAKE) dbg --directory=$(dir $@)



clean:
	echo "cleaning pi $(DIR)"
	rm -rf *.o
	$(MAKE) clean --directory=$(dir $(LIB))
