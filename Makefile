# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

WM = drw.cpp util.cpp dwm.cpp config.cpp
LAUNCH = startup.c
COMP = composition.cpp
OBJ = ${WM:.cpp=.o}
UID := $(shell id -u)

all: options dwm

options:
	@echo dwm build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CPPCOMPILER = ${CPPCOMPILER}"
	@echo "CCOMPILER = ${CCOMPILER}"
	@echo "HOME = ${HOME}"

.cpp.o:
	cd ./bin/;${CPPCOMPILER} -c ${CFLAGS} ../$<

${OBJ}: config.mk

configFile:
# only if not root
	if [ ${UID} != 0 ]; then\
		cp -f defkeybinds.conf ~/.config/dwm/keybinds.conf;\
		cp -f defdwm.conf ~/.config/dwm/dwm.conf;\
		fi

./bin:
	mkdir -p ./bin/


dwm: configFile ./bin ${OBJ}
	cd ./bin/;${CPPCOMPILER} -o $@-wm ${OBJ} ${LDFLAGS}
	

clean:
	rm -f dwm ${OBJ} dwm-${VERSION}.tar.gz

dist: clean
	mkdir -p dwm-${VERSION}
	cp -R LICENSE Makefile README config.def.h config.mk\
		dwm.1 drw.h util.h ${WM} dwm.png transient.c dwm-${VERSION}
	tar -cf dwm-${VERSION}.tar dwm-${VERSION}
	gzip dwm-${VERSION}.tar
	rm -rf dwm-${VERSION}

install: all
	${CCOMPILER} -o $@ ${LAUNCH} -DBUILDPATH=\"${DESTDIR}${PREFIX}/bin\"
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cd ./bin/;cp -f dwm ${DESTDIR}${PREFIX}/bin
	cd ./bin/;cp -f dwm-wm ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/dwm
	chmod 755 ${DESTDIR}${PREFIX}/bin/dwm-wm
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < dwm.1 > ${DESTDIR}${MANPREFIX}/man1/dwm.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/dwm.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwm\
		${DESTDIR}${MANPREFIX}/man1/dwm.1

dinst: all
	${CCOMPILER} -o dwm ${LAUNCH} -DBUILDPATH=\"${HOME}/dwm-customisation/bin\"
# chmod 755 ./dwm

.PHONY: all options clean dist install uninstall
