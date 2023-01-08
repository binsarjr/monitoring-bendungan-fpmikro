/*
 * Pendeteksi Ketinggian AIR
 *
 * Ketua:
 * 21.11.4002 Binsar Dwi Jasuma
 *
 * Anggota:
 * 21.11.3969 Ilham Mustaqiim
 * 21.11.3978 Muhammad Fayza A S
 * 21.11.4000 Lia ayu lestari
 * 21.11.4033 Adi Dwi Ribowo
 * 21.11.4039 Muhammad Hamdan Zulfa
 *
 * @Ref https://github.com/mobizt/Firebase-ESP-Client/blob/main/examples/Authentications/LegacyTokenAuthen/LegacyTokenAuthen.ino
 * @Ref https://github.com/mobizt/Firebase-ESP-Client/blob/main/examples/RTDB/Basic/Basic.ino
 */

#include <ESP8266WiFi.h>
#include <ESPDateTime.h>
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Konstanta atau Config yang digunakan oleh program ini
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define FIREBASE_URL "YOUR_FIREBASE_URL"
#define FIREBASE_AUTH "YOUR_FIREBASE_AUTH"
#define API_KEY "YOUR_FIREBASE_APIKEY"

unsigned long lastTimeUpdate;
// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

/**
 * Fungsi untuk menghubungkan WiFi melalui konstanta yang sudah disetting di atas
 */
void networkConnection()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Connect to the network
    Serial.print("Connecting to ");
    Serial.print(WIFI_SSID);
    Serial.println(" ...");

    // Menunggu sampai WiFi terhubung
    while (WiFi.status() != WL_CONNECTED)
    { // Tunggu sampai wifi terkoneksi
        delay(500);
        Serial.print(".");
    }

    Serial.println('\n');
    Serial.println("Connection established!");
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP()); // mengirimkan ip add ESP8266 ke serial monitor
}

/**
 * Fungsi untuk menkonfigurasi Akun Firebase
 */
void firebaseSetup()
{
    config.signer.tokens.legacy_token = FIREBASE_AUTH;
    config.database_url = FIREBASE_URL;

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
    // mensetting besaran buffer size yang dikirim
    fbdo.setBSSLBufferSize(1024, 1024);
}

/**
 * Mendapatkan data dengan tipe data integer dari firebase
 */
bool dbGetInt(const char *dataPath, int *store)
{
    bool success = Firebase.RTDB.getInt(&fbdo, dataPath, store);
    Serial.printf("Get int ref... %s\n", success ? String(*store).c_str() : fbdo.errorReason().c_str());
    return success;
}

/**
 * Membuat/mengirim data dengan tipe data integer ke firebase
 */
bool dbSetInt(const char *dataPath, int store)
{
    bool success = Firebase.RTDB.setInt(&fbdo, dataPath, store);
    Serial.printf("Set int ref... %s\n", success ? "OK" : fbdo.errorReason().c_str());
    return success;
}

/**
 * Mendapatkan data dengan tipe data string dari firebase
 */
bool dbGetString(const char *dataPath, const char **store)
{
    bool success = Firebase.RTDB.getString(&fbdo, dataPath);
    if (success)
    {
        *store = fbdo.to<const char *>();
    }
    Serial.printf("Get String ref... %s\n", success ? *store : fbdo.errorReason().c_str());
    return success;
}

/**
 * Membuat/mengirim data dengan tipe data string ke firebase
 */
bool dbSetString(const char *dataPath, const char *store)
{
    bool success = Firebase.RTDB.setString(&fbdo, dataPath, store);
    Serial.printf("Set String ref... %s\n", success ? "OK" : fbdo.errorReason().c_str());
    return success;
}

/**
 * Class Logger digunakan untuk melakukan proses akuisisi data berdasarkan periode waktu tertentu
 * Guna menampilkan data pada tabel dan grafik
 */
class Logger
{
private:
    // Lokasi dbpath di firebase
    const char *dbpath = "datalog";

public:
    Logger()
    {
    }

    /**
     * Method yang membungkus kode program yang berkaitan dengan logger yang mana
     * nantinya akan panggil pada void setup
     */
    void setup()
    {
        // menyiapkan data/variabel firebase dengan tipe data json
        FirebaseJson json;
        // pada saat awalan data emang akan dihapus atau dibuat ulang
        // guna memperkecil ukuran database
        json.setJsonData("{}");
        // cara menyimpan data dalam format json
        Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, dbpath, &json) ? "ok" : fbdo.errorReason().c_str());
    }
    /**
     * Method yang akan digunakan untuk mengirimkan data log yang baru
     */
    void write(const char *datetime, int value)
    {
        String path = String(dbpath) + String("/") + String(datetime);
        dbSetInt(path.c_str(), value);
    }
};

