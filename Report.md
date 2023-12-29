**2023 Fall ESLab Final Project - Sky Pointer**
===

**Authors**:
- 陳又華 B10901100
- 邱翊帆 B10901083
- 林雋哲 B10901192

## Outline
- [Abstract](#Abstract)
- [Motivation](#Motivation)
- [Implementation](#Implementation)
- [Achievements](#Achievements)
- [Setup](#Setup)
- [Usage](#Usage)
- [Progress](#Progress)
- [Demo](#Demo)
- [References](#References)


## Abstract
Sky Pointer is a device designed to assist in locating stars in the night sky. It utilizes a combination of motors, a PC, and an STM32 for controlling the device's movement and communicating between the PC and STM32.
## Motivation
During star observation, locating stars can be a challenging task, especially for beginners. Currently, many telescopes come with built-in functionalities facilitated by equatorial mounts to aid in this process. However, these telescopes tend to be expensive and challenging to obtain. Our aim is to explore a more accessible approach to create a device with similar star-finding capabilities with the help of the knowledge we learn in the embedded system class.

## Implementation
### 1. PC side
#### 1.0 Flow chart
![GUI](https://hackmd.io/_uploads/Hy3vEZiv6.png)

#### 1.1 GUI
* Create a GUI using pygame.  
* When the user click the ENTER button, the program will store the star name in a variable and send it to socket.
#### 1.2 Database
* Use [Ninja api](https://api-ninjas.com/) to get the star's RA and DEC.  
* Use `astropy` to convert RA and DEC to Azimuth and Altitude according to the time and place observer at.
#### 1.3 Socket
* Run a socket server to listen to the port.
* Use socket to send the Azimuth and Altitude to the microcontroller.


### 2. STM32 Side
#### 2.0 Flow Chart
![Alt and az from wi-fi](https://hackmd.io/_uploads/By_0UGiv6.png)

#### 2.1 Calibration
Utilizing the 3D accelerometer and 3D gyroscope (LSM6DSL) along with the high-performance 3-axis magnetometer (LIS3MDL), the system determines the bias of *yaw, pitch, roll*, the Euler angles. This calibration process occurs when entering the star's name or upon a recalibration request. The obtained bias values are then applied to calibrate the actual motor rotation needed.

#### 2.2 Calculation
The STM32 receives azimuth and altitude data from the PC via a Wi-Fi socket. It uses the *yaw, pitch, roll* bias and the previous motor state to calculate the required motor rotation, aligning the arrow on the servo motor with the stars.

#### 2.3 Stepper Motor
A Nema 17 stepper motor, coupled with an A4988 driver, is used to rotate the disk carrying another motor and the arrow indicating the star. This motor excels at azimuth control, offering stability under high leverage.

#### 2.4 Servo Motor
The SG90 servo motor is utilized to rotate the arrow pointing to the sky. Its lightweight nature and simplified wiring make it well-suited for altitude control.

## Setup
### 1. wifi set up
Both PC and stm32 need to connect to the same wifi.
### 2. PC side
#### 2.0 Clone the skypointer_pc folder
#### 2.1 Install libraries
```bash
pip install -r requirements.txt
```
#### 2.2 wifi set up
modify the ip address in `main.py` to the pc ip address
```python
    HOST = "YOUR_IP_ADDRESS"
    PORT = YOUR_PORT
```
modify the port address in `source/my_wifi.h` to the port socket is listening to
```c++
        static constexpr size_t REMOTE_PORT = YOUR_PORT;
```
#### 2.3 Database set up
Register an account in [Ninja api](https://api-ninjas.com/), and get the api key.
Replace 
Fill in your api key into `trans.py`
```python
    response1 = requests.get(api_url1, headers={'X-Api-Key': 'YOUR_API_KEY'})
    response2 = requests.get(api_url2, headers={'X-Api-Key': 'YOUR_API_KEY'})
```
#### 2.4 Run
Run main.py, and the program will start to listen to the port and wait for the data from the microcontroller.
GUI will pop up and you can type the star you want to observe in the text box and click ENTER.
### 3. stm32 side
#### 3.1 Clone the skypointer_stm32 folder
#### 3.2 wifi set up
modify the wifi ssid and password in `mbed_app.json`
```json
    "nsapi.default-wifi-ssid": "\"YOUR_WIFI_ID\"",
    "nsapi.default-wifi-password": "\"YOUR_WIFI_PASSWORD\"",
```
modify the ip address in `mbed_app.json` to the pc ip address and port
```json
    "config": {
        "hostname": {
            "value": "\"PC_IP_ADDRESS\""
        },
    }
```
#### 3.3 Run
compile the program using MbedOS 6 and download it to the microcontroller.
### 4. Hardware set up
#### 4.1 Connect the STM32 to stepped motor driver
| Stepper motor driver |STM32 |
| ----- | ------------------ |
| EN    | D1                 |
| MS1   | D2                 |
| MS2   | D3                 |
| MS3   | D4                 |
| STEP  | D10                |
| DIR   | D0                 |
| VDD   | 5V                 |
| GND(near VDD) | GND         |


Also, connect the "RESET" and "SLEEP" on the driver together.

#### 4.2 Connect the stepped motor driver to stepped motor
| driver | Stepper motor |
| ----- | ------ |
| 2B    | Red    |
| 2A    | Blue   |
| 1A    | Green  |
| 1B    | Black  |
#### 4.3 Connect the servo to the STM32
| STM32 | Servo |
| ----- | ----- |
| D6  | Signal   |
| 5V  | VCC      |
| GND | GND      |

## Usage
### 1. PC side
Key in the star you want to observe in the text box and click ENTER.  
If the star is not in the database, pointer will point to polar star.
### 2. stm32 side
#### 2.1 Set up
Once the wifi socket is connected, orange LED will light up.  
Once the the calibration is done, LED 3 will light up.   
Please don't move the microcontroller during the calibration.
#### 2.2 Recalibration
If you want to recalibrate the microcontroller, press the user button on the microcontroller.  
Please do the calibration on flat serface. Our device isn't stable enough to do the calibration on a tilted floor.

#### 2.3 Observe
Once the calibration is done, you can key in the star you want to observe in the text box and click ENTER.  
Sky pointer will point to the star you want to observe.
## Challenges
* Accuracy Issues: Acheiving precise alignment of the pointer with stars might be challenging due to sensor inaccuracies, motor limitations, or calibraiton errors.
* Mechanical Stability: Ensuring the physical stability of the device to minimize unintentional movements impacting accuracy might be crucial.
* Communication Stability: Maintaining stable and reliable communication between the PC and STM32 via Wi-Fi might face dropouts, affecting data transfer and control commands.
## Achievements
The Sky Pointer project demonstrates a remarkable acheivement in creating an accessible device for locating stars in the sky. By integrating hardware components like motors, microcontroller and a PC interface, and through a user friendly UI, calibration, the project not only showcases technical methods but also a thoughtful design catering usability and accuracy in astronomical persuits.
## Presentation
[Final presentation ppt](https://docs.google.com/presentation/d/1YeRcgsimlEcWzX1uLTKFrOCfOVDhEgTQauhfTa8_i14/edit?usp=sharing)
## Demo
[Demo video](https://youtu.be/JGX2cEVmW8k?feature=shared)
# References
##### [Ninja API](https://api-ninjas.com/)
##### [Astropy Docs](https://docs.astropy.org/en/stable/)
##### [How to Calibrate a Magnetometer](https://www.digikey.com/en/maker/projects/how-to-calibrate-a-magnetometer/50f6bc8f36454a03b664dca30cf33a8b)
##### [Stepper Motor Code](https://os.mbed.com/users/melse/code/StepperMotor/)
##### [Nema 17 Tutorial](https://lastminuteengineers.com/a4988-stepper-motor-driver-arduino-tutorial/)
