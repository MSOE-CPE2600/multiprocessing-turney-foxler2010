CC=gcc
CFLAGS=-c -Wall -Werror
MANDEL_LDFLAGS= -lm -ljpeg -lpthread
MOVIE_LDFLAGS= -lm
MANDEL_SOURCES= mandel.c jpegrw.c
MANDEL_EXECUTABLE=mandel
MOVIE_SOURCES=mandelmovie.c outfile_parser.c growth_functions.c
MOVIE_EXECUTABLE=mandelmovie

# Do not modify vars below this line!!

MANDEL_OBJECTS=$(MANDEL_SOURCES:.c=.o)
MOVIE_OBJECTS=$(MOVIE_SOURCES:.c=.o)

all: $(MANDEL_EXECUTABLE) $(MOVIE_EXECUTABLE)

debug: CFLAGS += -g
debug: $(MANDEL_EXECUTABLE) $(MOVIE_EXECUTABLE)

clean:
	rm -rf $(MANDEL_OBJECTS) $(MANDEL_EXECUTABLE) $(MOVIE_OBJECTS) $(MOVIE_EXECUTABLE) *.d

# pull in dependency info for *existing* .o files
-include $(MANDEL_OBJECTS:.o=.d)
-include $(MOVIE_OBJECTS:.o=.d)

$(MANDEL_EXECUTABLE): $(MANDEL_OBJECTS)
	$(CC) $(MANDEL_OBJECTS) $(MANDEL_LDFLAGS) -o $@

$(MOVIE_EXECUTABLE): $(MOVIE_OBJECTS)
	$(CC) $(MOVIE_OBJECTS) $(MOVIE_LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $< > $*.d
