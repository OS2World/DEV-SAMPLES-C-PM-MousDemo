# nmake makefile
#
# Tools used:
#  Compile::Watcom Resource Compiler
#  Compile::GNU C
#  Make: nmake or GNU make
all : mousdemo.exe

mousdemo.exe : mousdemo.obj mousdemo.def
	gcc -Zomf mousdemo.obj mousdemo.def -o mousdemo.exe
	
mousdemo.obj : mousdemo.c
	gcc -Wall -Zomf -c -O2 mousdemo.c -o mousdemo.obj

clean :
	rm -rf *exe *RES *obj *hlp
