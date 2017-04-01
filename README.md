# NFCee Firmware

## Requirements

Manually install the [GNU ARM embedded toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm), or use the following [PPA](https://launchpad.net/~team-gcc-arm-embedded/+archive/ubuntu/ppa):

```shell
sudo add-apt-repository ppa:team-gcc-arm-embedded/ppa
sudo apt-get update
sudo apt-get install gcc-arm-embedded
```

## Build

```shell
git clone --recursive https://github.com/renaudcerrato/nfcee-fw.git
cd nfcee-fw
mkdir build
cd build/
cmake .. && make
```

## Debugging

https://github.com/gnuarmeclipse/openocd/releases
