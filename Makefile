include ./Makefile.const

.PHONY: all

LIBS=src/Expression.o

all: libsimpleorm.so libsimpleorm.a

libsimpleorm.so: $(LIBS)
	$(CXX) -shared $(CXXFLAGS) -o $@ $^

libsimpleorm.a: $(LIBS)
	$(AR) $(ARFLAGS) $@ $^

src/Expression.o: src/Expression.cpp src/SimpleORM/Expression.h
	$(CXX) $(CXXFLAGS) $(OPTIMALIZATION) -c $< -o $@

test:
	make -C tests

documentation:
	doxygen

clean:
	@rm -f ./src/*.o libsimpleorm.a libsimpleorm.so
	@echo "Cleaned ..."
