Here is a **GitHub-ready README.md** for your **ESP8266 Smart Gas Cooker Controller project**. You can copy and paste this directly into your repository.

---

# 🔥 Smart Gas Cooker Controller (ESP8266 + Firebase)

This project is an **IoT-based smart gas cooker controller** built using **ESP8266**, **Firebase Realtime Database**, a **servo motor**, and a **flame sensor**.

The system allows users to **remotely control a gas stove** using a mobile application (e.g., Flutter) while automatically detecting whether a flame is present and attempting ignition using a **high-voltage spark module**.

The project improves **safety and convenience** by monitoring flame status and updating it in **Firebase in real time**.

---

# ⚙️ Features

* 📡 **WiFi connectivity using ESP8266**
* 🔥 **Automatic flame ignition using spark module**
* 🔍 **Flame detection using analog flame sensor**
* 🔄 **Real-time Firebase synchronization**
* 🎛 **Servo motor gas valve control**
* 📱 **Remote control via mobile app**
* ⚠ **Flame safety monitoring**

---

# 🧰 Hardware Requirements

| Component                 | Description                 |
| ------------------------- | --------------------------- |
| ESP8266 (NodeMCU)         | Microcontroller with WiFi   |
| Servo Motor               | Controls gas valve position |
| Flame Sensor              | Detects presence of flame   |
| High Voltage Spark Module | Generates ignition spark    |
| Jumper Wires              | Circuit connections         |
| Breadboard                | Prototyping                 |

---

# 🧑‍💻 Software Requirements

* Arduino IDE
* ESP8266 Board Package
* Firebase Realtime Database
* Required Arduino Libraries:

  * `ESP8266WiFi`
  * `ESP8266Firebase`
  * `Servo`

---

# 📂 Project Workflow

The system works in the following steps:

1️⃣ ESP8266 connects to **WiFi network**

2️⃣ ESP8266 reads commands from **Firebase**

3️⃣ If the user turns **cooker ON**

* Servo opens the gas valve
* Spark module attempts ignition

4️⃣ Flame sensor checks if flame is detected

5️⃣ Status updates are sent to **Firebase**

6️⃣ Mobile app displays real-time **flame status**

---

# 📁 Firebase Database Structure

Example Firebase structure used in this project:

```
users/
   USER_ID/
      cooker_control/
         flameLevel: "High / Medium / Low"
         isOn: true / false
         flameDetected: true / false
         flameStatus: "status message"
```

---

# 🔌 Pin Configuration

| Component    | ESP8266 Pin |
| ------------ | ----------- |
| Servo Motor  | D5          |
| Spark Module | D4          |
| Flame Sensor | A0          |

---

# ⚡ Flame Detection Logic

The flame sensor produces **analog values**.

Typical readings:

| Condition     | Sensor Value |
| ------------- | ------------ |
| Flame Present | Low value    |
| No Flame      | High value   |

Threshold used in this project:

```
FLAME_THRESHOLD = 500
```

If the sensor value is **less than 500**, flame is considered **detected**.

---

# 🔥 Ignition Process

When the cooker is turned ON:

1️⃣ Servo opens gas valve
2️⃣ Wait **2 seconds**

3️⃣ First spark attempt

* Spark duration: **0.5 seconds**

4️⃣ Flame sensor checks detection

If no flame is detected:

5️⃣ Wait **2 seconds**

6️⃣ Second spark attempt

If still no flame:

⚠ System reports **"No flame detected. Please check gas supply."**

---

# 📡 WiFi Configuration

Edit the following variables in the code:

```cpp
#define _SSID "YourWiFiName"
#define _PASSWORD "YourWiFiPassword"
#define REFERENCE_URL "YourFirebaseDatabaseURL"
```

---

# 🧠 Firebase Paths Used

| Path            | Purpose                     |
| --------------- | --------------------------- |
| `flameLevel`    | Desired flame intensity     |
| `isOn`          | Cooker ON/OFF control       |
| `flameDetected` | Boolean flame detection     |
| `flameStatus`   | Human-readable flame status |

---

# ▶️ How to Run the Project

### 1️⃣ Install Arduino Libraries

Install required libraries in **Arduino IDE**.

---

### 2️⃣ Configure WiFi & Firebase

Update the following in the code:

* WiFi SSID
* WiFi password
* Firebase URL
* USER_ID

---

### 3️⃣ Upload Code

Select board:

```
NodeMCU 1.0 (ESP8266)
```

Upload the code to the ESP8266.

---

### 4️⃣ Power the Circuit

After powering the ESP8266:

* It connects to WiFi
* Syncs with Firebase
* Waits for commands

---

# 📱 Mobile App Integration

The system is designed to work with a **mobile application (e.g., Flutter)** that:

* Controls cooker ON/OFF
* Selects flame level
* Displays flame detection status

Example UI card:

```
🔥 Flame Status
Flame detected – Continue cooking
```

---

# 🔒 Safety Considerations

⚠ Gas control systems require careful design.

Recommended improvements:

* Gas leak sensor
* Auto shutoff if flame fails
* Temperature monitoring
* Manual emergency stop

---

# 🚀 Future Improvements

* Add **gas leak detection**
* Add **automatic gas shutoff**
* Integrate **mobile notifications**
* Add **AI-based cooking monitoring**
* Deploy with **secure Firebase authentication**

---

# 👩‍💻 Author

**Sithara Kavindi**

