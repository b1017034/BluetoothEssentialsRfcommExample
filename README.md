# BluetoothEssentialsRfcommExample
Example from the book "Bluetooth essentials for programmers" by Albert S. Huang chapter "3.2 RFCOMM Sockets".

## libraries

```
sudo apt-get install bluez
sudo apt-get install bluetooth blueman
sudo apt-get install libbluetooth-dev
```

### install WiringPi
```
git clone git://git.drogon.net/wiringPi
cd wiringPi
./build
```

## Pairing

```
sudo bluetoothctl -a
scan on // start scaning addresses
scan off
pair <MAC address>
trust <MAC address> 
```

```
sudo vi /etc/systemd/system/dbus-org.bluez.service
```

add and change

```
ExecStart=/usr/lib/bluetooth/bluetoothd
ExecStartPost=/usr/bin/sdptool add SP
```

## Building

On one side
```
make client
```
On the other side
```
make server
```

## Running

On one side execute 
```
./rfcomm-client
```
On the other side execute 
```
./rfcomm-server
```

## Result

On one side
```
$ ./rfcomm-client
$
```
On the other
```
$ ./rfcomm-server 
accepted connection from 12:34:56:78:90:AB
received [hello!]
$
```
Where "12:34:56:78:90:AB" is a bluetooth address of the first side.

## P.S.
Set the approprite bluetooth address in rfcomm-client.c in line 
```
char dest[18] = "01:23:45:67:89:AB";
```
Server must be run prior to client.

## reference
https://qiita.com/akihiro01051/items/2f8336985f15f8d1b131
