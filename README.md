# Arduino Temperature Station (ATS)
## About 

ATS is a flexible Arduino-based Temperature Station project, with the following features:

  - Local (Serial) and Remote (TCP) access
  - Actuator to keep the temperature between two threesolds
  - On-the-fly change to temperature threesolds
  - Remote commands to override or manually activate various functions
  - Automatic device report every 5 minutes (3000ms)
  
  >If you need a Multi-platform TCP Client, checkout [STCP]
  
  >This software requires a compatible Ethernet Shield to properly work.

## Currently working commands:

  -Print the device's status  ``` /d ```
  
  -Force-activate the actuator in heating mode  ``` /h ```
  
  -Force-activate the actuator in cooling mode ``` /l ```
  
  -Change the temperature's threesolds ```/s <float min temperature> <float max temperature> ```
  
  -Force the device in idle-mode ``` /m ```
  
  -Re-activate the automatic device report function (in case of forced activation of the actuator or idle-mode)  ``` /c ```
  
 ## Current pin-out
  -Led (Red) ``` 9 ```
  -Led (Yellow) ``` 7 ```
  -Led (Green) ``` 8 ```
  -Conditioning circuit's output (From 0v (5C) to 5v (40C)) ``` A0 ```
  -Actuator ``` TBD ``` 
  


## Configuration


In order to connect the device to a TCP Server, you'll have to manually assign an IP and a MAC to the device and specify the Server's IP.

To do so, just edit the following portion of the code:
``` sh
IPAddress ip(10, 0, 0, 41); //Specify the client's IP
...
IPAddress server(10, 0, 0, 21); //Specify the server's IP
...
#define port 5555 //Specify the server's PORT
...
```
>Remember to assign different IPs and MACs if you plan on deployng more than one device in the same network.

Don't forget to also edit ```Ethernet.init(pin)``` with the CS pin of the Ethernet Shield currently in use:
``` sh
Ethernet.init(10);  // Most Arduino shields
//Ethernet.init(5);   // MKR ETH shield
//Ethernet.init(0);   // Teensy 2.0
//Ethernet.init(20);  // Teensy++ 2.0
//Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
//Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet
```
.


[STCP]: <https://github.com/lorenzocarlini/simple_tcp_server>
