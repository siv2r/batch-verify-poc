#include <stdio.h>
#include <stdlib.h>
#include <secp256k1.h> // for SECP256K1_INLINE macro

#include "scratch_impl.h"
#include "util.h"
#include "batch.h"

//TODO: include -c89 flag during compilation
int main() {
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t scratch_size = 20*sizeof(int);

    secp256k1_scratch *scratch = secp256k1_scratch_create(NULL, scratch_size);
    /* store checkpoint before allocating the data */
    const size_t scratch_checkpoint = secp256k1_scratch_checkpoint(NULL, scratch);
    /* allocates data for 10 integers*/
    int *points = (int*)secp256k1_scratch_alloc(NULL, scratch, 10 * sizeof(int));
    /* revert to old checkpoint, if allocation fails */
    if (points == NULL) {
        secp256k1_scratch_apply_checkpoint(NULL, scratch, scratch_checkpoint);
        return 0;
    }

    /* copy points data to the scratch space */
    for(int i=0; i<10; i++) {
        points[i] = arr[i];
    }
    /* do computations on the scratch space date or simply print them */
    for(int i=0; i<10; i++) {
        printf("obj_%d = %d\n", i, points[i]);
    }
    secp256k1_scratch_destroy(NULL, scratch);
    printf("obj_%d = %d\n", 2, points[2]); //TODO: why data present after destroying the scratch space?

    return 0;
}