TARGET_SERVER = server.out
TARGET_CLIENT = client.out

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

CC=gcc
.PHONY: all
all: server client

.PHONY: server
server:
	$(CC) server/server.c file_handler/file_handler.c -o $(BUILD_DIR)/$(TARGET_SERVER) -lpthread

.PHONY: client
client:
	$(CC) client/client.c file_handler/file_handler.c -o $(BUILD_DIR)/$(TARGET_CLIENT)

.PHONY: clean
clean:
	rm $(BUILD_DIR)/$(TARGET_SERVER) $(BUILD_DIR)/$(TARGET_CLIENT)
