

CFLAGS = -I include -Wall

SRC = $(wildcard src/*.c)

OBJ = $(patsubst src/%.c, build/%.o, $(SRC))

DEBUG = -g

TAP_NODE := /dev/net/tap

WIFI_DEVICE := wlps0

TAP_DEVICE := lnet_tap_0

# define func_iptables_setting
# sysctl net.ipv4.ip_forward=1
# iptables -I INPUT -s 10.0.0.0/24 -j ACCEPT 
# iptables -t nat -I POSTROUTING --out-interface $(WIFI_DEVICE) -j MASQUERADE
# iptables -I FORWARD --in-interface $(WIFI_DEVICE) --out-interface $(TAP_DEVICE) -j ACCEPT
# iptables -I FORWARD --in-interface $(TAP_DEVICE) --out-interface $(WIFI_DEVICE) -j ACCEPT
# endef

.PHONY: lnet clean check

lnet: $(OBJ) lnet_mk_nod lnet_ip_tables
	gcc $(DEBUG) $(OBJ) -o $@
	./lnet

lnet_mk_nod: 
ifeq ("$(wildcard $(TAP_NODE))", "")
	mknod /dev/net/tap c 10 200
endif
	
lnet_ip_tables:
	$(call func_iptables_setting)

build/%.o: src/%.c
	mkdir -p $(dir $@)
	gcc -g  $(CFLAGS) -c $< -o $@

clean:
	rm build/*.o lnet -rf
	iptables -F

check:
	iptables -L

# gcc -dM -E - < /dev/null 用于打印 gcc 中定义的宏 