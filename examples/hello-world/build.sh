make distclean TARGET=avr-atmega256rfr2
make  TARGET=avr-atmega256rfr2  hello-world.elf
avr-strip hello-world.elf

