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

