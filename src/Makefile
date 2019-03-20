# Embed-Soft EBuilder MakeFile generator
# This is a machine generated file, DO NOT edit
# 2005-11-17 11:40:17

CC 		= arm-elf-gcc
CXX 		= arm-elf-g++
OBJCOPY 	= arm-elf-objcopy
AR 		= arm-elf-ar
LD 		= arm-elf-ld
OUTPUT_DIR 	= ./
OUTPUT_NAME 	= test.elf
OUTPUT_BIN  	= test.bin
LIB 		= /usr/local/arm-elf/lib
GCCLIB 		= /usr/local/lib/gcc/arm-elf/3.4.3
EBC_LIB 	= 

TEXT 		= 01060000
DATA 		= 00002000
BSS 		= 00010000

INC_PATH 	= -I"/usrl/local/arm-elf/include" -I"/usr/local/lib/gcc/arm-elf/3.4.3/include" -I"./Include" -I"./" -I"./inc_low" -I"./inc_lwip" 
DEFINE 	= 
CPPFLAGS 	= $(INC_PATH) $(DEFINE)
ENDIAN_AS 	= -Wa,-EL
ENDIANFLAG 	= -mlittle-endian  -A 'endian(little)'
CFLAGS 		+= $(INC_PATH) $(DEFINE) $(ENDIANFLAG) -fno-rtti -DMACHINE_TYPE_ARM -O0
ASFLAGS 	= -x assembler-with-cpp -Wa,--gstabs -Wa,-mall $(ENDIAN_AS)
LDFLAGS 	= -n -N -nostartupfiles -static -EL -e_start -g -Map Maptest.txt
LINK_DIR 	= ./
LINK_SCRIPT 	:= "C:/EBuilder1.5/bin/ESDEV.LD"
LIBS 	= ./sf_lib/uCosII.a ./sf_lib/lwip.a $(LIB)/libm.a $(LIB)/libc.a $(GCCLIB)/libgcc.a

SOURCES 	= low_boot.s low_int.s armcpu.s system.c memory.c queue.c device.c serial.c stdin.c stdout.c console.c des.c util.c font.c graphics.c lcd_128x64.c stdapi.c main.c low_flash.c low_irq.c low_lcd.c low_net.c low_uart.c low_bios.c 
OBJECTS 	:= low_boot.o low_int.o armcpu.o system.o memory.o queue.o device.o serial.o stdin.o stdout.o console.o des.o util.o font.o graphics.o lcd_128x64.o stdapi.o main.o low_flash.o low_irq.o low_lcd.o low_net.o low_uart.o low_bios.o 

all: $(OBJECTS) $(OUTPUT_NAME) $(OUTPUT_BIN)
low_boot.o : low_boot.s
	$(CC) -c $(CFLAGS) $(ASFLAGS) low_boot.s
low_int.o : low_int.s
	$(CC) -c $(CFLAGS) $(ASFLAGS) low_int.s
armcpu.o : armcpu.s
	$(CC) -c $(CFLAGS) $(ASFLAGS) armcpu.s
system.o : system.c
	$(CC) -c $(CFLAGS) system.c
memory.o : memory.c
	$(CC) -c $(CFLAGS) memory.c
queue.o : queue.c
	$(CC) -c $(CFLAGS) queue.c
device.o : device.c
	$(CC) -c $(CFLAGS) device.c
serial.o : serial.c
	$(CC) -c $(CFLAGS) serial.c
stdin.o : stdin.c
	$(CC) -c $(CFLAGS) stdin.c
stdout.o : stdout.c
	$(CC) -c $(CFLAGS) stdout.c
console.o : console.c
	$(CC) -c $(CFLAGS) console.c
des.o : des.c
	$(CC) -c $(CFLAGS) des.c
util.o : util.c
	$(CC) -c $(CFLAGS) util.c
font.o : font.c
	$(CC) -c $(CFLAGS) font.c
graphics.o : graphics.c
	$(CC) -c $(CFLAGS) graphics.c
lcd_128x64.o : lcd_128x64.c
	$(CC) -c $(CFLAGS) lcd_128x64.c
stdapi.o : stdapi.c
	$(CC) -c $(CFLAGS) stdapi.c
main.o : main.c
	$(CC) -c $(CFLAGS) main.c
low_flash.o : low_flash.c
	$(CC) -c $(CFLAGS) low_flash.c
low_irq.o : low_irq.c
	$(CC) -c $(CFLAGS) low_irq.c
low_lcd.o : low_lcd.c
	$(CC) -c $(CFLAGS) low_lcd.c
low_net.o : low_net.c
	$(CC) -c $(CFLAGS) low_net.c
low_uart.o : low_uart.c
	$(CC) -c $(CFLAGS) low_uart.c
low_bios.o : low_bios.c
	$(CC) -c $(CFLAGS) low_bios.c

$(OUTPUT_NAME): $(OBJECTS)
	$(LD) -Ttext $(TEXT) -Tdata $(DATA) -Tbss $(BSS) $(LDFLAGS) -L"$(LINK_DIR)" -T$(LINK_SCRIPT) -o $@ $^ $(LIBS)

$(OUTPUT_BIN): $(OUTPUT_NAME)
	$(OBJCOPY) -R .data -R .rdata -R .sbss -R .bss  -O binary $(OUTPUT_NAME) tempbin.1
	$(OBJCOPY) -R .text -R .rodata -R .glue_7 -R .glue_7t -R .rodata.str1.4 --change-section-address .data=0 -O binary $(OUTPUT_NAME) tempbin.2
	cat -B tempbin.1 tempbin.2 > test.bin
	#rm tempbin.1 tempbin.2
	@echo.exe -e "[test.bin] CREATED SUCCESSFUL!\n"
.PHONY: clean
clean:
	rm -f $(OBJECTS)
