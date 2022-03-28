#ifndef BATCH_IMPL_H
#define BATCH_IMPL_H

#include <stdio.h>
#include <stdlib.h>
#include<assert.h>
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

#include "batch.h"

typedef struct batch_struct{
    secp256k1_scratch *data;
    secp256k1_gej *points;
    secp256k1_scalar *scalars;
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
    //todo: call secp256k1_ecmult_strauss_batch()
}

void batch_context_destroy (batch *ctx) {
    if(ctx->data != NULL) {
        secp256k1_scratch_destroy(NULL, ctx->data);
    }
    ctx->scalars = NULL;
    ctx->points = NULL;
    ctx->len = ctx->capacity = ctx->result = 0;
}

#endif