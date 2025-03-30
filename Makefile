
PROJ_NAME = $(shell basename $(CURDIR))


COMP = g++

SRC_FILES = $(wildcard src/*.cpp)
CFLAGS = -Iinc

all: $(PROJ_NAME).bin

$(PROJ_NAME).bin: $(SRC_FILES)
	$(COMP) $(CFLAGS) -o $@ $^

