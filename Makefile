SRC = src
LIB = lib

build b:
	$(MAKE) build --directory=$(SRC)

dbg d:
	$(MAKE) dbg --directory=$(SRC)

clean c:
	$(MAKE) clean --directory=$(SRC)

test t:
	$(MAKE) test --directory=$(LIB)
