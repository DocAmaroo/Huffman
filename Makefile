CC = gcc
CFLAGS = -Wall
SOURCE = distri.c arbre.c compress.c decompress.c main.c 
PROGNAME = huf

MSQ = @
RM = $(MSQ)rm -f
CLEAR = $(MSQ)clear

MSG = $(MSQ)echo
MSG1 = $(MSG) "Compilation de $^ ==> $@"
MSG_OK = $(MSG) ".......OK"

all : $(PROGNAME)

#clean
cls :
	$(RM) *.o
	$(RM) $(SOURCE:.c=)

clean : cls
	$(MSG) "Cleanup is running...."
	$(CLEAR)
	$(MSG_OK)

#Compilation
$(PROGNAME) : $(SOURCE:.c=.o)
	$(MSG1)
	$(CC) $(CFLAGS) $^ -o $@
	$(CLEAR)
	$(MSG_OK)
