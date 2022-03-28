CC = gcc
CFLAGS = -g -Wall -DSECP256K1_WIDEMUL_INT128 # last flag is required for `secp256k1_scalar`
CFLAGS += -DECMULT_WINDOW_SIZE=15 # as defined in basic-config.h
CPPFLAGS += -I./secp256k1/include -I./secp256k1/src

# uncomment the code below if you need any libsecp256k1 API
# LDFLAGS += -L./secp256k1/.libs
# LDLIBS += -lsecp256k1

BINS = main scratch

all: $(BINS)

main: main.o precomp.o
	$(CC) $(LDFLAGS) main.o precomp.o $(LDLIBS) -o $@

main.o: main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

precomp.o: ./secp256k1/src/precomputed_ecmult.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

scratch: scratch.o
	$(CC) $(LDFLAGS) $< $(LDLIBS) -o $@

scratch.o: scratch.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm *.o $(BINS)

# My notes: (of GNU)
# LDFLAGS   - flags that compiler needs to pass to linker while invoking it. -- Ex: -Lmy-lib-dir
# LDLIBS    - library flag/name that compiler needs to pass to linker while invoking it. -- Ex: -lfoo.
# LOADLIBES - this is depreciated ut still supported. An alternative to LDLIBS.
# CPPFLAGS  - flags that compiler need to pass to the preprocessor. -- Ex: -Imy-include-dir, -DMACRO=1
# if rule of a target (let's say main) is left empty then GNU make executes: (for C files)
#	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) main.c $(LDLIBS) -o main