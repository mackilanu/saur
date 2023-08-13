CFLAGS = -lcurl -lcjson -Wall -Wpedantic
BIN = centaur

TARGETS = src/main.c src/centaur_rpc.c src/centaur_pkg_list.c

saur: ${TARGETS}
	gcc -o ${BIN} ${CFLAGS} ${TARGETS}


clean:
	rm ${BIN}

