CFLAGS = -Wall -Wextra -Wshadow -Wunreachable-code -Wredundant-decls \
         -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes \
         -Wdeclaration-after-statement -Wno-return-local-addr \
         -Wunsafe-loop-optimizations -Wuninitialized -Werror

TARGET = ${LOGNAME}-Lab3
FILES = *.c makefile
TAR = $(TARGET).tar.gz

all: viktar $(TAR)

viktar: viktar.o
	gcc $(CFLAGS) -o viktar viktar.o
	
viktar.o: viktar.c
	gcc $(CFLAGS) -c viktar.c

$(TAR): $(FILES)
	tar -czvf $(TAR) $(FILES)

clean cls:
	rm -f $(TAR) *.o *.err *.out all *~ \#*
