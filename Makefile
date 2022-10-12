# Makefile for WHATT team
# Used to create main exe only
# made by:
# Pechdanai Saepong (Nine) 63070503434 
# Fasai Sae-Tae (Atom) 63070503436 
# Chanon Khanijoh (Sean) 63070503408 
# Pokpong jitjomnongmate (Paul) 63070503425

ifeq ($(OSTYPE),WINDOWS)
	EXECEXT =.exe
	COMP	=__MINGCC__
	PLATFORM	=mingw
else
	EXECEXT =
	COMP	=__GCC__
	PLATFORM	=linux
endif

EXECUTABLES= main$(EXECEXT)

all : $(EXECUTABLES)

read.o :	read.c read.h feature.h linkedListUtil.h
	gcc -c read.c

feature.o :	feature.c read.h linkedListUtil.h
	gcc -c feature.c

linkedListUtil.o :	linkedListUtil.c linkedListUtil.h
	gcc -c linkedListUtil.c

validation.o :	validation.c validation.h
	gcc -c validation.c

main.o :	main.c read.h feature.h validation.h
	gcc -c main.c

main$(EXECEXT) : feature.o linkedListUtil.o main.o read.o validation.o
	gcc -o main$(EXECEXT) feature.o linkedListUtil.o main.o read.o validation.o

clean : 
	-rm *.o
	-rm $(EXECUTABLES) 