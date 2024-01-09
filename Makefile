CC = gcc
CFLAGS = -Wall
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Lista de arquivos fonte
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Gere uma lista de objetos a partir dos arquivos fonte
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Nome do executável do cliente e do servidor
CLIENT_EXEC = $(BIN_DIR)/client
SERVER_EXEC = $(BIN_DIR)/server

# Regras
all: $(CLIENT_EXEC) $(SERVER_EXEC)

# Compilação do cliente
$(CLIENT_EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJ_DIR)/client.o $(OBJ_DIR)/common.o $(OBJ_DIR)/campo_minado.o $(OBJ_DIR)/actions.o -o $(CLIENT_EXEC)

# Compilação do servidor
$(SERVER_EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJ_DIR)/server.o $(OBJ_DIR)/common.o $(OBJ_DIR)/campo_minado.o $(OBJ_DIR)/actions.o -o $(SERVER_EXEC)

# Compilação dos objetos a partir dos arquivos fonte
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Crie diretórios se eles não existirem
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

# Limpeza
clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(CLIENT_EXEC) $(SERVER_EXEC)
