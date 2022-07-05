# ObEngine NEAR Ledger Plugin

## Setup
On Linux extra libraries might be needed:
```
libusb-1.0-0-dev libhidapi-dev
```

## Build
```shell
git submodule update --init --recursive
mkdir build
cd build
cmake ..
make -j8
```

## Use
Copy the resulting `libNearLedgerPlugin.*` (extension is platform dependant), into the ObEngine
`Plugins/` directory.

Then from Lua, e.g. from `Projects/SampleProject/GameObjects/SampleObject/SampleObject.lua:22`
```lua
local version = ledger.get_version();
print(table.concat(version,"."))

local account = ledger.bip32_path_to_bytes("44'/397'/0'/0'/1'", --[[prepend_length=]]false);
print(table.concat(account,","))

local public_key = ledger.get_public_key(account);
print(table.concat(public_key,","))

-- This way of declaring serialized_tx does not work though...
local serialized_tx = {
    20,0,0,0,...
};
local signature = ledger.sign(account, serialized_tx);
print(table.concat(signature, ","))
```
> Note that calling `get_version()` between calls to `get_public_key()` or `sign()` allows to reset
> the state of the Near Nano application and avoids some weird issues.

## References
- [Library to sign NEAR transactions on a Ledger Nano](https://github.com/nbleuzen-ledger/near-ledger)
- [NEAR app on Ledger Nano](https://github.com/LedgerHQ/app-near)
- [NEAR transaction crafting and broadcasting example](https://github.com/near-examples/transaction-examples/blob/master/send-tokens-deconstructed.js)
