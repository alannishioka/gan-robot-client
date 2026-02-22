/**
 * A BLE client example that is rich in capabilities.
 * There is a lot new capabilities implemented.
 * author unknown
 * updated by chegewara
 *
 * Superficially modified to control GAN magic cube robot V1
 * By Alan Nishioka, January 2026
 *
 * BLE protocol documented at:
 * https://github.com/cubing/cubing.js/blob/main/src/cubing/bluetooth/docs/gan-robot.md
 *
 * Tested with ESP32C3 Dev Module (ESP32 C3 Super Mini)
 * Uses ESP32 BLE library instead of ArduinoBLE
 */

#include "BLEDevice.h"
//#include "BLEScan.h"

// The remote service we wish to connect to.
static BLEUUID serviceUUID("0000fff0-0000-1000-8000-00805f9b34fb");

// The characteristic of the remote service we are interested in.
static BLEUUID charUUID("0000fff3-0000-1000-8000-00805f9b34fb");

static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic *pRemoteCharacteristic;
static BLEAdvertisedDevice *myDevice;

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient *pclient) {}

  void onDisconnect(BLEClient *pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer() {
  Serial.print("Forming a connection to ");
  Serial.println(myDevice->getAddress().toString().c_str());

  BLEClient *pClient = BLEDevice::createClient();
  Serial.println(" - Created client");

  pClient->setClientCallbacks(new MyClientCallback());

  // Connect to the remove BLE Server.
  pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
  Serial.println(" - Connected to server");
  pClient->setMTU(517);  //set client to request maximum MTU from server (default is 23 otherwise)

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our service");

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found our characteristic");

  connected = true;
  return true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
    }  // Found our server
  }  // onResult
};  // MyAdvertisedDeviceCallbacks

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan *pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
}  // End of setup.

// Timing for state of loop scramble/unscramble
int seconds = 0;

// Number of moves to scramble/unscramble
#define MOVES 20
#define PAUSE 20
int moves[MOVES];
byte newValue[32];
char digit[] = "0123456789ABCDEF";

// This is the Arduino main loop function.
void loop() {

  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothing more we will do.");
    }
    doConnect = false;
  }
  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {

    // Scramble cube
    if (seconds == 0)
    {
      // Choose MOVES random numbers
      // Face is bits 3-1, Direction is bit 0
      // Don't move same face forward/backward after each other
      // Only choose face 0-4
      for( int i = 0; i < MOVES; i++ )
      {
          moves[i] = rand() % 10;
          if(( i > 0 ) && ( moves[i] ^ moves[i-1] ) == 1 ) i--;
      }

      // Convert moves[] to newValue[] to send to robot
      // Robot uses one nibble per move
      // Face is 0x0, 0x3, 0x6, 0x9, 0xC
      // Direction is Face+0 or Face+2
      // 180 turn is never used
      // 0xF means end of moves
      for( int i = 0; i < MOVES+1; i++ )
      {
        byte nibble;

        // Format move for robot.  Terminate with 0xF
        if( i < MOVES ) nibble = (( moves[i] >> 1 ) * 3 ) + (( moves[i] & 1 ) * 2 );
        else            nibble = 0xF;

        // Write top nibble first, bottom nibble second
        if( ~i & 1 ) newValue[i/2]  = nibble << 4;
        else         newValue[i/2] |= nibble << 0;
      }
      Serial.print("Scramble   ");
    }

    // Unscramble cube
    if (seconds == 20)
    {
      // Convert moves[] to newValue[] to send to robot
      // Count backwards in moves[] and flip direction
      for( int i = 0; i < MOVES+1; i++ )
      {
        byte nibble;

        // Format move for robot.  Invert direction to unscramble.  Terminate with 0xF
        if( i < MOVES ) nibble = (( moves[MOVES-i-1] >> 1 ) * 3 ) + (( ~moves[MOVES-i-1] & 1 ) * 2 );
        else            nibble = 0xF;

        // Write top nibble first, bottom nibble second
        if( ~i & 1 ) newValue[i/2]  = nibble << 4;
        else         newValue[i/2] |= nibble << 0;
      }
      Serial.print("Unscramble ");
    }

    // Send newValue to robot
    if( seconds == 0 || seconds == 20 )
    {
      // Set the characteristic's value to be the array of bytes that is actually a string.
      pRemoteCharacteristic->writeValue(newValue, MOVES/2+1);
      for( int i = 0; i < MOVES/2+1; i++ )
      {
        Serial.print( digit[((int)newValue[i] >> 4)&0xF] );
        Serial.print( digit[((int)newValue[i] >> 0)&0xF] );
        Serial.print(" ");
      }
      Serial.println("");
    }

    // Increment loop state seconds
    seconds++;

    // Loop seconds
    seconds %= 40;

    Serial.println(seconds);

  } else {
    BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
  }

  delay(1000);  // Delay a second between loops.
}  // End of loop
