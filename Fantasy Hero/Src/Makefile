# Makefile to compile C++ code to a GBA-file
# @author: Jordi Jaspers

# --- Project details -------------------------------------------------

PROJ    := main
TARGET  := $(PROJ).cpp
OBJS    := $(PROJ).o
BIN		:= $(PROJ).elf
GBA		:= $(PROJ).gba

# --- File Locations -------------------------------------------------

DEVKITARM 	:= C:\devkitPro\devkitARM
EMULATOR	:= C:\mGBA\mGBA.exe
PATH 		:= $(DEVKITARM)/bin:$(PATH)

# --- Shortcuts ---------------------------------------------------

PREFIX  := arm-none-eabi-
CC      := $(PREFIX)g++
LD      := $(PREFIX)g++
OBJCOPY := $(PREFIX)objcopy

ARCH    := -mthumb-interwork -mthumb
SPECS   := -specs=C:\devkitPro\devkitARM\arm-none-eabi\lib\gba.specs

CFLAGS  := $(ARCH) -O2 -Wall -fno-strict-aliasing
LDFLAGS := $(ARCH) $(SPECS)

.PHONY : clean build
.DEFAULT_GOAL := build

# --- Build Process -----------------------------------------------------------

build: clean compile link copybin run

compile: $(TARGET)
	$(CC) -c $< $(CFLAGS) -o $(OBJS)

link: $(OBJS)
	$(LD) $^ $(LDFLAGS) -o $(BIN)	

copybin: $(BIN)
	$(OBJCOPY) -v -O binary $(BIN) $(GBA)

run: $(GBA)
	$(EMULATOR) ./main.gba	

clean : 
	rm -rf *.o
	rm -rf *.sav
	rm -rf *.elf
	rm -rf *.gba

# --- end ---------------------------------------------------------------	
