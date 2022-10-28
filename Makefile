# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

SRC = drw.cpp util.cpp dwm.cpp config.cpp
OBJ = ${SRC:.cpp=.o}

all: options dwm

options:
	@echo dwm build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "COMPILER = ${COMPILER}"

.cpp.o:
	cd ./bin/;${COMPILER} -c ${CFLAGS} ../$<

${OBJ}: config.mk

configFile:
	cp -f config.def.h ./bin/config.h
	cp -f defkeybinds.conf ~/.config/dwm/keybinds.conf

./bin:
	mkdir -p ./bin/


dwm: configFile ./bin ${OBJ}
	cd ./bin/;${COMPILER} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f dwm ${OBJ} dwm-${VERSION}.tar.gz

dist: clean
	mkdir -p dwm-${VERSION}
	cp -R LICENSE Makefile README config.def.h config.mk\
		dwm.1 drw.h util.h ${SRC} dwm.png transient.c dwm-${VERSION}
	tar -cf dwm-${VERSION}.tar dwm-${VERSION}
	gzip dwm-${VERSION}.tar
	rm -rf dwm-${VERSION}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cd ./bin/;cp -f dwm ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/dwm
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < dwm.1 > ${DESTDIR}${MANPREFIX}/man1/dwm.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/dwm.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwm\
		${DESTDIR}${MANPREFIX}/man1/dwm.1

dinst: all
	mv ./bin/dwm .
	chmod 755 ./dwm

.PHONY: all options clean dist install uninstall
