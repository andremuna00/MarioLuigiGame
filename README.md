# 🎮 Mario & Luigi Game
![image](https://github.com/user-attachments/assets/94c675a4-bbb4-460c-8f52-4dc4ccfd99b6)

## 📝 Description
A competitive two-player platformer featuring Mario and Luigi, developed in C++ during my third year of high school. This project combines classic platforming mechanics with a unique competitive element, supporting both keyboard and gamepad controls. Players compete to be the first to hit randomly activated blocks, while dealing with realistic physics-based movement.
## ⭐ Key Features

- Split-control system supporting both keyboard and gamepad input
- Physics-based movement and jumping mechanics
- Random block activation system
- Real-time scoring mechanism
- Dual-player simultaneous gameplay
- Predefined level design with strategic block placement
- Seamless controller hot-plugging

## 🕹️ Controls
### Keyboard
- Player 1 (Mario)
    - A: Move Left
    - D: Move Right
    - W: Jump

- Player 2 (Luigi)
    - Left Arrow: Move Left
    - Right Arrow: Move Right
    - Up Arrow: Jump

- Gamepad
    - Both Players
    - D-Pad/Analog Stick: Movement
    - A Button (Xbox) / X Button (PlayStation): Jump
    - Hot-plugging supported for both controllers

## 🛠️ Technical Implementation
- Physics Engine
    - Gravity simulation for jumping mechanics
    - Collision detection with platforms and blocks
    - Momentum conservation in movement
    - Variable jump heights based on button press duration

- Input Management
    - Multi-device input handling
    - Concurrent keyboard and gamepad support
    - Controller state monitoring
    - Input lag compensation

- GUI System
    - Real-time character animation
    - Dynamic block state visualization
    - Score display system
    - Split-screen capability for practice mode

## ⚙️ Technical Requirements

- C++ Compiler (C++11 or later)
- Minimum Resolution: 1024x768
- USB Controller Ports (for gamepad support)

## 🚀 Installation

Clone the repository:
```bash
git clone https://github.com/yourusername/mario-luigi-jump.git
```
Navigate to project directory:
```bash
cd mario-luigi-jump
```
Compile with required libraries:
```bash
g++ main.cpp -o mario_jump
```
## 🎮 Game Rules

- Both players start on opposite sides of the level
- Blocks randomly become "active" (highlighted) after random intervals
- Players must race to jump and hit the active block first
- First player to hit the active block scores a point
- Game continues until a predetermined score is reached
- Players must manage momentum and position to reach blocks quickly

## 💡 Project Background
This project was developed as a personal exploration in my third year of high school, focusing on:

- Real-time physics implementation
- Multi-input device handling
- GUI development in C++
- Game state management
- Competitive gameplay mechanics

## 🔧 Technical Challenges Solved

- Implementing accurate physics for platform movement
- Managing simultaneous input from multiple devices
- Creating responsive controls with minimal input lag
- Handling controller connection/disconnection events
- Balancing gameplay for different control schemes

## 📜 License
This project is open source and available under the MIT License.
## ✨ Acknowledgments

- Nintendo for the inspiration (Mario & Luigi characters are property of Nintendo)
- Fellow students who helped test the dual-control system
- High school CS department for supporting independent projects
