CFLAGS = -Wall -Wextra -Wshadow -Wunreachable-code -Wredundant-decls \
         -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes \
         -Wdeclaration-after-statement -Wno-return-local-addr \
         -Wunsafe-loop-optimizations -Wuninitialized -Werror

GFLAGS = -Wunreachable-code

REMOTE := origin
BRANCH := main
MESSAGE := Automated commit of C source code and makefile

TARGET = ${LOGNAME}-Lab3
FILES = *.c makefile
TAR = $(TARGET).tar.gz

LIBS = -lmd

all: viktar $(TAR)

viktar: viktar.o
	gcc $(GFLAGS) -o viktar viktar.o $(LIBS)
	
viktar.o: viktar.c
	gcc $(GFLAGS) -c viktar.c


push:
	git add *.c makefile
	git commit -m "$(MESSAGE)"
	git push $(REMOTE) $(BRANCH)

$(TAR): $(FILES)
	tar -czvf $(TAR) $(FILES)

clean cls:
	rm -f $(TAR) *.o all *~ \#*
