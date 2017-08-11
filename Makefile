CC = gcc
CXX = g++
CFLAGS = -O0 -Wall
# CXXFLAGS = -O0 -frtti -fexceptions -Wall
CXXFLAGS = -g -O0 -fno-strict-aliasing -Wno-write-strings -frtti -fexceptions -Wall -DDEBUG
INCPATH = -I./libphz -I./zlib -I./include

LD = gcc
LFLAGS = #-static
LIBS =  -L./libphz -L./zlib -L./lib -lphz -lzlib -lprotobuf -lstdc++ -lev -ljsoncpp -lhiredis 

DESTDIR_TARGET = chenzhouphz
OBJECTS = zjh.o daemonize.o log.o redis_client.o client.o game.o jpacket.o table.o player.o base64.o ppacket.o loginproto.pb.o gameproto.pb.o 

all: $(DESTDIR_TARGET)

$(DESTDIR_TARGET): $(OBJECTS)
	$(LD) $(LFLAGS) -o $(DESTDIR_TARGET) $(OBJECTS) $(LIBS)

####### Implicit rules ######

.SUFFIXES: .cpp .cc .cxx .c
.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

clean:
	rm -rf $(DESTDIR_TARGET) yongxingmhz  yizhangphz linwuphz  matianphz  jiahephz guiyangphz *.o

getall:
	cp chenzhouphz yongxingmhz
	cp chenzhouphz yizhangphz
	cp chenzhouphz linwuphz
	cp chenzhouphz matianphz
	cp chenzhouphz jiahephz
	cp chenzhouphz guiyangzpphz
	cp chenzhouphz chenzhou3phz