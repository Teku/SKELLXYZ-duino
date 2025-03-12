# **Arduino UDP Servo Controller - Explanation**

## **How It Works**
1. **Wi-Fi Setup:**  
   - The ESP32/ESP8266 connects to a specified Wi-Fi network using the provided SSID and password.
   - Once connected, it prints the local IP address to the serial monitor.

2. **Servo Initialization:**  
   - The servos are attached to their designated GPIO pins.
   - Each servo has predefined angle limits to prevent excessive movement.

3. **UDP Listener:**  
   - The ESP32/ESP8266 continuously listens for incoming UDP packets on port `8888`.
   - When a packet is received, the command is extracted and printed to the serial monitor.

4. **Packet Parsing:**  
   - The received command string (e.g., `"B30 P-20 T10 M60"`) is split into individual commands.
   - Each command consists of a servo identifier (`B`, `P`, `T`, `M`) followed by a numerical angle.
   - The numerical angle is constrained within the allowed range for each servo.

5. **Servo Movement:**  
   - The mapped servo position is sent to the corresponding servo.
   - If the requested angle is outside the allowed range, it is clamped, and a warning is printed.
   - The servos move smoothly to their new positions.

---

## **UDP Communication Overview**
### **What is UDP?**
UDP (User Datagram Protocol) is a lightweight, connectionless communication protocol used for fast data transmission. Unlike TCP, it does not establish a connection before sending data, making it ideal for real-time applications like servo control.

### **Why Use UDP?**
- **Low Latency:** Ideal for fast and real-time control.
- **Lightweight:** No need for handshaking, reducing communication overhead.
- **Stateless:** Can receive commands from multiple sources without maintaining a session.

### **How UDP Works in This Code**
1. The ESP32/ESP8266 binds to UDP port `8888`.
2. It waits for incoming data packets.
3. When a packet arrives, it extracts and decodes the servo commands.
4. It then adjusts the servo positions accordingly.

---

## **Required Components**
| Component        | Description |
|-----------------|------------|
| **ESP32/ESP8266** | Microcontroller with Wi-Fi capability to receive UDP commands. |
| **Wi-Fi Network** | Required for UDP communication between the sender and ESP32/ESP8266. |
| **Servos (x4)**  | Connected to GPIO pins to be controlled via UDP. |
| **5V Power Supply** | Required to power the servos, as the ESP32 cannot directly provide enough current. |
| **Jumper Wires** | Used to connect servos to the ESP32/ESP8266. |

---

This setup allows you to remotely control servos over a network using simple UDP packets. 🚀
