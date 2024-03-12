SRC := .
CC := gcc
BUILD := build
CFLAGS := -g -pthread

thread_pool:
	mkdir -p ${BUILD}
	${CC} ${CFLAGS} ${SRC}/*.c -o ${BUILD}/$@
	${BUILD}/$@

clean:
	rm -rf ${BUILD}
