#UNIVERSAL MAKE FILE
#FOR ROBOT FILE

#TODO:
#Okay. You marking guys probably want us to chop this project up into multiple files. My abilities with the makefile must adapt
#Make a makefile that compiles both programs at the same time? A tomorrow job.
#Yeah. This is probably a bit of a butchering of the otherwise fine art of making makefiles. Perhaps pursue best practices when I have the time

#Detect operating system here.
#This code block is mostly cannibalised from a makefile made by a He Tao that I acquired from github.
#Link is https://gist.github.com/sighingnow/deee806603ec9274fd47
OSFLAG 				:=
ifeq ($(OS),Windows_NT)
	OSFLAG += WIN32
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		OSFLAG += LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		OSFLAG += OSX
	endif
endif

#SFML paths:

#WINDOWS:
#CFLAGS: C:\\SFML\include
#LFLAGS: C:\\SFML\lib

#MAC:
#CFLAGS: /usr/local/include/SFML/include
#LFLAGS: /usr/local/lib

#LINUX:
#This is only going to be running on my ubuntu computer, which I have installed SFML 2.4 using the package manager.
#I don't know what the version mac installs through homebrew, so I should watch this space to see if we run into and version mismatch errors
#CFLAGS: /usr/include/SFML
#LFLAGS: /usr/lib/x86_64-linux-gnu

ifeq ($(OSFLAG), WIN32)
	CFLAGS = -I C:\\SFML\include 
	LFLAGS = -L C:\\SFML\lib
endif 

ifeq ($(OSFLAG), LINUX)
	CFLAGS = -I /usr/include/SFML 
	LFLAGS = -L /usr/lib/x86_64-linux-gnu -Wl,-R /usr/lib/x86_64-linux-gnu
endif

ifeq ($(OSFLAG), OSX)
	CFLAGS = -I ${SFML}/include -std=c++0x
	LFLAGS = -L /usr/local/lib
endif


LIBS = -lsfml-window  -lsfml-graphics -lsfml-system -lsfml-network 
server3: server3.o 
	g++ $(LFLAGS) -o server3 server3.o ${LIBS} 
server3.o: server3.cpp 
	g++  -c $(CFLAGS) server3.cpp 



