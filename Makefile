CC = g++
CFLAGS=-Wall -std=c++0x -g
LDFLAGS=
# ROOT_LIBS = `root-config --libs`
# ROOT_INCLUDE  =  `root-config --incdir`
# INCDIR       =  include/
# OBJ = Terrain.o RayTrace.o Dem.o Attenuation.o Flux.o
TARGET =  shape_test

%.o: src/%.cpp
		$(CC) -c $(CFLAGS) $(LDFLAGS) $^ -o $@
%: %.o
	$(CC) -o $@ $^

all:  app

app: $(TARGET)
	# strip -s $<


#shape_test.o: MrShape.o
shape_test: MrShape.o

clean:
	rm *.o $(TARGET)
