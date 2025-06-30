# Project name
TARGET = main

# Toolchain
CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Directories
SRCDIRS = . BOARD drivers utilities include
INCDIRS = . BOARD drivers utilities include

# Source files
C_SOURCES = \\\ \
  main.c \
  BOARD/board.c \
  BOARD/pin_mux.c \
  BOARD/clock_config.c \
  drivers/fsl_smc.c \
  drivers/fsl_uart.c \
  drivers/fsl_gpio.c \
  drivers/fsl_lpsci.c \
  drivers/fsl_clock.c \
  drivers/fsl_common.c \
  utilities/fsl_io.c \
  utilities/fsl_log.c \
  utilities/fsl_str.c \
  utilities/fsl_assert.c \
  utilities/fsl_debug_console.c \
  include/system_MKL46Z4.c

ASM_SOURCES = startup_MKL46Z4.S

# Includes
INCLUDES = $(addprefix -I, $(INCDIRS))

# Flags
CFLAGS = -mcpu=cortex-m0plus -mthumb -Wall -g -O2 $(INCLUDES)
ASFLAGS = -mcpu=cortex-m0plus -mthumb -x assembler-with-cpp $(INCLUDES)
LDFLAGS = -T MKL46Z256xxx4_flash.ld -nostartfiles --specs=nosys.specs -Wl,-Map=$(TARGET).map

# Output files
ELF = $(TARGET).elf
BIN = $(TARGET).bin
HEX = $(TARGET).hex

# Default rule
all: $(ELF) $(BIN) $(HEX)

$(ELF): $(C_SOURCES) $(ASM_SOURCES) MKL46Z256xxx4_flash.ld
	$(CC) $(CFLAGS) $(ASM_SOURCES) $(C_SOURCES) -o $@ $(LDFLAGS)
	$(SIZE) $@

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

# Flash rule
flash: $(ELF)
	openocd -f openocd.cfg -c "program $(ELF) verify reset exit"

# Clean rules
clean:
	rm -f *.o *.d *.map $(BIN) $(HEX)

cleanall: clean
	rm -f $(ELF)

.PHONY: all clean cleanall flash 