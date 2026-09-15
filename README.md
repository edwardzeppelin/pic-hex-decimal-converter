# PIC Microcontroller Hexadecimal to Decimal Converter

A PIC C embedded application that inputs 4-digit hexadecimal numbers via pushbuttons and converts them to decimal in real time on an HD44780 LCD display.

**Features**

* **Interactive Hex Input**: Enter 4-character hexadecimal values (`0000` to `FFFF`) using pushbuttons.
* **Real-time LCD Display**: Displays the Hexadecimal input on line 1 and the converted Decimal value on line 2.
* **Button Debouncing**: Software-based 50 ms delay debouncing for stable button press detection.
* **Dynamic Digit Switching**: Cycle through target digit positions and increment hex values (`0`–`9`, `A`–`F`).

**Hardware & Pinout**

| Component / Pin | Function | Configuration |
| :--- | :--- | :--- |
| **PIC Microcontroller** | System MCU | 20 MHz External Crystal (`_XTAL_FREQ 20000000`) |
| **LCD RS Pin** | Register Select | `RB1` |
| **LCD EN Pin** | Enable Signal | `RB2` |
| **LCD Data Lines** | 4-bit Data Interface | `RB0`–`RB3` |
| **RB4 Button** | Convert | Triggers Hex to Decimal conversion |
| **RB5 Button** | Clear | Resets input to `0000` and clears decimal row |
| **RB6 Button** | Next Position | Shifts cursor position across 4 digits |
| **RB7 Button** | Increase Value | Increments current hex digit (`0`–`F`) |

**Project Structure**

* `config.h`: System clock definitions (20 MHz).
* `main.c` / `cursach.asm`: Main program loop, button handler, debouncing, and Hex-to-Dec conversion logic.
* `LCD.c` / `LCD.h`: LCD initialization, command execution, and character display functions.

**Building & Flashing**

1. Open the project in **MPLAB IDE** or **MPLAB X IDE**.
2. Select the **HI-TECH C** or **XC8** compiler.
3. Ensure `config.h` and `LCD.h` header paths match your project directory.
4. Compile the project to generate the `.hex` binary file.
5. Flash the `.hex` file to your PIC microcontroller using a PICkit programmer.
