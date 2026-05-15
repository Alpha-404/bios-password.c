A C port of the code at bios-pw.org, im planning to get this to run on microcontrollers at some point like on the flipper zero

compile with `make`


```
➜  C git:(vendorless) ✗ ./keygen
Usage: ./keygen SERIAL

Available keygens:
Solver Name        | Description                                | Example              | Regex       
-------------------+--------------------------------------------+----------------------+---------------
sony               | Sony 7-digit numeric keygen                | 1234567              | ^[0-9]{7}$  
hpmini             | HP Mini 10-character alphanumeric keygen   | CNU1234ABC           | ^[0-9A-Z]{10}$
phoenix            | Generic Phoenix                            | 12345                | ^[0-9]{5}$  
phoenixHP          | HP/Compaq Phoenix BIOS                     | 12345                | ^[0-9]{5}$  
phoenixFSI         | Fujitsu-Siemens Phoenix                    | 12345                | ^[0-9]{5}$  
phoenixFSIModelL   | Fujitsu-Siemens (model L) Phoenix          | 12345                | ^[0-9]{5}$  
phoenixFSIModelP   | Fujitsu-Siemens (model P) Phoenix          | 12345                | ^[0-9]{5}$  
phoenixFSIModelS   | Fujitsu-Siemens (model S) Phoenix          | 12345                | ^[0-9]{5}$  
phoenixFSIModelX   | Fujitsu-Siemens (model S) Phoenix          | 12345                | ^[0-9]{5}$  
hpami              | HP AMI                                     | CNU1234ABX           | ^[0-9ABCDEF]{8}$
acerInsydeKeygen   | Some Acer, HP  laptops. 8 digits           | 0173549286           | ^[0-9]{10}$ 
insydeKeygen       | Insyde H20 (generic)                       | 03133610             | ^[0-9]{8}$  
hpInsydeKeygen     | HP Insyde H2O                              | i 70412809           | [i \d{8}]   
asus               | ASUS (alt+r in bios pw to use)             | 2010-02-03           | ^[0-9]{4}-[0-9]{2}-[0-9]{2}$
dellHdd            | old dell hdd serial keygen (11 char)       | *****789XYZ          | ^[ *]{0,10}[A-Z0-9]{1,11}$
dellServiceTag     | old dell service tag keygen (7 char)       | ABC12**              | ^[A-Z0-9]{1,7}\*{0,6}$
➜  C git:(vendorless) ✗ 
```
