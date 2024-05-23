# Automatic Vending Machine

This project encompasses the development of an automatic vending machine controlled by an Arduino Mega 2560 microcontroller. The machine integrates various components to provide a seamless vending experience, offering user authentication, product selection, payment processing, and product dispensing functionalities.
##

### Components Overview
**Arduino Mega 2560:** \
The Arduino Mega 2560 serves as the brain of the vending machine, coordinating the operation of all other components and executing the programmed logic for user interaction and product management.

**20x4 LCD Display:** \
The LCD display provides a user-friendly interface for displaying information such as product selections, payment status, and system messages. Its 20x4 character configuration ensures ample space for clear and concise communication with users.

**Stepper Motors:** \
Two stepper motors are employed for product dispensing purposes. These motors enable precise control over the rotation of dispensing mechanisms, ensuring accurate product delivery to users.

**4x4 Membrane Matrix Keypad:** \
The membrane matrix keypad facilitates user input, allowing customers to navigate through product options, confirm selections, and enter payment information. Its compact design and intuitive layout make it easy for users to interact with the vending machine.

**Servo Motor:** \
A servo motor is utilized for auxiliary functions such as opening and closing the product dispensing mechanism. Its precise angular control enables smooth and reliable operation, enhancing the overall user experience.

**RFID Reader RC522:** \
The RFID reader RC522 enables secure user authentication using RFID cards. By scanning and validating RFID credentials, the vending machine ensures that only authorized users can access its features and make purchases.

**Protoboard:** \
The protoboard serves as a platform for assembling and connecting the electronic components of the vending machine. It provides a convenient and organized layout for wiring and ensures reliable electrical connections between components.
##

### Features and Functionality
**User Authentication:**
- Users can authenticate themselves by presenting RFID cards to the RFID reader RC522.
- Only users with valid RFID credentials are granted access to the vending machine's functionalities.

**Product Selection:**
- The membrane matrix keypad enables users to browse through available products and make selections.
- The LCD display provides real-time feedback on product options, helping users make informed choices.

**Payment Processing:**
- Upon selecting a product, users can enter payment information using the membrane matrix keypad.
- The vending machine verifies payment details and deducts the appropriate amount from the user's account balance.

**Product Dispensing:**
- Once payment is confirmed, the vending machine initiates the dispensing process using the stepper motors and servo motor.
- Products are dispensed accurately and efficiently, ensuring a seamless transaction experience for users.

**Administration Mode:**
- An administration mode is available for system maintenance and management tasks.
- Authorized personnel can configure product prices, use this mode to add new RFID cards, and change their balance..
##

### Testing
Unit tests are provided for individual components to ensure proper functionality. Additionally, integration tests can be performed to validate the system's operation as a whole. Refer to the provided test scripts and documentation for testing instructions.
##

### Installation
To set up the system, follow these steps:

**Hardware Setup:**
- Connect the components according to the provided pin configurations and physical setup.

**Software Installation:**
- Clone this repository to your local machine.
- Open the Arduino IDE and import the necessary libraries for the components used.
- Compile and upload the code to your Arduino board.
##

### Usage
Once the system is installed, follow these steps to use it:
- Power on the vending machine.
- Follow the instructions displayed on the LCD screen.
- Use the keypad to select products, add balance, or perform administrative tasks.
- Complete the payment process using an RFID card.
- Collect the dispensed product and any change, if applicable.
##

### References and Tutorials
- [Automatic Vending Machine with RFID and NodeMCU](https://blog.eletrogate.com/maquina-de-vendas-automatica-com-rfid-e-nodemcu/)
- [Arduino Tutorial Controlling a Stepper Motor](https://www.aranacorp.com/pt/controlar-um-motor-de-passo-com-arduino/#google_vignette)
- [Using Membrane Matrix Keypad with Arduino](https://www.robocore.net/tutoriais/usando-teclado-matricial-com-arduino)
- [Interfacing RFID RC522 with Arduino MEGA](https://www.instructables.com/Interfacing-RFID-RC522-With-Arduino-MEGA-a-Simple-/)
- [How to Use Servo Motor with Arduino](https://blog.fazedores.com/como-usar-servo-motor-com-arduino/)
##

### Documentation

In the `docs` folder of this repository, you can find comprehensive documentation for this project in both Portuguese and English:

- [Portuguese Documentation (PDF)](docs/Documentação.pdf)
- [English Documentation (PDF)](docs/Documentation.pdf)

Additionally, a video detailing the development process of this project is available on YouTube:
[Development Process Video](https://youtu.be/4j8oVVgXNdc)

Feel free to explore these resources for a deeper understanding of the project's implementation and functionality.

##
### License
This project is licensed under the MIT License - see the LICENSE file for details.

<br>

> [!NOTE]
> **1.** If using our code, ensure to configure the pins correctly according to the physical setup of the components.\
> **2.** The provided tests help to verify the functionality of individual components.
