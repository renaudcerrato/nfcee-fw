# NFCee with OpenOCD

## Usage

To launch openOCD type in a shell the following commands

```shell
openocd -f tools/stm32f4x.cfg &
telnet localhost 4444
```

Here are few basic commands (to type from a telnet terminal): 

```shell
reset  // reset the MCU
halt  // halt the MCU
reset halt //halt the MCU before CRT0
flash write_image eraseall 0x8000000 ncee.bin  // erase all the flash and write ncee.bin firmware at 0x8000000 flash location
```


