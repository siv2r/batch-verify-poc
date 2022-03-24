#ifndef BATCH_IMPL_H
#define BATCH_IMPL_H

#include "batch.h"

typedef struct {
    secp256k1_scratch *data;
    size_t len;
    size_t capacity;
    int result;
} batch_struct;

#endif