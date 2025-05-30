# STM32 Bitbanged I2C Simulation with QEMU and GUI

## 🧠 Overview

This project demonstrates a basic embedded application for an STM32F100 microcontroller running under QEMU. The firmware is written in C and intended to simulate bitbanged I2C communication. A Python-based GUI interfaces with the running QEMU instance to monitor or control aspects of the simulation.

It includes:
- A minimal embedded C project for STM32
- QEMU-based ARM Cortex-M3 simulation
- Python GUI that connects to the simulated system
- Demonstration of communication constraints when QEMU is not running
> You can check ```pseudocode.md``` for a detailed description of each file and a pseudo-code snippet
---

## 🖥️ Environment

| Tool             | Version / Details              |
|------------------|--------------------------------|
| OS               | Ubuntu 22.04 LTS               |
| QEMU             | 6.2.0 (or any Cortex-M capable)|
| ARM GCC Compiler | `arm-none-eabi-gcc` 10.3.1     |
| Python & TKinter | 3.10+                          |
| STM32 Model      | STM32F100 (Cortex-M3)          |

Use the following commands to check your environment:

```bash
lsb_release -a               # Linux distribution version
qemu-system-arm --version    # QEMU version
arm-none-eabi-gcc --version  # ARM compiler version
python3 --version            # Python version
````

---

## 📂 Project Structure

```
simulator/
├── embedded_app/
│   ├── main.c             # Main firmware
│   ├── stm32f10x.h        # MCU definitions
│   ├── Makefile           # Build instructions
│   ├── stm32f100.ld       # Linker script
│   └── syscalls.c         # Minimal syscall stubs
├── gui/
│   └── gui.py             # Python GUI
└── run_qemu.sh            # QEMU launcher
```

---

## ▶️ How to Compile & Run

### Compile the Embedded App & Run

From the project root:

```bash
./run_qemu.sh
```

Make sure this script is executable:

```bash
chmod +x run_qemu.sh
```

### 2. In a detached terminal, run the GUI

From the `gui/` directory:

```bash
python3 gui.py
```

If the QEMU simulation is running, the GUI should connect and show simulated output. If QEMU is **not** running, you should see a connection error or message indicating failure.

---

## 📸 Screenshots

### ✅ When QEMU is running

![Screenshot from 2025-05-30 13-22-24](https://github.com/user-attachments/assets/c44a82c3-3213-4090-8758-0cb561195e4c)


---

### ❌ When QEMU is **not** running

![Screenshot from 2025-05-30 13-22-54](https://github.com/user-attachments/assets/cc7ea90a-49c9-48e4-aa5c-97eaa37cb471)

---

## 📌 Notes

* The firmware is compiled using `arm-none-eabi-gcc`, targeting the STM32F100.
* The simulation uses QEMU in headless mode (running without a graphical window) with communication via USART emulated (Open a TCP server on port 1234, which emulates the STM32's USART/serial port).
* The Python GUI is a simple client for interfacing with the running simulation.

---

## 🔧 Dependencies

Ensure the following are installed:

```bash
sudo apt update
sudo apt install qemu-system-arm gcc-arm-none-eabi make python3 python3-pyqt5
```

## 🛠️ Dev Notes

- After fighting through 100 compatibility issues and switching Linux versions, I stopped committing to Git and decided to make it functional first... then upload. LOL.
- QEMU can be picky with STM32 configs (I even had to reinstall ubuntu for it). Trial and error was the real teacher here.

