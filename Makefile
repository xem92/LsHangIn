all: client server

RoomsList.o: RoomsList.c lib.h
	gcc -c RoomsList.c -Wall -Wextra
auxiliar.o: auxiliar.c lib.h
	gcc -c auxiliar.c -Wall -Wextra
UserList.o: UserList.c lib.h
	gcc -c UserList.c -Wall -Wextra
log.o: log.c lib.h
	gcc -c log.c -Wall -Wextra
md5.o: md5.c md5.h
	gcc -c md5.c -Wall -Wextra
sockets.o: sockets.c lib.h config.h
	gcc -c sockets.c -Wall -Wextra
command.o: command.c lib.h log.h
	gcc -c command.c -Wall -Wextra
client.o: client.c lib.h
	gcc -c client.c -Wall -Wextra
server.o: server.c lib.h
	gcc -c server.c -Wall -Wextra

client: client.o UserList.o md5.o command.o sockets.o auxiliar.o RoomsList.o log.o
	gcc client.o UserList.o md5.o command.o sockets.o auxiliar.o RoomsList.o log.o -o client -lpthread

server: server.o UserList.o command.o md5.o sockets.o auxiliar.o RoomsList.o log.o
	gcc server.o UserList.o command.o md5.o sockets.o auxiliar.o RoomsList.o log.o -o server -lpthread