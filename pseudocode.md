# STM32 Bit-Banged I2C + QEMU Simulation + Python GUI (Educational Version)

This is an **educational and practice-oriented README** designed to help the reader (and myself) understand the structure and flow of a simulated STM32 firmware application. The goal is to show how each part of the project fits together, from low-level embedded C code to host-PC interaction using a GUI via TCP serial.

---

##  Project Structure Overview

```text
simulator/
├── embedded_app/
│   ├── main.c            # Bare-metal firmware for STM32 (bit-banged I2C logic)
│   ├── stm32f10x.h       # Basic memory-mapped register definitions
│   ├── Makefile          # Compiles the C code using arm-none-eabi toolchain
│   ├── stm32f100.ld      # Linker script: defines memory layout
│   └── syscalls.c        # Stub syscalls needed by newlib
├── gui/
│   └── gui.py            # Python GUI that connects to QEMU’s emulated UART
└── run_qemu.sh           # Launches QEMU simulation with TCP serial port
```

---

## What does it to

This project simulates a **bare-metal STM32 firmware** that performs bit-banged I2C transactions and communicates through a **virtual UART port** exposed over **TCP** by QEMU. A Python GUI runs on the host and interacts with the simulated firmware by sending commands over this virtual serial connection.

---

##  Detailed File Breakdown

### `main.c` – Firmware Logic (Bit-banged I2C)

This is the heart of the STM32 firmware. It runs on the simulated microcontroller in QEMU.

#### Functional:

* Configures GPIO pins manually.
* Emulates the I2C protocol by toggling GPIO lines (SCL and SDA).
* Runs an infinite loop that sends hardcoded I2C sequences.
* Optionally communicates status messages via USART.

#### Pseudocode:

```c
initialize_clocks();                  // Enable GPIO and peripheral clocks
configure_GPIO_for_I2C();            // Set GPIOs to open-drain, output
configure_USART_if_used();          // Configure USART TX (optional)

while (true) {
    start_I2C();
    send_byte(0xA0);
    wait_for_ACK();
    stop_I2C();

    send_debug_message_UART("I2C sent");
}
```

---

### `stm32f10x.h` – Register Map Definitions

This is a file from CMSIS modified for simplification. It defines the base addresses and register offsets for RCC, GPIO, and USART.

#### Example:

```c
#define RCC_APB2ENR (*((volatile uint32_t*) 0x40021018))
#define GPIOC_CRH   (*((volatile uint32_t*) 0x40011004))
```

#### Functional:

* Allows direct register access (`bare-metal` programming).
* Used to enable GPIO clocks and configure I/O modes.

---

### `Makefile` – Build Instruction

Compiles the firmware with `arm-none-eabi-gcc`.

#### Features:

* Uses `-mcpu=cortex-m3` and `-mthumb` for STM32F1.
* Links against `stm32f100.ld` for proper memory mapping.
* Includes startup objects and `syscalls.c`.

#### Pseudocode:

```makefile
main.elf: main.c stm32f10x.h
    arm-none-eabi-gcc -nostdlib -T stm32f100.ld -o main.elf ...
```

✔ Produces `main.elf`, which is the binary loaded into QEMU.

---

### `stm32f100.ld` – Linker Script

Defines **where in memory** the firmware should be placed.

#### Layout:

```ld
FLASH : ORIGIN = 0x08000000, LENGTH = 64K
RAM   : ORIGIN = 0x20000000, LENGTH = 8K
```

#### Sections:

* `.text` goes to flash (code)
* `.data` and `.bss` go to RAM

✔ Ensures the binary is loaded like it would be on real STM32 hardware.

---

### `syscalls.c` – Syscall Stubs

Implements minimal system calls to satisfy `newlib` (C standard library used by GCC).

#### Typical functions:

* `_write`: Redirects printf output to USART or discards it.
* `_sbrk`: Memory allocation for `malloc`.
* `_exit`, `_close`, etc.

#### Purpose:

Avoids link errors when using basic C features.

---

### `gui.py` – Python GUI

A GUI (written in Python, Tkinter) that connects to the **virtual serial port exposed by QEMU** on TCP port `1234`.

#### Functionality:

* Opens `localhost:1234` using `socket`.
* Sends simple text commands (e.g., `"PING"`, `"START"`).
* Receives and displays responses from the firmware.

#### Pseudocode:

```python
connect_to('localhost', 1234)
show_window()

on_button_click():
    send_command('START_I2C')
    receive_response()
    display_result()
```

 #### This is emulating user (or external) input and observing firmware reactions.

---

### `run_qemu.sh` – QEMU Startup Script

Starts QEMU in headless mode with the STM32 binary and a virtual serial port.

#### Command Breakdown:

```bash
qemu-system-arm \
  -M stm32vldiscovery \
  -nographic \
  -kernel embedded_app/main.elf \
  -serial tcp::1234,server,nowait
```

* `-M stm32vldiscovery`: Loads an STM32F100 model.
* `-serial tcp::1234,server`: Opens a TCP server to emulate USART.
* `-nographic`: Disables graphical output; uses serial-only.

---
