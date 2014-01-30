CC = g++
CFLAGS=-Wall -std=c++11 -g
CFLAGS+=-O3 
LDFLAGS=
# ROOT_LIBS = `root-config --libs`
# ROOT_INCLUDE  =  `root-config --incdir`
# INCDIR       =  include/
# OBJ = Terrain.o RayTrace.o Dem.o Attenuation.o Flux.o
TARGET =  shape_test

%.o: src/%.cpp
		$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@ $(filter *.o, $^)
%: %.o
	$(CC) -o $@ $^

all:  app

app: $(TARGET)
	# strip -s $<


#shape_test.o: MrShape.o
#MrShape.o: basic_types.o
#MrRay.o:   basic_types.o

shape_test: MrShape.o MrRay.o basic_types.o

clean:
	rm *.o $(TARGET)
