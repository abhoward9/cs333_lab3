CFLAGS = -Wall -Wextra -Wshadow -Wunreachable-code -Wredundant-decls \
         -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes \
         -Wdeclaration-after-statement -Wno-return-local-addr \
         -Wunsafe-loop-optimizations -Wuninitialized -Werror

REMOTE := origin
BRANCH := main
MESSAGE := Automated commit of C source code and makefile

TARGET = ${LOGNAME}-Lab3
FILES = *.c makefile
TAR = $(TARGET).tar.gz

all: viktar $(TAR)

viktar: viktar.o
	gcc $(CFLAGS) -o viktar viktar.o
	
viktar.o: viktar.c
	gcc $(CFLAGS) -c viktar.c


push:
	git add *.c makefile
	git commit -m "$(MESSAGE)"
	git push $(REMOTE) $(BRANCH)

$(TAR): $(FILES)
	tar -czvf $(TAR) $(FILES)

clean cls:
	rm -f $(TAR) *.o all *~ \#*
