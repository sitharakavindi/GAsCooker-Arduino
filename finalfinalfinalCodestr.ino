#include <ESP8266WiFi.h>
#include <ESP8266Firebase.h>
#include <Servo.h>

// =====================
// WiFi & Firebase
// =====================
#define _SSID         "TharaRedmi12"
#define _PASSWORD     "tharavindi"
#define REFERENCE_URL "url"

const String USER_ID = "....";

Firebase firebase(REFERENCE_URL);

// =====================
// Hardware
// =====================
#define SERVO_PIN        D5
#define SPARK_PIN        D4        // HIGH VOLTAGE SPARK PIN
#define FLAME_SENSOR_PIN A0        // Analog flame sensor

Servo gasServo;

// Firebase paths
String flameLevelPath   = "users/" + USER_ID + "/cooker_control/flameLevel";
String isOnPath         = "users/" + USER_ID + "/cooker_control/isOn";

// NEW: flame status paths for Flutter card
String flameDetectedPath = "users/" + USER_ID + "/cooker_control/flameDetected";
String flameStatusPath   = "users/" + USER_ID + "/cooker_control/flameStatus";

// state tracking
bool lastIsOn = false;


const int FLAME_THRESHOLD = 500;

// =====================
// Convert Firebase weird strings to bool
// =====================
bool parseIsOn(String raw) {
  raw.trim();
  raw.toLowerCase();

  if (raw == "true" || raw == "1") return true;
  if (raw == "false" || raw == "0") return false;

  // library truncation handling
  if (raw.indexOf("ru") >= 0) return true;   // "ru" from true
  if (raw.indexOf("al") >= 0) return false;  // "als" from false

  return false;
}

// =====================
// Get Servo Angle
// =====================
int getServoAngle(String level) {
  if (level == "High")   return 100;
  if (level == "Medium") return 50;
  return 0;
}

// =====================
// Flame detection
// =====================
bool isFlameDetected() {
  int value = analogRead(FLAME_SENSOR_PIN);
  Serial.print("[FLAME] Sensor value = ");
  Serial.println(value);

  // For YOUR sensor: 616 (no flame) should be treated as NO.
  // So we consider flame detected when the value is LOWER than threshold.
  bool detected = (value < FLAME_THRESHOLD);

  Serial.print("[FLAME] Detected? ");
  Serial.println(detected ? "YES" : "NO");

  return detected;
}

// small helper to update status fields
void setFlameStatus(bool detected, const String &msg) {
  firebase.setString(flameDetectedPath, detected ? "true" : "false");
  firebase.setString(flameStatusPath,   msg);
}

// =====================
// Setup
// =====================
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.println("=== Gas Cooker Controller ===");

  gasServo.attach(SERVO_PIN, 500, 2500);
  gasServo.write(0);

  pinMode(SPARK_PIN, OUTPUT);
  digitalWrite(SPARK_PIN, LOW);

  pinMode(FLAME_SENSOR_PIN, INPUT);

  // WIFI
  WiFi.begin(_SSID, _PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // initial status (no cooking, no flame)
  setFlameStatus(false, "Stop Cooking. No flame.");
}

// =====================
// Loop
// =====================
void loop() {

  // ===== READ isOn =====
  String raw = firebase.getString(isOnPath);
  bool isOn = parseIsOn(raw);

  Serial.print("isOn = ");
  Serial.println(isOn ? "TRUE" : "FALSE");

  // Detect transition: OFF -> ON
  bool justTurnedOn = (isOn && !lastIsOn);

  // ===== If OFF: move servo to 0, no sparks =====
  if (!isOn) {
    gasServo.write(0);
    digitalWrite(SPARK_PIN, LOW);
    lastIsOn = false;

    // update flame status when cooker is off
    setFlameStatus(false, "Cooking stopped. No flame.");

    delay(400);
    return;
  }

  // ===== If ON: read flame level and move servo =====
  String flameLevel = firebase.getString(flameLevelPath);
  flameLevel.trim();
  Serial.print("flameLevel = ");
  Serial.println(flameLevel);

  int targetAngle = getServoAngle(flameLevel);
  Serial.print("Servo angle -> ");
  Serial.println(targetAngle);
  gasServo.write(targetAngle);

  // ===== SPARK ONLY WHEN isOn becomes TRUE =====
  if (justTurnedOn) {
    Serial.println("🔥 isOn became TRUE -> Spark sequence starting...");

    delay(2000);  // wait 2 seconds before first spark

    // ---- First spark ----
    Serial.println("🔥 SPARK 1 (1.5s)");
    digitalWrite(SPARK_PIN, HIGH);
    delay(500);
    digitalWrite(SPARK_PIN, LOW);
    Serial.println("🔥 Spark 1 OFF");

    // Check flame
    bool flame = isFlameDetected();

    if (flame) {
      // ✅ flame detected after first spark
      setFlameStatus(true, "flame is detect.continue cooking.");
    } else {
      // ❌ no flame yet
      setFlameStatus(false, "No flame detected. Trying again.");
    }

    // ---- If still no flame, wait 2s and spark again ----
    if (!flame) {
      Serial.println("❌ No flame after first spark -> waiting 2s for second spark");
      delay(2000);

      Serial.println("🔥 SPARK 2 (0.5s)");
      digitalWrite(SPARK_PIN, HIGH);
      delay(500);
      digitalWrite(SPARK_PIN, LOW);
      Serial.println("🔥 Spark 2 OFF");

      // Re-check flame
      flame = isFlameDetected();
      if (flame) {
        setFlameStatus(true, "flame is detect.continue cooking.");
      } else {
        setFlameStatus(false, "No flame detected. Please check gas supply.");
        Serial.println("⚠️ Still no flame after second spark (no auto shutdown in this version).");
      }
    }
  } else {
    // Cooker already ON: periodically refresh flame status for UI
    bool flame = isFlameDetected();
    if (flame) {
      setFlameStatus(true, "flame is detect.continue cooking.");
    } else {
      setFlameStatus(false, "No flame detected while cooking.");
    }
  }

  lastIsOn = isOn;

  delay(400);
}
