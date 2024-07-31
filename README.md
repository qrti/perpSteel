# **Perpetual Steel Ball Track**

### As perpetual as the laws of physics of this universe allow
<br>

<img src="images/01_total.jpg" width=480><br>

YouTube Video  
https://www.youtube.com/watch?v=C1I4kvoMqeI

[**-> views**](#views)  
[**-> source**](source)  
<br>

## Background
The idea for building this device came to me while looking at a commercially available version. It was a long journey with much trial and error before the replica finally worked. The track had to be bent from steel wire, parts for the frame, collecting bowl, and supports had to be made with the 3D printer, the electromagnet had to be wound, and the electronics and software had to be developed
<br>

## Build
The building instructions for this project are not complete, they are only intended as an aid for a replica  
[**-> 3D-Printed Parts**](stl)

**Inner and Outer Track Template**  
<img src="stl/00_track_template_inner.png" width=160>
<img src="stl/00_track_template_outer.png" width=160>  
The track was bent from 1.5 mm galvanized iron wire, as this is easy to work with and solder,  
the templates are meant to make bending easier  
<br>

**Cup and Cup Guide**  
<img src="stl/01_cup.png" width=160>
<img src="stl/01_cup_hole_guide.png" width=160>  
To prevent the steel ball from derailing a guide is glued to the outlet of the cup hole  
<br>

**Rail Type 1 and 2**  
<img src="stl/01_cup_rail_1.png" width=160>
<img src="stl/01_cup_rail_2.png" width=160>  
Type 1 is fixed to the rim of the cup, two pieces of type 2 are mounted above,  
to connect cup and rails sticks of steel wire are put in the provided holes  
<br>

**Electro Magnet Bobbin and Magnetic Field Guide Template**  
<img src="stl/02_magnet_bobbin.png" width=160>
<img src="stl/02_magnet_guide.png" width=160>  
The bobbin is completely wound with 0.8 mm enameled copper wire,  
the soft iron core has a diameter of 20 mm and is 16 mm long,  
the magnetic field guide is intended to center the steel ball in front of the magnet,  
it is made from 3 mm thick sheet iron, only the outlines of the template are relevant,  
so the hole in the template is not transferred to the sheet  
(the field guide can possibly be omitted)  
<br>

**Inductive Proximity Sensor and Arduino**  
<img src="images/07_sensor.jpg" height=120>
<img src="images/08_arduino.jpg" height=120>  
This kind of sensor is normally used in 3D-Printers, see the schematic for more details,  
an Arduino Mega 2560 handles timing and delays for sensor and magnet,  
the LCD shows current settings, the keys allow timing changes  
<br>

## Schematic
<img src="images/schematic.png" width=640>
<br>

## Operation
**Right Key**  
press about 1.5 s to enter or exit time and delay settings

**Select Key**  
change values in 100 / 500 / 1000 ms steps

**Left Key - / Right Key +**  
sensor OFF -> magnet ON delay

**Down Key - / Up Key +**  
magnet ON time  

(timing is saved to EEPROM after changes)

**Display**  
*Line 1*  
status or averaged sensor ON to OFF time in us

*Line 2*  
sensor OFF -> magnet ON delay in ms  
magnet ON time in ms  
change value in ms

**Calculation**  
sensor ON to OFF time is measured in TIMER4_CAPT and used as reciprocal relative speed (Vr)  
Vr is averaged and weighted, it influences the sensor OFF -> magnet ON delay
<br>

<br>
<br>

**Inspired by**  
https://www.youtube.com/watch?v=r_LG8FDt51U&t

**How to make a Steel Marble Track**  
https://www.youtube.com/watch?v=kPguktA674w

**Code Edited and Developed with**  
https://code.visualstudio.com/  
https://platformio.org/

<br>

---

<br>

## Views

<img src="images/02_viewup.jpg" width=640>
<br>
<br>

Stepup Electronic and Capacitor Charge Resistors  
<img src="images/03_electronic.jpg" width=640>
<br>
<br>

Low ESR Capacitors 4 x 3300uF 35V  
<img src="images/04_capa.jpg" width=640>
<br>
<br>

Inductive Proximity Sensor and Electro Magnet on Top of Magnetic Field Guide  
<img src="images/05_magnet.jpg" width=640>
<br>
<br>

Arduino Mega 2560 with LCD and Key Shield  
<img src="images/06_arduino.jpg" width=640>
<br>
<br>

<br>
<br>

---

[qrt@qland.de](mailto:qrt@qland.de) 240731
