#ifndef UTIL_H
#define UTIL_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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

void print_ascii(const unsigned char *buf, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) {
        printf("%c", buf[i]);
    }
    printf("\n");
}

unsigned char hex_char_to_buf(const unsigned char inp) {
    if (inp >= '0' && inp <= '9') {
        return inp - '0';
    } else if (inp >= 'A' && inp <= 'F') {
        return inp - 'A' + 10;
    } else if (inp >= 'a' && inp <= 'f') {
        return inp - 'a' + 10;
    }
    fprintf(stderr, "hex string should contain only 0-9,a-f and A-F");

    return -1;
}

/* given hex string is stored as big endian bytes in unsinged char
Q  : is there a way to set the out pointer by passing it through arguments?
Ans: yes, but in that case the user has to allocate memory (either dynamically or statically)
*/
void hex_str_to_buf(unsigned char *out, int out_len, const unsigned char *inp, int inp_len) {
    int i = 0, j = 0, temp = (inp_len + 1)/2; /* output string length */
    unsigned char low4, high4;

    assert(out_len == temp);

    if (inp_len % 2) {
        out[j] = hex_char_to_buf(inp[i]);
        i++;
        j++;
    }
    /* the remaining hex string length will be even */
    while (i < inp_len && j < out_len) {
        high4 = hex_char_to_buf(inp[i]);
        low4 = hex_char_to_buf(inp[i+1]);
        out[j] = low4 | (high4 << 4);

        i += 2;
        j++;
    }

    /*TODO: verify that `out` val is correct? */
}
#endif