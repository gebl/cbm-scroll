#######################################################################
# General variables. Twiddle as you see fit.
#######################################################################

CC65_TARGET=c64
CC      = cc65
AS      = ca65
CL      = ld65
CFLAGS  = -t $(CC65_TARGET)
LDFLAGS = -t $(CC65_TARGET) -m $(PROGRAM).map --dbgfile $(PROGRAM).dbg

#######################################################################

PROGRAM:=dino.prg
SRCS:=$(sort $(wildcard *.c) $(wildcard *.s))

OBJECT_LIST:=$(SRCS:.c=.o)
OBJECT_LIST:=$(OBJECT_LIST:.s=.o)

LINK_OBJECTS:=$(addprefix objects/,$(OBJECT_LIST))

.phony all: $(PROGRAM)

.phony clean:
	rm -fr objects
	rm -f *.map
	rm -f $(PROGRAM)

objects/%.s: %.c
	mkdir -p objects
	$(CC) -Oi -T -t c64 $< -o $@

objects/%.o: %.s
	mkdir -p objects
	$(AS) $(CFLAGS) -o $@ $<

objects/%.o: objects/%.s
	mkdir -p objects
	$(AS) $(CFLAGS) -o $@ $<

$(PROGRAM): $(LINK_OBJECTS)
	$(CL) $(LDFLAGS) -o $@ $^ $(CC65_TARGET).lib
