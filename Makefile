SOURCES = main.cpp digester.cpp getDirStats.cpp
CPPC = g++
CPPFLAGS = -std=c++11 -c -Wall -O2
LDLIBS = -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib -lssl -lcrypto
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = dirstats

all: $(TARGET)

digester.o: digester.h
getDirStats.o: getDirStats.h digester.h
%.o : %.c
$(OBJECTS): Makefile 

.cpp.o:
	$(CPPC) $(CPPFLAGS) $< -o $@

$(TARGET): $(OBJECTS)
	$(CPPC) -o $@ $(OBJECTS) $(LDLIBS)

.PHONY: clean
clean:
	rm -f *~ *.o $(TARGET) 

