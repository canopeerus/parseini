include config.mk

all : ${ALL}
	${CC} ${CFLAGS} ${ALL} -o ${OUT}

install :
	cp -f ${OUT} ${BIN}/

uninstall:
	rm -f ${BIN}/${OUT}

clean:
	rm -f ${ALL} ${OUT}
