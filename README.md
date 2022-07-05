# ObEngine NEAR Ledger Plugin

## Setup
On Linux extra libraries might be needed:
```
libusb-1.0-0-dev libhidapi-dev
```

## Build
```
git submodule update --init --recursive
mkdir build
cd build
cmake ..
make -j8
```

## References
- [Library to sign NEAR transactions on a Ledger Nano](https://github.com/nbleuzen-ledger/near-ledger)
- [NEAR app on Ledger Nano](https://github.com/LedgerHQ/app-near)
- [NEAR transaction crafting and broadcasting example](https://github.com/near-examples/transaction-examples/blob/master/send-tokens-deconstructed.js)
