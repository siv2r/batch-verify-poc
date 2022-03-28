#include <stdio.h>
#include <stdlib.h>
#include <secp256k1.h> // for SECP256K1_INLINE macro

// #include "batch.h" --> include this to make opaque
#include "batch_impl.h"
#include "scratch_impl.h"
#include "util.h"

#define POINTS 3

int main() {
    printf("creating context object...\n\n");
    batch *ctx = batch_context_create(POINTS);
    /* add terms to batch object */
    secp256k1_ge arr_pts[3] = {secp256k1_ge_const_g, secp256k1_ge_const_g, secp256k1_ge_const_g};
    secp256k1_scalar arr_sc[3] = {secp256k1_scalar_one, secp256k1_scalar_one, secp256k1_scalar_one};
    for (int i = 0; i < POINTS; i++) {
        batch_add_one_term(ctx, &arr_sc[i], &arr_pts[i]);
    }
    /* print batch object value */
    print_batch(ctx);
    //todo: build ecmult_context
    //todo: put sc_g = -3
    //todo: call batch_verify
    //todo: result should be 0
    printf("destroying context object...\n");
    batch_context_destroy(ctx);
}