include ./Makefile.const

.PHONY: all db

all: test

LINKS=-lsqlite3

test:
	make -C tests

documentation:
	doxygen

clean:
	@rm -f ./src/*.o
	@echo "Cleaned ..."
