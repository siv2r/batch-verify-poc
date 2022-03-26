#include <stdio.h>
#include <stdlib.h>
#include <secp256k1.h> // for SECP256K1_INLINE macro

#include "scratch_impl.h"
#include "util.h"
#include "group_impl.h"
#include "field_impl.h"
#include "scalar_impl.h"
#include "debug.h"

#define POINTS 3
#define POINT_SIZE (sizeof(secp256k1_ge) + sizeof(secp256k1_scalar))
#define SCRATCH_OBJECTS 2 // no of scratch objects allocated on scratch => (points, scalars)
#define EXTRA_SPACE (16 * SCRATCH_OBJECTS)
#define SCRATCH_MAX_SIZE ((POINTS * POINT_SIZE) + (EXTRA_SPACE))

//TODO: include -c89 flag during compilation
int main() {
    secp256k1_ge arr_pts[3] = {secp256k1_ge_const_g, secp256k1_ge_const_g, secp256k1_ge_const_g};
    secp256k1_scalar arr_sc[3] = {secp256k1_scalar_one, secp256k1_scalar_one, secp256k1_scalar_one};

    secp256k1_scratch *scratch = secp256k1_scratch_create(NULL, SCRATCH_MAX_SIZE);
    const size_t max_points_alloc = secp256k1_scratch_max_allocation(NULL, scratch, SCRATCH_OBJECTS) / POINT_SIZE;
    printf("Max number of POINTS allocation allowed: %lu\n", max_points_alloc);
    /* store checkpoint before allocating the data */
    const size_t scratch_checkpoint = secp256k1_scratch_checkpoint(NULL, scratch);
    /* allocates data for 3 (scalar, point) tuples*/
    secp256k1_ge *points = (secp256k1_ge*)secp256k1_scratch_alloc(NULL, scratch, POINTS * sizeof(secp256k1_ge));
    secp256k1_scalar *scalars = (secp256k1_scalar*)secp256k1_scratch_alloc(NULL, scratch, POINTS * sizeof(secp256k1_scalar));
    /* revert to old checkpoint, if allocation fails */
    if (points == NULL || scalars == NULL) {
        secp256k1_scratch_apply_checkpoint(NULL, scratch, scratch_checkpoint);
        printf("Exceeded maximum possible allocation...\n");
        return 0;
    }

    /* copy points data to the scratch space */
    for(int i=0; i<POINTS; i++) {
        printf("copying point %d to the scratch space ....\n", i);
        secp256k1_ge_set_xy(&points[i], &arr_pts[i].x, &arr_pts[i].y);
        printf("copying scalar %d to the scratch space ....\n", i);
        secp256k1_scalar_cmov(&scalars[i], &arr_sc[i], 1);
    }
    /* do computations on the scratch space date or simply print them */
    for(int i=0; i<POINTS; i++) {
        printf("point_%d:\n", i);
        print_ge(&points[i]);
        printf("scalar_%d:\n", i);
        print_scalar(&scalars[i]);
        printf("\n");
    }
    secp256k1_scratch_destroy(NULL, scratch);

    return 0;
}