/**
 * Class LED digunakan untuk menghandle object object LED seperti led kuning, merah dan hijau
 * Sehingga tidak perlu repot repot lagi memanggil fungsi fungsi yang tidak begitu jelas arahnya
 */
class LED
{
private:
    int defaultLogic;
    int PIN;
    int kedip = 0;
    int lastUpdateMilis = 0;

public:
    /**
     * Digunakan untuk mendapatkan value logic yang telah disimpan
     * tanpa perlu melakukan koneksi ke firebase lagi
     */
    int logicState;
    LED(int _PIN, int _defaultLogicState = 0)
    {
        PIN = _PIN;
        defaultLogic = _defaultLogicState;
    }
    /**
     * Method yang membungkus kode program yang berkaitan dengan led yang mana
     * nantinya akan panggil pada void setup
     */
    void setup()
    {
        pinMode(PIN, OUTPUT);
    }
    /**
     * set logic HIGH atau LOW
     */
    void write(int _logicState)
    {
        logicState = _logicState;
        digitalWrite(PIN, logicState);
    }
    /**
     * membaca logic yang tersimpan pada object
     */
    int read()
    {
        return logicState;
    }

    /**
     * Method untuk led berkedip menggunakan milis setiap 100ms
     */
    void berkedip()
    {
        // berkedip setiap 100 ms
        if (millis() - lastUpdateMilis > 100)
        {
            kedip = !kedip;
            write(kedip);
            lastUpdateMilis = millis();
        }
    }
};

/**
 * Class WaterLevelSensor digunakan untuk menghandle keseluruhan fungsi yang nantinya akan digunakan
 * oleh water level sensor saja
 */
class WaterLevelSensor
{
private:
    int sensorPin;
    const char *status;
    /**
     * Database path untuk ketintggian air
     */
    const char *dbpath;
    /**
     * Database path untuk status bendungan
     */
    const char *dbpathStatus;
    // Konfigurasi titik ketinggian air
    int maxAman = 400;
    int minSiaga = 400;
    int maxSiaga = 420;
    int minBahaya = 420;
    int maxBahaya = 440;
    int minBencana = 440;
    int persentaseTinggiAir;

public:
    int readValue;
    WaterLevelSensor(int _sensorPin, const char *_dbpath, const char *_dbpathStatus)
    {
        sensorPin = _sensorPin;
        dbpath = _dbpath;
        dbpathStatus = _dbpathStatus;
    }

    /**
     * Method yang membungkus kode program yang berkaitan dengan Water Level Sensor yang mana
     * nantinya akan panggil pada void setup
     */
    void setup()
    {
        // Kirim data apabila belum tersedia
        if (!dbGetInt(dbpath, &readValue))
        {
            dbSetInt(dbpath, readValue);
        }

        // Kirim status apabila belum tersedia
        if (!dbGetString(dbpathStatus, &status))
        {
            dbSetString(dbpathStatus, getStatus());
        }
        // set referensi untuk digunakan oleh website sibendungan.vercel.app
        dbSetInt("/preferences/max_aman", maxAman);
        dbSetInt("/preferences/min_siaga", minSiaga);
        dbSetInt("/preferences/max_siaga", maxSiaga);
        dbSetInt("/preferences/min_bahaya", minBahaya);
        dbSetInt("/preferences/max_bahaya", maxBahaya);
        dbSetInt("/preferences/min_bencana", minBencana);
    }
    /**
     * Digunakan untuk membaca sensor dan menampilkan pada serial monitor
     * titik ketinggian dan persentase ketinggian air
     */
    int read()
    {
        readValue = analogRead(sensorPin);
        // 600 karena menggunakan 3v
        persentaseTinggiAir = map(readValue, 0, 600, 0, 100);
        Serial.print(readValue);
        Serial.print(" ");
        Serial.print(persentaseTinggiAir);
        Serial.println("%");
        return readValue;
    }
    /**
     * Simpan data dan status ke firebase
     */
    void save()
    {
        dbSetInt(dbpath, readValue);
        dbSetString(dbpathStatus, getStatus());
    }

    /**
     * Mengambil status bendungan berdasarkan logic yang telah ditetapkan
     */
    const char *getStatus()
    {
        if (aman())
            return "aman";
        if (siaga())
            return "siaga";
        if (bahaya())
            return "bahaya";
        if (bencana())
            return "bencana";
        return "Belum didefinisikan";
    }

