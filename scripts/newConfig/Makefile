# Search source directories for any sources to build

OBJDIR := $(PWD)/obj
LIBDIR := $(PWD)/lib
SOURCES_C := $(shell find . -name '*.c')
SOURCES_CPP := $(shell find . -name '*.cpp')
C_BINS := $(patsubst %.c, %, $(SOURCES_C))
CPP_BINS := $(patsubst %.cpp, %, $(SOURCES_CPP))
BINARY_NAME := newconfig

CC		:= gcc
CPP		:= g++
LD		:= ld

CFLAGS		= -O2
CPPFLAGS	= -pthread -DHAVE_CONFIG_H -I. -I./include -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=500
LDFLAGS		= -lncursesw -lcppurses -lpthread $(LIBDIR)/libpugixml.a $(LIBDIR)/libfmt.a


all : $(OBJDIR) $(C_BINS) $(CPP_BINS) $(BINARY_NAME)

$(OBJDIR):
	mkdir -p $@

$(C_BINS):
	$(CC) -o $(OBJDIR)/$@.o -lc -lm $(CPPFLAGS) $(CFLAGS) -c $@.c

$(CPP_BINS):
	$(CPP) -std=c++2a -pthread -lc -lcc1 -lm -o $(OBJDIR)/$@.o -I. -I./include $(CFLAGS) -c $@.cpp

$(BINARY_NAME): $(C_BINS) $(CPP_BINS)
	$(CPP) $(OBJDIR)/*.o $(LDFLAGS) -o $(BINARY_NAME)

clean:
	$(RM) $(OBJDIR)/*.o $(BINARY_NAME)

test:
	./$(BINARY_NAME) ../../nConfig.xml

debug:
	./$(BINARY_NAME) ../../nConfig.xml dummy
