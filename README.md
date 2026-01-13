# Arduino Uno R4 WiFi - IoT LED Control (Web, BLE & E-Paper)

Αυτό το έργο είναι ένα ολοκληρωμένο σύστημα IoT που επιτρέπει τον έλεγχο ενός LED χρησιμοποιώντας **WiFi (Web Server)** και **Bluetooth Low Energy (BLE)** ταυτόχρονα. Η τρέχουσα κατάσταση του συστήματος απεικονίζεται γραφικά σε μια οθόνη **E-Paper**.

## 🚀 Δυνατότητες

* **Hybrid Control:** Έλεγχος του LED είτε μέσω Web Interface είτε μέσω Bluetooth (BLE).
* **Real-time Sync:** Η κατάσταση συγχρονίζεται αυτόματα. Αν αλλάξετε το LED από το Web, ενημερώνεται το BLE και η οθόνη (και αντίστροφα).
* **Web Server:** Φιλοξενεί ιστοσελίδα με CSS και JavaScript απευθείας από το Arduino.
* **E-Paper Feedback:** Οπτική επιβεβαίωση της κατάστασης (ON/OFF) με γραφικά σε οθόνη ηλεκτρονικής μελάνης.

## 🛠️ Απαιτούμενος Εξοπλισμός

1.  **Arduino Uno R4 WiFi**
2.  **Οθόνη E-Paper** (SPI interface, συμβατή με `GxEPD2`, π.χ. 1.54").
3.  Καλώδια σύνδεσης (Jumper wires).

## 🔌 Συνδεσμολογία (Wiring)

Η οθόνη συνδέεται μέσω πρωτοκόλλου SPI. Ακολουθήστε τον παρακάτω πίνακα για τη σύνδεση με το Arduino Uno R4 WiFi:

| Pin Οθόνης | Pin Arduino | Περιγραφή |
| :--- | :--- | :--- |
| **VCC** | **3.3V** | ⚠️ *Προσοχή: Μην συνδέσετε στα 5V!* |
| **GND** | **GND** | Γείωση |
| **DIN / MOSI**| **11** | Δεδομένα SPI |
| **CLK / SCK** | **13** | Ρολόι SPI |
| **CS** | **10** | Chip Select |
| **DC** | **5** | Data/Command |
| **RST** | **6** | Reset |
| **BUSY** | **7** | Busy Status |
| **LED** | **4** | Εξωτερικό LED (Active High) |

> **Σημείωση:** Χρησιμοποιούμε το Pin 4 για το LED ώστε να αποφύγουμε τη σύγκρουση με το Pin 13 (SCK) που είναι συνδεδεμένο στο ενσωματωμένο LED.

## 📚 Απαιτούμενες Βιβλιοθήκες

Εγκαταστήστε τις ακόλουθες βιβλιοθήκες μέσω του Arduino Library Manager:

* `WiFiS3`
* `ArduinoBLE`
* `GxEPD2`
* `Adafruit GFX`

## ⚙️ Εγκατάσταση & Ρύθμιση

1.  **Δημιουργία Αρχείου Secrets:**
    Στον φάκελο του έργου, δημιουργήστε (αν δεν υπάρχει) το αρχείο `arduino_secrets.h` και προσθέστε τα στοιχεία του δικτύου σας:
    ```cpp
    #define SECRET_SSID "Το_Όνομα_Του_WiFi_Σας"
    #define SECRET_PASS "Ο_Κωδικός_Του_WiFi_Σας"
    ```

2.  **Upload:**
    Ανοίξτε το `main.cpp` στο Arduino IDE και ανεβάστε το στο board.

3.  **Serial Monitor:**
    Ανοίξτε το Serial Monitor (115200 baud). Θα δείτε την IP διεύθυνση μόλις συνδεθεί.

## 📱 Χρήση

### Μέσω Web Browser
1.  Βρείτε την IP διεύθυνση στο Serial Monitor (π.χ., `192.168.1.50`).
2.  Ανοίξτε έναν browser και πληκτρολογήστε: `http://<IP_ADDRESS>`.
3.  Χρησιμοποιήστε τον διακόπτη στην οθόνη για να ελέγξετε το LED.

### Μέσω Bluetooth (BLE)
Χρησιμοποιήστε μια εφαρμογή όπως το **nRF Connect** ή **LightBlue**:
1.  Σαρώστε για συσκευές και συνδεθείτε στο **"UnoR4_IoT"**.
2.  Βρείτε το Service με UUID: `19B10000-E8F2-537E-4F6C-D104768A1214`.
3.  Επιλέξτε το Characteristic με UUID: `19B10001-E8F2-537E-4F6C-D104768A1214`.
4.  Γράψτε (Write) τιμές:
    * `01` (Hex) -> **ON**
    * `00` (Hex) -> **OFF**

## 📂 Δομή Αρχείων

* `main.cpp`: Κύρια λογική προγράμματος.
* `web_assets.h`: HTML & CSS κώδικας (PROGMEM).
* `display.cpp/h`: Διαχείριση οθόνης E-Paper.
* `arduino_secrets.h`: Κωδικοί WiFi (δεν πρέπει να γίνεται commit στο Git).
