#ifndef BATCH_IMPL_H
#define BATCH_IMPL_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <secp256k1.h> // for SECP256K1_INLINE macro

#include "scratch_impl.h"
#include "util.h"
#include "group_impl.h"
#include "field_impl.h"
#include "scalar_impl.h"
#include "debug.h"

#include "ecmult.h" // for ECMULT_TABLE_SIZE
#include "precomputed_ecmult.h" // for WINDOW_G ECMULT_WINDOW_SIZE
#include "ecmult_impl.h"

#include "secp256k1_extrakeys.h"
// #include "batch.h"

typedef struct batch_struct{
    secp256k1_scratch *data;
    secp256k1_gej *points;
    secp256k1_scalar *scalars;
    secp256k1_scalar sc_g;
    secp256k1_gej res_gej;
    size_t len;
    size_t capacity;
    int result;
} batch;

//todo: removed this function in the final version
void* custom_malloc(size_t size) {
    void *ret = malloc(size);
    if (ret == NULL) {
        printf("dynamic memory allocation failed!");
        return NULL;
    }
    return ret;
}

batch* batch_context_create(size_t n) {
    size_t scratch_size;
    size_t checkpoint;
    batch *ret = (batch*)custom_malloc(sizeof(batch));

    /* create scratch space that can store `n terms` */
    scratch_size = secp256k1_strauss_scratch_size(n) + STRAUSS_SCRATCH_OBJECTS*16;
    ret->data = secp256k1_scratch_create(NULL, scratch_size);
    checkpoint = secp256k1_scratch_checkpoint(NULL, ret->data);
    /*allocate n scalar and n points on scratch */
    ret->scalars = (secp256k1_scalar*)secp256k1_scratch_alloc(NULL, ret->data, n*sizeof(secp256k1_scalar));
    ret->points = (secp256k1_gej*)secp256k1_scratch_alloc(NULL, ret->data, n*sizeof(secp256k1_gej));

    if (ret->scalars == NULL || ret->points == NULL) {
        secp256k1_scratch_apply_checkpoint(NULL, ret->data, checkpoint);
        printf("(scalar, point) memory allocation failed!\n");
        return NULL;
    }
    /* set scalar of g to 0 */
    secp256k1_scalar_clear(&ret->sc_g);
    //todo: intial value for result_gej?
    ret->len = 0;
    ret->capacity = n;
    ret->result = 0;

    return ret;
}

int batch_add_sig(batch *ctx, const unsigned char *sig64, const unsigned char *msg, size_t msglen, const secp256k1_xonly_pubkey *pubkey) {
    /* check args prerequsites */
    assert(sig64 != NULL);
    assert(pubkey != NULL);

    //todo: check schnorrsig_verify for decompress
}

int batch_add_xonly_pubkey_tweak_check(batch *ctx, const unsigned char *tweaked_pubkey32, int tweaked_pk_parity, const secp256k1_xonly_pubkey *internal_pubkey, const unsigned char *tweak32) {
    /* check args prerequsites */
    assert(tweaked_pubkey32 != NULL);
    assert(internal_pubkey != NULL);
    assert(tweak32 != NULL);

    //todo: check secp256k1_xonly_pubkey_tweak_add_check for decompression
}

int batch_verify(batch *ctx) {
    //todo: && with is_gej_infinity()
    ctx->result = secp256k1_ecmult_strauss_batch(NULL, ctx->data, ctx->scalars, ctx->points, &ctx->res_gej, &ctx->sc_g, NULL, NULL, ctx->len, 0);
    return ctx->result;
}

void batch_context_destroy (batch *ctx) {
    if(ctx->data != NULL) {
        secp256k1_scratch_destroy(NULL, ctx->data);
    }
    ctx->scalars = NULL;
    ctx->points = NULL;
    secp256k1_scalar_clear(&ctx->sc_g);
    secp256k1_gej_clear(&ctx->result); //won't the result of batch_verify be also 0?
    ctx->len = ctx->capacity = ctx->result = 0;
}


int batch_add_one_term(batch *ctx, const secp256k1_scalar *sc, const secp256k1_ge *pt) {
    if (ctx->len == ctx->capacity) {
        /* run the batch_verify algorithm if scratch is full */
        batch_verify(ctx);
        //todo: print to the command line
        //todo: clear out the scratch space
        //todo: exit if batch_verify fails?
    }
    int i = ctx->len;
    secp256k1_scalar_cmov(&ctx->scalars[i], sc, 1); /* alternative for this? */
    secp256k1_gej_set_ge(&ctx->points[i], pt);
    ctx->len += 1;
    return 1;
}

void print_batch(batch *inp) {
    printf("length  : %lu\n", inp->len);
    printf("capacity: %lu\n", inp->capacity);
    printf("result: %lu\n\n", inp->result);
    for (int i = 0; i < inp->len; i++) {
        printf("scalar_%d:\n", i);
        print_scalar(&inp->scalars[i]);
    }
    printf("\n");
    for (int i = 0; i < inp->len; i++) {
        printf("point_%d:\n", i);
        print_gej(&inp->points[i]);
    }
    printf("\n");
}

#endif