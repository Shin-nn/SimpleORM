include ../Makefile.const

TESTS=expr-001 db-001

.PHONY: clean

all: | $(TESTS)

test: all
	@for i in $(TESTS); do echo "$$i - TODO"; done;

clean:
	@rm -f $(TESTS)
	@echo "Cleaned..."

%: %.cpp ../src/*
	$(CXX) $(CXXFLAGS) $(OPTIMALIZATION) -o $@ $< $ -lsqlite3 -I../src/