CC = gcc
CFLAGS =-Wall
LDFLAGS =

SRC_PATH = src
BIN_PATH = bin

OBJS = $(BIN_PATH)/*.o

EXEC_CLIENT = client
EXEC_SERVER = server
EXECS = $(BIN_PATH)/$(EXEC_CLIENT) $(BIN_PATH)/$(EXEC_SERVER)

all: $(EXECS)

$(BIN_PATH)/$(EXEC_CLIENT): $(BIN_PATH)/$(EXEC_CLIENT).o
	$(CC) -o $@ $^ $(LDFLAGS)

$(BIN_PATH)/$(EXEC_SERVER): $(BIN_PATH)/$(EXEC_SERVER).o
	$(CC) -o $@ $^ $(LDFLAGS)

$(BIN_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f $(OBJS)

cleanall:
	rm -f $(EXECS) $(OBJS)