    /**
     * Cek apakah status bendungan aman
     */
    bool aman()
    {
        return readValue < maxAman;
    }

    /**
     * Cek apakah status bendungan siaga
     */
    bool siaga()
    {
        return readValue >= minSiaga && readValue < maxSiaga;
    }

    /**
     * Cek apakah status bendungan bahaya
     */
    bool bahaya()
    {
        return readValue >= minBahaya && readValue < maxBahaya;
    }

    /**
     * Cek apakah status bendungan bencana
     */
    bool bencana()
    {
        return readValue >= minBencana;
    }
};

// buat object ledHijau dari class LED
LED ledHijau(D4, LOW);
// buat object ledKuning dari class LED
LED ledKuning(D3, LOW);
// buat object ledMerah dari class LED
LED ledMerah(D2, LOW);
// buat object waterLevelSensor dari class WaterLevelSensor
WaterLevelSensor waterLevelSensor(A0, "ketinggian-air", "status");
// buat object datalog dari class Logger
Logger datalog;

int intervalTime;
/**
 * Fungsi yang mengatur periode waktu pengiriman/upload data ke firebase
 */
void intervalTimeHandler()
{
    // Ambil waktu upload setiap ms berdasarkan data di firebase
    if (!dbGetInt("upload_setiap_ms", &intervalTime))
    {
        dbSetInt("upload_setiap_ms", 10000); // 10 detik
    }
}

/**
 * Simpan kapan terakhir kali data di upload
 */
void lastUpdateHandler()
{
    dbSetString("last_update_utc", DateTime.formatUTC(DateFormatter::SIMPLE).c_str());
}

/**
 * Setup datetime
 */
void setupDateTime()
{
    DateTime.setTimeZone("Asia/Jakarta");
    DateTime.begin();
    if (!DateTime.isTimeValid())
    {
        Serial.println("Failed to get time from server.");
    }
    else
    {
        Serial.printf("Date Now is %s\n", DateTime.toISOString().c_str());
        Serial.printf("Timestamp is %ld\n", DateTime.now());
    }
}

/**
 * Upload data ke firebase setiap pada waktu yang ditentukan
 * @param int intervalTime
 */
void upload()
{
    // upload data setiap waktu yang ditentukan
    // berdasarkan millis intervalTime
    if (millis() - lastTimeUpdate > intervalTime)
    {
        // proses upload data water level sensor
        waterLevelSensor.save();
        // cek apakah waktu interval update berubah atau tidak
        intervalTimeHandler();
        // upload data sensor atau kondisi saat ini untuk menjadi data log/logger
        datalog.write(DateTime.formatUTC(DateFormatter::SIMPLE).c_str(), waterLevelSensor.readValue);
        // upload kapannterakhir kali data di proses
        lastUpdateHandler();

        // update waktu terakhir kali diupdate menggunakan millis
        lastTimeUpdate = millis();

        Serial.println("Semua data berhasil di upload");
    }
}

int _millisCheckStatus = 0;
int _millisCheckStatusBencana = 0;
/**
 * Mengecek status bendungan lalu memberikan perintah terhadap
 * LED berdasarkan kondisi
 */
void checkStatus()
{
    if (millis() - _millisCheckStatusBencana > 500)
    {

        // semua led berkedip ketika status bencana setiap 500ms
        if (waterLevelSensor.bencana())
        {
            ledHijau.berkedip();
            ledKuning.berkedip();
            ledMerah.berkedip();
        }
        else
        {
            _millisCheckStatusBencana = millis();
        }
    }

    // status di cek setiap .500ms
    if (millis() - _millisCheckStatus > 500)
    {
        if (!waterLevelSensor.bencana())
        {
            // hijau menyala ketika status aman
            ledHijau.write(waterLevelSensor.aman());

            // kuning menyala ketika status siaga
            ledKuning.write(waterLevelSensor.siaga());
            // merah menyala ketika status bahaya
            ledMerah.write(waterLevelSensor.bahaya());
        }
        _millisCheckStatus = millis();
    }
}

void setup()
{
    Serial.begin(9600);
    Serial.println('\n');

    networkConnection();

    setupDateTime();
    firebaseSetup();
    delay(100);
    intervalTimeHandler();
    ledHijau.setup();
    ledKuning.setup();
    ledMerah.setup();
    waterLevelSensor.setup();
    datalog.setup();
}

void loop()
{
    int sensor = waterLevelSensor.read();

    checkStatus();
    upload();
    delay(500);
}
