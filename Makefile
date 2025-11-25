CC = gcc
CLANG = clang
CFLAGS = -lbpf
#note change this based on architecture
BPF_CFLAGS = -O2 -g -target bpf -I/usr/include/aarch64-linux-gnu
SRC = src
BIN = bin

all: $(BIN) $(BIN)/list $(BIN)/load $(BIN)/xdp_filter.o

$(BIN):
	mkdir -p $(BIN)

$(BIN)/list: $(SRC)/list.c
	$(CC) $< -o $@ $(CFLAGS)

$(BIN)/load: $(SRC)/load.c
	$(CC) $< -o $@ $(CFLAGS)

$(BIN)/xdp_checker.o: $(SRC)/xdp_filter.c
	$(CLANG) $(BPF_CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN)/*
