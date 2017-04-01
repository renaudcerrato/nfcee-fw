# NFCee Firmware

## Requirements

```
sudo add-apt-repository ppa:team-gcc-arm-embedded/ppa
sudo apt-get update
sudo apt-get install gcc-arm-embedded
```

## Build

```
git clone https://github.com/renaudcerrato/nfcee-fw.git
cd nfcee-fw
mkdir build
cd build/
cmake .. && make
```

