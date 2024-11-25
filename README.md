
# FinalCode
Code Evolution and Subfolders
1. Towerparking (Basic Version Without Debouncing and PWM):
This version implements vertical and horizontal movements for parking and retrieval of vehicles.
Key Features:
Multiple floors (6) and slots (12), with status tracking using sensors.
Basic control of the elevator and side motors for horizontal movements.
No PWM signals for smooth motor control and no debouncing for button presses.
Limitations:
Without PWM, motor speed control is fixed, leading to abrupt movements.
Lack of debouncing could result in multiple triggers for a single button press.
Use Case: Proof of concept to demonstrate overall system capability.
2. Vertikalbevegelse (Simplified Vertical Movement):
Focuses solely on vertical movement for easier testing and debugging.
Key Features:
Only the elevator motor is controlled, allowing the elevator to move up and down between floors.
Simplified logic for parking (moveElevatorUp()) and retrieval (moveElevatorDown()).
Button inputs trigger the corresponding elevator movement.
Advantages:
Easier to test without horizontal motors or additional sensors.
Simplified structure for debugging the elevator movement.
Use Case: Testing the elevator functionality independently before integrating other components.
3. UpdatedCode (Final Optimized Version):
The last version of the code developed before the coding responsibilities were transitioned.
Key Features:
Integration of vertical and horizontal movements using PWM for smooth control.
Debouncing for buttons to ensure accurate and reliable user input handling.
Full system functionality, including sensor integration for slot availability.
Optimized logic to prevent motor overlap (e.g., locking mechanisms during transitions).
Role in Project: This version was the closest to the final system setup, enabling a working prototype.
Project Contributions and Changes
Early Development: Focused on coding the basic functionality (e.g., vertical and horizontal movements).
Later Stages: Assisted the team (Azi) by providing ideas, setting up the physical system, and troubleshooting rather than directly coding.
Structure and Files
Towerparking.ino: The first-stage code for vertical and horizontal movements without debouncing or PWM.
Vertikalbevegelse.ino: Simplified code for vertical movement testing.
UpdatedCode.ino: The final optimized version with full functionality, including debouncing and PWM.
Final Remarks
This project demonstrates the evolution of an elevator parking system from a basic concept to a functional prototype. While the physical setup imposed some challenges, the team managed to implement a system capable of vertical and horizontal movements. Future improvements could include further optimization of the code and expansion to accommodate more floors and slots.


