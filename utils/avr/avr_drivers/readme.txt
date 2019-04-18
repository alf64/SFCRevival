These are common avr_drivers which could be shared across other projects.

Build them as Static Library (.a), and better name it depending on the target mcu and fcpu
i.e. libavr_drivers_atmega32_11059200
since the library shall be build for specific microcontroller and specific f_cpu

1. Include avr_drivers/include for other projects (-I compiler flag).
Example gcc invocation for project Flash_Protowriter using avr_drivers includes:
avr-gcc -I"C:\Users\bmudlaff\workspaces\eclipse_mars_avr\SFCRevival\utils\avr\avr_drivers\include" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=11059200UL -MMD -MP -MF"main.d" -MT"main.o" -c -o "main.o" "../main.c"

2. Include them in linker in the other projects (-l and -L linker flags)
Example linker invocation for project Flash_ProtoWriter using avr_drivers library:
avr-gcc -Wl,-Map,Flash_ProtoWriter.map -L"C:\Users\bmudlaff\workspaces\eclipse_mars_avr\SFCRevival\utils\avr\avr_drivers\Release" -mmcu=atmega32 -o "Flash_ProtoWriter.elf"  ./LEDdrv/led.o  ./FlashRW/flashrw.o  ./main.o   -lavr_drivers_atmega32_11059200
