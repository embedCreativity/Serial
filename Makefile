# Makefile for EmbedCreativity's serial library
SHARED_LIB := libec_serial.so
LIBVERSION := 1.0.0

# build helloworld executable when user executes "make"
CFLAGS += -Wall
LDFLAGS += -L/usr/lib
LIBRARY += -lec_serial

libSerial: ec_serial.o
	$(CC) -shared -Wl,-soname,$(SHARED_LIB) -o $(SHARED_LIB).$(LIBVERSION) ec_serial.o -lc
	sudo mv $(SHARED_LIB).$(LIBVERSION) /usr/lib/
	sudo cp --preserve=timestamps ec_serial.h /usr/include/
	sudo chmod 644 /usr/include/ec_serial.h
	sudo ldconfig -n /usr/lib

ec_serial.o: ec_serial.c
	$(CC) -fPIC -g -c -Wall ec_serial.c

# remove object files and executable when user executes "make clean"
clean:
	rm -f *.o $(SHARED_LIB)*


