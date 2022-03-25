Batch Interface PoC
---
This is a proof of concept for the batch object and its APIs.

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
./main
```

Checklist
---
Make sure the following in final implementation.
- Batch object needs to be opaque in final impl
- Modify libsecp256k1 build to support a new API

Scratch Space Notes
---
- After allocating memory block in scratch (int* ptr = scratch_alloc(10)) don't use `scratch->data` to access the value, use only `ptr`
- [void points in C](https://www.geeksforgeeks.org/void-pointer-c-cpp/)

Doubts
---
1. why data present (in temp ptr) after destroying the scratch space?
2. why allocating 3 points (`secp256k1_ge`) on scratch space requires more memory than `3*sizeof(secp256k1_ge)`?
   - `3 * sizeof(secp256k1_ge)` = `3 * 88` = `264`
   - but after `scratch_alloc(264)`, the `scratch->alloc_size = 272` not `264`. why?
   - read about alignment of C structs
3. 