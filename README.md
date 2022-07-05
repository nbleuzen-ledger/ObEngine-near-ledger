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

## Use
From Lua:
```
local version = ledger.get_version();
print(table.concat(version,"."))

local account = ledger.bip32_path_to_bytes("44'/397'/0'/0'/1'", --[[prepend_length=]]false);
print(table.concat(account,","))

local public_key = ledger.get_public_key(account);
print(table.concat(public_key,","))
```

## References
- [Library to sign NEAR transactions on a Ledger Nano](https://github.com/nbleuzen-ledger/near-ledger)
- [NEAR app on Ledger Nano](https://github.com/LedgerHQ/app-near)
- [NEAR transaction crafting and broadcasting example](https://github.com/near-examples/transaction-examples/blob/master/send-tokens-deconstructed.js)
