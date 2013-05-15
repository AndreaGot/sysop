
# ============================================================================
# Academic Year : 2012-2013
# Project Name  : Progetto 1: Modifica utility di base di GNU/Linux
# Authors       : Giuliano Tortoreto (152183) and Andrea Gottardi (151955)
# Name          : mkbkp.c
# Description   : Archivia i file specificati, li estrae ed elenca il contenuto dell'archivio
# ============================================================================


CC = gcc
CFLAGS = -Wall

COUT = managelogs/managelogs.o

all: create_dir mkbkp equal plive

create_dir:
	mkdir -p files
	#mkdir -p /var/log/utility # remove first # in order to make mkdir work

managelogs.o: 
	$(CC) $(CFLAGS) -c managelogs/managelogs.c -o managelogs/managelogs.o

mkbkp.o:
	$(CC) $(CFLAGS) -c mkbkp/mkbkp.c -o mkbkp/mkbkp.o

equal.o:
	$(CC) $(CFLAGS) -c equal/equal.c -o equal/equal.o

plive.o:
	$(CC) $(CFLAGS) -c plive/plive.c -o plive/plive.o

mkbkp: create_dir mkbkp.o managelogs.o
	$(CC) $(CFLAGS) $(COUT) mkbkp/mkbkp.o -o files/mkbkp

equal: create_dir equal.o managelogs.o
	$(CC) $(CFLAGS) $(COUT) equal/equal.o -o files/equal

plive: create_dir plive.o managelogs.o
	$(CC) $(CFLAGS) $(COUT) plive/plive.o -o files/plive

clean:
	$(RM) -r files; $(RM) -r */*.o
