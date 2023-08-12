CFLAGS = -lcurl -lcjson -Wall -Wpedantic
BIN = saur

TARGETS = src/main.c src/request.c

saur: ${TARGETS}
	gcc -o ${BIN} ${CFLAGS} ${TARGETS}


clean:
	rm ${BIN}

