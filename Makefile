CFLAGS = -I include -Wall

src = $(wildcard src/*.c)
obj = $(patsubst src/%.c, build/%.o, $(src))
headers = $(wildcard include /*.h)

lnet: $(obj)
	gcc $(obj) -o $@

build/%.o: src/%.c
	$(shell mkdir -p $(dir $@))
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm build/*.o lnet