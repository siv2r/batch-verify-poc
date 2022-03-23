#ifndef BATCH_H
#define BATCH_H

#include <stdlib.h>

#include "scratch.h"

typedef struct {
    secp256k1_scratch *data;
    size_t len;
    size_t capacity;
    int result;
} batch_struct;

#endif