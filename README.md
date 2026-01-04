# Console Othello (Reversi)

A fully playable, console-based implementation of the Othello board game written in C++. 
This project focuses on manual memory management (no vectors) and features a colorful UI using ANSI escape codes.

## Features

- **Game Modes:**
  - **Single Player:** Play against a CPU Bot.
  - **Two Player:** Local PvP on the same keyboard.
- **Dynamic Board Size:** Selectable board dimensions: **6x6**, **8x8**, or **10x10**.
- **Interactive UI:**
  - Cursor-based navigation (no typing coordinates).
  - Valid move indicators (hints).
  - ANSI colors for a clean visual experience.
- **Save System:** Matches are automatically logged to `history.txt` with timestamps and scores.
- **Technical Implementation:**
  - Built using dynamic arrays (`int**`) and manual memory management.
  - No `std::vector` or `auto` used (strictly typed).

## Controls

| Key | Action |
| :---: | :--- |
| **W / A / S / D** | Move Cursor |
| **Enter** | Place Disc / Confirm |
| **Space** | Quit to Menu |

## How to Compile & Run

This project relies on Windows-specific libraries (`conio.h`, `windows.h`) for input handling and console manipulation.

1. **Clone the repository:**
   ```bash
   git clone https://github.com/Matin-Ardestani/Othello.git
   ```
1. **Run the game:**
   ```bash
   g++ main.cpp -o othello
   ```
   ```bash
   othello.exe
   ```

## Some Shots From The Game
<img width="572" height="511" alt="Screenshot 2026-01-04 163916" src="https://github.com/user-attachments/assets/8d02cce9-7360-49bd-9e23-80f336a0cda5" />
<img width="1121" height="418" alt="Screenshot 2026-01-04 163933" src="https://github.com/user-attachments/assets/6394239b-e9aa-488f-9c76-306e19b329aa" />
<img width="596" height="436" alt="Screenshot 2026-01-04 163959" src="https://github.com/user-attachments/assets/e296bf72-a3e3-4f9d-835f-89c99db2c653" />
<img width="534" height="445" alt="Screenshot 2026-01-04 164023" src="https://github.com/user-attachments/assets/1da1d9f8-837e-46e0-a94e-c1d93228d59d" />
<img width="519" height="470" alt="Screenshot 2026-01-04 164059" src="https://github.com/user-attachments/assets/584a11cb-9c0e-4cec-814d-c9e41fe9d4a4" />
