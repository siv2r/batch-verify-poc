#include <stdio.h>
#include <stdlib.h>
#include <secp256k1.h> // for SECP256K1_INLINE macro

// #include "batch.h" --> include this to make opaque
#include "batch_impl.h"
#include "scratch_impl.h"
#include "util.h"

#define POINTS 3
//todo: write a wrapper func on printf for this
#define DEBUG 0 // make this 1 to print debug information

/* checks if 0 ?= -3G + 1G + 1G + 1G */
//todo: why malloc fails if both tests created their own batch object?
void test1(batch *ctx) {
    /* set inp scalar of g = -3 */
    secp256k1_scalar_set_int(&ctx->sc_g, 3);
    secp256k1_scalar_negate(&ctx->sc_g, &ctx->sc_g);
    /* call batch_verify */
    assert(batch_verify(ctx));
    /* check if the res_gej = indentity elem */
    assert(secp256k1_gej_is_infinity(&ctx->res_gej));
    printf("Test1 success!!\n");
}

/* checks if 3G ?= 0G + 1G + 1G + 1G */
void test2(batch *ctx) {
    /* set inp scalar of g = 0 */
    secp256k1_scalar_clear(&ctx->sc_g);
    /* call batch_verify */
    assert(batch_verify(ctx));
    //todo: check if the res_gej = 3*G
    secp256k1_ge expected = SECP256K1_GE_CONST(0xf9308a01, 0x9258c310, 0x49344f85, 0xf89d5229, 0xb531c845, 0x836f99b0, 0x8601f113, 0xbce036f9, 0x388f7b0f, 0x632de814, 0x0fe337e6, 0x2a37f356, 0x6500a999, 0x34c2231b, 0x6cb9fd75, 0x84b8e672);
    secp256k1_ge calculated;
    secp256k1_ge_set_gej(&calculated, &ctx->res_gej);
    assert(secp256k1_ge_is_valid_var(&expected));
    assert(secp256k1_ge_is_valid_var(&calculated));
    assert(secp256k1_fe_equal_var(&expected.x, &calculated.x));
    assert(secp256k1_fe_equal_var(&expected.y, &calculated.y));
    if (DEBUG) {
        printf("Expected Result:\n");
        print_ge(&expected);
        printf("\nCalcuated Result:\n");
        print_ge(&calculated);
        printf("\n");
    }
    printf("Test2 success!!\n");
}

int main() {
    if (DEBUG)
        printf("creating context object...\n\n");
    batch *ctx = batch_context_create(POINTS);
    /* add terms to batch object */
    secp256k1_ge arr_pts[3] = {secp256k1_ge_const_g, secp256k1_ge_const_g, secp256k1_ge_const_g};
    secp256k1_scalar arr_sc[3] = {secp256k1_scalar_one, secp256k1_scalar_one, secp256k1_scalar_one};
    for (int i = 0; i < POINTS; i++) {
        batch_add_one_term(ctx, &arr_sc[i], &arr_pts[i]);
    }
    if (DEBUG)/* print batch object value */
        print_batch(ctx);
    printf("--------------------------------\n");
    printf("Running test1 on batch object\n");
    test1(ctx);
    printf("--------------------------------\n");
    printf("Running test2 on batch object\n");
    test2(ctx);
    printf("--------------------------------\n");
    if (DEBUG)
        printf("destroying context object...\n\n");
    batch_context_destroy(ctx);
}