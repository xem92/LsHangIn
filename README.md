# LsHangIn
LsHangIn is a system consisting in a group of online services, programed and running on UNIX environments, that will reseive petitions and will return the results to the clients.

## Compiling
Use `make` for compiling the code.

## Configuration files
### users.dat
"Database" for the clients. 
Format: `<user_name>:<password>:<id>`
File example:
  <Tarzan>:<1e3h56>:<34>
  <Jack>:<4815162342>:<815>
  <Joan>:<asd>:<7>
### config.dat
Configuration file use with the server IP and port.
Example:
  192.168.1.10
  8100

## Server
To run the server: `server`
  
## Client
To run a client: `client`
Client commands:
  * connect<port>:<IP>
  * show_rooms
  * create_room<room_name>:<max_people>
  * delete_room<room_name>
  * modify_room<room_name>:<new_max_people>
  * enter_room<room_name>
  * talk<room_name>:<text>
  * exit_room<room_name>

**Note:** delete_room and modify_room are commands reserved for the room admin.
**Note2:** the room admin is the one who created the room.
