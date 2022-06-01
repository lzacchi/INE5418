# Socket Programmin

## Instructions:
### Manual compilation:
* Server: `gcc -o server.out server/server.c file_handler/file_handler.c -lpthread`
* Client: `gcc -o client.out client/client.c file_handler/file_handler.c`

### Makefile
* Server: `make server`
* Client: `make client`
* Or `make all`
* Remove executables: `make clean`

### Running
* The server app must be executed beforehand;
* After the server is up, the client app can be executed;
* File names must be relative. Example: server/files/readme.txt

### Example run
* `>./client.out`
* Specify the name of the file like so: "server/files/readme.txt"
* The file should be transfered and saved as "client/files/download"
