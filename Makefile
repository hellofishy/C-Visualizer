LIB356HOME=$(HOME)/documents/wesleyan/junior/comp356/local

CFLAGS += -std=c99 -Wall
LDFLAGS += -framework OpenGL -framework GLUT -framework CoreAudio -framework AudioToolbox -framework AudioUnit -framework CoreServices -framework Carbon -L$(LIB356HOME)/lib -l356 -L$(LIB356HOME)/lib -lportaudio -L$(LIB356HOME)/lib -lsndfile
CPPFLAGS += -D__MACOSX__ -I$(LIB356HOME)/include

BINS=main pixel2 pixel3

% : %.c
	$(CC) -o $@ $(CFLAGS) $(CPPFLAGS) $^ $(LDFLAGS)

%.o : %.c
	$(CC) -o $@ $(CFLAGS) -c $(CPPFLAGS) $<

clean :
	rm -f *.o
	rm -f $(BINS)
