Batch Interface PoC
---
This is a proof of concept for the batch object proposed in [this comment](https://github.com/bitcoin-core/secp256k1/pull/760#issuecomment-809242311). There are also some discussions [here](https://github.com/bitcoin-core/secp256k1/issues/1087).  
I  made the following decisions to simplify the implementation of this PoC:
- no randomizer generation.
- no API for decompressing `schnorr_sig`.
- no API for decompressing `tweak_check`.
- support only `strauss_batch` for batch verification.
- no callback func and callback data.
  
***Note:*** This PoC demonstrates user interaction with the batch object. I made some terrible design decisions (like a bad refactor of `strauss_batch`) to cut implementation time. These won't persist in the final implementation.

Interface Details
---
- Batch Object Structure
```C
typedef struct batch_struct{
    secp256k1_scratch *data; /*(scalar, Point)*/
    secp256k1_gej *points; /* base ptr of Points */
    secp256k1_scalar *scalars; /* base ptr of scalars */
    secp256k1_scalar sc_g; /* scalar of G */
    secp256k1_gej res_gej; /* final result as gej */
    size_t len; /* current len */
    size_t capacity; /* max possible len */
    int result; /* final result as success or fail */
} batch;
```
- API's available to the user
```C
/* allocates the memory for the batch object */
batch* batch_context_create(size_t n);

/* this API will be replaced by `batch_add_schnorrsig` and `batch_add_xonly_tweak_check` in the final implmentation*/
int batch_add_one_term(batch *ctx, const secp256k1_scalar *sc, const secp256k1_ge *pt);

/* not implemented 
adds (-ai, Qi), (ai, Pi) to the batch object
Qi = tweaked pubkey key  - secp256k1_gej
Pi = internal pubkey - secp256k1_gej
ai = randomizer - secp256k1_scalar
*/
int batch_add_xonly_tweak_check(batch* ctx, const unsigned char *tweaked_pubkey32, int tweaked_pk_parity, const secp256k1_xonly_pubkey *internal_pubkey, const unsigned char *tweak32);

/* not implemented 
adds (ai, Ri), (ai.ei, Xi) to the batch object
Ri = nonce commitment - secp256k1_gej
Xi = pubkey - secp256k1_gej
ai = randomizer - secp256k1_scalar
*/
int batch_add_schnorrsig(batch* ctx, const unsigned char *sig64, const unsigned char *msg, size_t msglen, const secp256k1_xonly_pubkey *pubkey);

/* computes the multi-multiplication:
0 ?= -sc*G + A1 + a2.A2 + ... + au.Au
*/
int batch_verify(batch *ctx);

/* destorys the context object */
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
Make sure the following are in the final implementation.  
[] Batch object needs to be opaque in the final impl  
[] Modify libsecp256k1 build to support a new API  
[] Generate randomizers  
[] In `batch_context_create()` if user gives `n` allocate `2n` points space (different from this PoC)  
[] In `batch_add_one_term`, transparently (print to terminal) run `batch_verify` is the scratch space is full 

Design Decisions
---
1. Allocating `scalar: secp256k1_scalar[n]`, `points: secp256k1_gej[n]` consecutively (on scratch space) during batch object creation.  
   - user wants to run verify on `n1` terms (`n1 < n`). Now, `2*(n-n1)` space on scratch will never be used
2. Refactor `staruss_batch` and `pippenger_batch` by adding two new arguments: `secp256k1_scalar* scalars` and `secp256k1_gej* points`.  
   - if `points = scalars = NULL`, then the scratch space does not have any points and scalars allocated inside it (same as old impl).
   - if `scalars != NULL` or `points != NULL`, then the scratch contains a contiguous block of `secp256k1_scalars` (pointer by `scalars` variable) or `secp256k1_gej` (pointer by `points` variable). Hence, `strauss_batch` avoid allocating space for points or scalars.
   - This seems like a bad refactor. Try to think of a design where we don't need to pass both `scalars` and `points` base pointers.
3. Randomizer generation - [see proposal](https://www.notion.so/siv2r/Batch-Verification-Proposal-siv2r-4b41080bd3de4c59a37ad9279d562155#df7c10ae194c4051a95f12b3bf3c08b1)
4. Synthetic Randomness - [see proposal](https://www.notion.so/siv2r/Batch-Verification-Proposal-siv2r-4b41080bd3de4c59a37ad9279d562155#d98dba3034db4543aa7d02851476e055)

Scratch Space Notes
---
- After allocating memory block in scratch (int* ptr = scratch_alloc(10)) don't use `scratch->data` to access the value, use only `ptr`
- [void pointers in C](https://www.geeksforgeeks.org/void-pointer-c-cpp/)

Doubts
---
1. why is data present (in temp ptr) after destroying the scratch space?
2. why does allocating 3 points (`secp256k1_ge`) on scratch space require more memory than `3*sizeof(secp256k1_ge)`?
   - `3 * sizeof(secp256k1_ge)` = `3 * 88` = `264`
   - but after `scratch_alloc(264)`, the `scratch->alloc_size = 272` not `264`. why?
   - read about alignment of C structs
3. `scratch_destroy` does not seem to free the `scratch->data` why? Also, its `max_size` and `alloc_size` are not set to 0 after destroying the object.
4. The final implementation will not have callback func or callback data, right?