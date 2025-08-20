CC := $(CROSS_COMPILE)gcc
$(info "CC=$(CC)")

RM := rm -f

BIN := simple_chrdev_test

CFLAGS := $(CFLAGS) -I./inc -g -Wall -Werror

LDFLAGS ?= -lpthread -lrt

SRC :=	$(wildcard ./*.c)
OBJ := $(patsubst %.c, %.o, $(SRC))

ALL: $(BIN)

$(BIN): $(OBJ)
	@$(CC) -o $(BIN) $(OBJ) $(CFLAGS)  $(LDFLAGS)

clean:
	@$(RM) $(OBJ) $(BIN)

%o: %c
	$(CC) -c $< -o $@ $(CFLAGS) 