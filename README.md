Batch Interface PoC
---
This is a proof of concept for the batch object and its APIs. The following decisions were made to simplyfy the implementation:
- no randomizer generation
- no API for decompressing `schnorr_sig`
- no API for decompressing `tweak_check`
- support only `strauss_batch` for batch verfication
- no callback func and callback data

Interface Details
---
- Batch Object Structure
```C
typedef struct batch_struct{
  secp256k1_scratch *data;
  secp256k1_gej *points;
  secp256k1_scalar *scalars;
  size_t len;
  size_t capacity;
  int result;
} batch;
```
- API's available to the user
```C
batch* batch_context_create(size_t n);

/* this will be replaced by `add_sig` and `add_tweak_check` in the final implmentation*/
int batch_add_one_term(batch *ctx, const secp256k1_scalar *sc, const secp256k1_ge *pt);

int batch_verify(batch *ctx);

void batch_context_destroy (batch *ctx);

```

Build Instructions
---
To clone this repository:
```
git clone --recurse-submodules [URL Link]
cd batch-verify-poc
```
To build and run the code:
```
make
./main #batch object usage example
./scratch #secp256k1_scratch usage example
```

Checklist
---
Make sure the following in final implementation.
- Batch object needs to be opaque in final impl
- Modify libsecp256k1 build to support a new API
- generate randomizers

Drawbacks of this PoC
---
- allocating `scalar: secp256k1_scalar[n]`, `points: secp256k1_gej[n]` consecutively (on scratch space) will force the user to enter exactly `n` terms. But we only required the user to enter atmost `n` terms.

Scratch Space Notes
---
- After allocating memory block in scratch (int* ptr = scratch_alloc(10)) don't use `scratch->data` to access the value, use only `ptr`
- [void pointers in C](https://www.geeksforgeeks.org/void-pointer-c-cpp/)

Doubts
---
1. why data present (in temp ptr) after destroying the scratch space?
2. why allocating 3 points (`secp256k1_ge`) on scratch space requires more memory than `3*sizeof(secp256k1_ge)`?
   - `3 * sizeof(secp256k1_ge)` = `3 * 88` = `264`
   - but after `scratch_alloc(264)`, the `scratch->alloc_size = 272` not `264`. why?
   - read about alignment of C structs
3. `scratch_destroy` does not seem to free the `scratch->data` why? Also, its `max_size` and `alloc_size` are not set to 0 after destroying the object.
4. The final implementation will not have callback func or callback data, right?