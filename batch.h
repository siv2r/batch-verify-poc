#ifndef BATCH_H
#define BATCH_H

#include <stdlib.h>

#include "scratch.h"
#include "secp256k1_extrakeys.h"

typedef struct batch_struct batch;

batch* batch_context_create(size_t n);

/* args = 1, 2, 5 are not null */
int batch_add_sig(
    batch *ctx, 
    const unsigned char *sig64, 
    const unsigned char *msg, 
    size_t msglen, 
    const secp256k1_xonly_pubkey *pubkey
);

/* args = 1, 2, 4, 5 are not null */
int batch_add_xonly_pubkey_tweak_check(
    batch *ctx,
    const unsigned char *tweaked_pubkey32,
    int tweaked_pk_parity,
    const secp256k1_xonly_pubkey *internal_pubkey,
    const unsigned char *tweak32
);

int batch_verify(batch *ctx);

void batch_context_destroy (batch *ctx);

#endif