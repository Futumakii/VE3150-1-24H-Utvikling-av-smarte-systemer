# Elevator Parking System
This project is about creating an elevator parking system that automates the parking of cars by moving them vertically and horizontally across multiple floors and slots.

# Code Versions
1. Towerparking (First Version): The basic version that controls vertical and horizontal movement but doesn’t use advanced features like PWM or debouncing. 
* What it Does:
*   Moves the elevator vertically and cars horizontally to park or retrieve them.
*   Tracks parking slot availability with sensors.
* Features:
*   Controls 6 floors and 12 parking slots (2 per floor).
*   No PWM, so motor speed is fixed.
*   No debouncing, which might cause buttons to act multiple times with one press.
*   Why It’s Useful: Shows how the system can handle both vertical and horizontal movements.
2. Vertikalbevegelse (Vertical Movement Only): A simplified version for testing only the elevator’s up and down movement.
* What it Does:
*  Focuses on testing the elevator movement (up and down) only.
* Features:
*  Simple controls for moving the elevator up or down using buttons.
*  Does not include sensors or horizontal movement.
*  Why It’s Useful: Makes testing and debugging the elevator easier.

3. UpdatedCode (Final Version): The most advanced version before the coding responsibility was passed on to Azi.
* What it Does:
*   Combines vertical and horizontal movements with smoother motor control.
*   Uses debouncing to ensure buttons work reliably.
* Features:
*   Full system functionality with PWM for smooth motor control.
*   Checks slot availability with sensors.
*   Prevents overlapping actions by ensuring one motor runs at a time.
*   Why It’s Important: It was the closest version to the final working system.

# Contributions
* Early Work: I wrote the first versions of the code, focusing on basic features.
* Later Stages: I helped Azi by sharing ideas, troubleshooting, and setting up the physical system.

# Files
* Towerparking.ino: The first version with vertical and horizontal movement, but no PWM or debouncing.
* Vertikalbevegelse.ino: A simplified version for testing elevator movement only.
* UpdatedCode.ino: The final version with full functionality, including PWM and debouncing.

# Final Notes

The project was simplified due to limitations and now supports vertical movement for one spot and floor, with room to expand.