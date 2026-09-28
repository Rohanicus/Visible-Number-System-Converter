# LCD1602 Number System Converter

An Arduino project that shows a number in binary, octal, decimal, and hexadecimal on a 16×2 character LCD. Type a decimal integer (0–65535) into Serial Monitor to set the value. Press a button to switch formats. The binary display uses all 16 characters on the second row.

## Parts

- Arduino Uno or compatible 5 V board
- Standard 16-pin LCD1602 compatible with the built-in `LiquidCrystal` library (parallel interface)
- Pushbutton
- 10 kΩ potentiometer for LCD contrast
- Breadboard and jumper wires
- Suitable resistor for the LCD backlight if the LCD module does not already include one (commonly 220 Ω; check your module)

**This wiring is for a parallel LCD1602, not an I2C backpack.**

## Wiring

| LCD pin | Connect to |
| --- | --- |
| 1 VSS | Arduino GND |
| 2 VDD | Arduino 5V |
| 3 VO | Center terminal of 10 kΩ potentiometer; outer terminals to 5V and GND |
| 4 RS | Arduino D12 |
| 5 RW | Arduino GND |
| 6 E | Arduino D11 |
| 7–10 D0–D3 | Leave unconnected |
| 11 D4 | Arduino D5 |
| 12 D5 | Arduino D4 |
| 13 D6 | Arduino D3 |
| 14 D7 | Arduino D2 |
| 15 A / LED+ | 5V through appropriate backlight resistor if required by module |
| 16 K / LED− | GND |

Connect one leg of the pushbutton to Arduino **D8** and the other to **GND**. The sketch uses `INPUT_PULLUP`, so no external button resistor is needed. Make sure every component shares ground.

## Upload and use

1. Open `NumberSystemConverter/NumberSystemConverter.ino` in Arduino IDE.
2. Choose your board and port, then click **Upload**. `LiquidCrystal` is included with the standard Arduino IDE.
3. Adjust the contrast potentiometer until the text appears.
4. Open **Serial Monitor**, set it to **9600 baud**, and use **Newline** or **Both NL & CR** as the line ending.
5. Type `42` and press Enter. The first row reads `Decimal: 42`; the second row initially reads `0000000000101010`.
6. Press the button to see `OCT: 52`, `DEC: 42`, `HEX: 2A`, then binary again.

Only nonnegative whole decimal numbers up to 65535 are accepted. Invalid input leaves the current value unchanged.

## How it works

The sketch stores the entered value as a 16-bit unsigned number. A button press selects one of four bases; the sketch converts the number for display. It checks serial input for valid digits and debounces the button so a single press changes the mode once.

## Next steps

- Add a keypad so the converter works without a computer.
- Add input support for binary and hexadecimal numbers.
- Design a PCB after validating the circuit on a breadboard.
