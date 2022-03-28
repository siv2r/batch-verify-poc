#include <stdio.h>
#include <stdlib.h>
#include <secp256k1.h> // for SECP256K1_INLINE macro

// #include "batch.h"
#include "batch_impl.h"
#include "scratch_impl.h"
#include "util.h"

#define POINTS 3

int main() {
    printf("creating context object...\n");
    batch *ctx = batch_context_create(POINTS);
    printf("destroying context object...\n");
    batch_context_destroy(ctx);
}