TARGET = $(notdir $(CURDIR))

DBGCFLAGS = -g -D__DEBUG
C_FLAGS += -Wall -g
CPP_FLAGS=
LD_FLAGS += -lpthread

COMPILE.c = gcc $(C_FLAGS) $(CPP_FLAGS) -c
LINK.c =  gcc $(LD_FLAGS)

EXEC_DIR = ../app
DBGTARGET = debug/$(TARGET)d
RELTARGET = release/$(TARGET)

DBGCFLAGS = -g -D__DEBUG
RELCFLAGS = -O2 -fno-strict-aliasing

SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)

DBGOBJFILES = $(SOURCES:%.c=debug/%.o)
RELOBJFILES = $(SOURCES:%.c=release/%.o)

.PHONY: clean debug release install

all:    debug release

install:
	install -d $(EXEC_DIR)
	install $(RELTARGET) $(EXEC_DIR)

release:        $(RELTARGET)
debug:          $(DBGTARGET)

#static lib
RELLDFLAGS = 
DBGLDFLAGS = 
XDC_CFLAGS = 

$(RELTARGET):   $(RELOBJFILES) $(XDC_LFILE)
	$(LINK.c) -o $@ $^ $(RELLDFLAGS)

$(DBGTARGET):   $(DBGOBJFILES) $(XDC_LFILE)
	$(LINK.c) -o $@ $^ $(DBGLDFLAGS)

$(RELOBJFILES): release/%.o: %.c $(HEADERS)
	@mkdir -p release
	$(COMPILE.c) $(RELCFLAGS) -o $@ $<

$(DBGOBJFILES): debug/%.o: %.c $(HEADERS)
	@mkdir -p debug
	$(COMPILE.c) $(DBGCFLAGS) -o $@ $<
clean:
	rm -rf varlist release debug
