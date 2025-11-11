CC=gcc
CFLAGS=-c -Wall -Werror
LDFLAGS=-ljpeg
SOURCES= mandel.c jpegrw.c
EXECUTABLE=mandel

# Do not modify vars below this line!!

OBJECTS=$(SOURCES:.c=.o)

all: $(SOURCES) $(EXECUTABLE)

debug: CFLAGS += -g
debug: $(SOURCES) $(EXECUTABLE)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) *.d

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $< > $*.d
