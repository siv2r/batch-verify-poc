#include <stdio.h>
#include <stdlib.h>
#include <secp256k1.h> // for SECP256K1_INLINE macro

// #include "scalar.h"
#include "scalar_impl.h"

void print_hex(const unsigned char *buf, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) {
        printf("%02x", buf[i]);
        if(i % 4 == 3) {
            printf(" ");
        }
    }
    printf("\n");
}

//TODO: include -c89 flag during compilation
int main() {
    secp256k1_scalar sc =
        SECP256K1_SCALAR_CONST(0xF3F148DB, 0xF94B1BCA, 0xEE189630, 0x6141F319,
                               0x729DCCA9, 0x451617D4, 0xB529EB22, 0xC2FB521A);
    unsigned char buf[32];

    secp256k1_scalar_get_b32(buf, &sc);
    print_hex(buf, 32);

    return 0;
}