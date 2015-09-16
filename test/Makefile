include ../Makefile.const

TESTS=db-001

all: | build $(TESTS)
	@for i in $(TESTS); do echo "$$i - "; done;

build:
	make -C ../ 

db-001: db-001.cpp ../src/*
	$(CXX) $(CXXFLAGS) $(OPTIMALIZATION) -o $@ $< $ -lsqlite3 -I../src/