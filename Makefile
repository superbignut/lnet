CFLAGS = -I include -Wall

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))
# headers = $(wildcard include /*.h)

TAP_NODE := /dev/net/tap

.PHONY: lnet

lnet: $(OBJ)
ifeq ($(wildcard $(TAP_NODE)), "")
	mknod /dev/net/tap c 10 200
endif
	gcc  -g $(OBJ) -o $@
	./lnet

build/%.o: src/%.c
	$(shell mkdir -p $(dir $@))
	gcc -g  $(CFLAGS) -c $< -o $@

clean:
	rm build/*.o lnet -rf