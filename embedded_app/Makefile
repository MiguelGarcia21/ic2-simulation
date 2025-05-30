TARGET = main
BIN = $(TARGET).bin

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -mcpu=cortex-m3 -mthumb -Wall -g -O0
LDFLAGS = -T stm32f100.ld -nostartfiles -Wl,--gc-sections

all: $(BIN)

$(BIN): $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf: $(TARGET).o
	$(CC) $(LDFLAGS) $^ -o $@

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.elf *.bin

qemu: $(BIN)
	qemu-system-arm -M stm32vldiscovery -kernel $< -nographic -serial tcp::1234,server,nowait

.PHONY: all clean qemu