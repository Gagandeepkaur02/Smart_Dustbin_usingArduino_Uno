# Smart_Dustbin_usingArduino_Uno
# Smart Dustbin using Arduino, Ultrasonic Sensor & Servo Motor

An **automatic contactless dustbin** that opens its lid when an object (like your hand) is detected nearby, and closes it after a few seconds.  
Built with **Arduino Uno**, **HC-SR04 Ultrasonic Sensor**, and **SG90 Servo Motor** — perfect for STEM learning, DIY home automation, and hygiene-friendly waste disposal.

---

## 📜 Features
- **Touchless operation** — opens when hand/object is detected within set distance
- **Automatic closing** after a preset delay
- **Adjustable trigger distance**
- **Compact design** suitable for school projects or exhibitions

---

## 🛠 Components Required
| Component | Quantity | Description |
|-----------|----------|-------------|
| Arduino Uno / Nano | 1 | Microcontroller board |
| HC-SR04 Ultrasonic Sensor | 1 | Distance measurement |
| SG90 Servo Motor | 1 | Controls dustbin lid |
| Jumper Wires | As needed | For connections |
| Breadboard (Optional) | 1 | For prototyping |
| Power Supply (5V) | 1 | USB or battery pack |

---

## 🔌 Pinout
**Ultrasonic Sensor (HC-SR04):**
- VCC → 5V  
- GND → GND  
- TRIG → **D5**  
- ECHO → **D6**

**Servo Motor (SG90):**
- Signal → **D9**  
- VCC → 5V  
- GND → GND

---

## 📂 Circuit Diagram
(Add your Fritzing image or circuit diagram here)

---

## 💻 Arduino Code
The Arduino sketch is included in this repository under `smart_dustbin.ino`.

**Main Logic:**
1. Read distance from ultrasonic sensor
2. If distance < trigger threshold → Open lid
3. Wait for set time → Close lid

---

## ⚙️ How It Works
The **HC-SR04** sensor sends ultrasonic pulses and measures the echo time to calculate distance.  
When an object is detected within the set threshold (e.g., 25 cm), the **SG90 Servo Motor** rotates to lift the lid.  
After no detection for a preset time, the lid closes automatically.

---


