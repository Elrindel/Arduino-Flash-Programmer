EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L 74xx:74HC595 U2
U 1 1 5EE90BF1
P 3300 4500
F 0 "U2" H 3550 5200 50  0000 C CNN
F 1 "74HC595" H 3550 5100 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm_Socket_LongPads" H 3300 4500 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74hc595.pdf" H 3300 4500 50  0001 C CNN
	1    3300 4500
	1    0    0    -1  
$EndComp
$Comp
L Memory_Flash:SST39SF040 U3
U 1 1 5EE91E38
P 5150 3500
F 0 "U3" H 5500 5000 50  0000 C CNN
F 1 "SST39SF040" H 5500 4900 50  0000 C CNN
F 2 "Package_DIP:DIP-32_W15.24mm_Socket_LongPads" H 5150 3800 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/25022B.pdf" H 5150 3800 50  0001 C CNN
	1    5150 3500
	1    0    0    -1  
$EndComp
Text GLabel 2800 2600 0    50   Input ~ 0
CLK
Text GLabel 2800 4300 0    50   Input ~ 0
CLK
Text GLabel 2800 4600 0    50   Input ~ 0
RCLK
Text GLabel 2800 4700 0    50   Input ~ 0
GND
Text GLabel 2800 2900 0    50   Input ~ 0
RCLK
Text GLabel 2800 3000 0    50   Input ~ 0
GND
Wire Wire Line
	2800 2600 2900 2600
Wire Wire Line
	2800 2900 2900 2900
Wire Wire Line
	2900 3000 2800 3000
Text GLabel 2800 2700 0    50   Input ~ 0
VCC
Text GLabel 2800 4400 0    50   Input ~ 0
VCC
Text GLabel 3250 3800 0    50   Input ~ 0
VCC
Text GLabel 3250 2100 0    50   Input ~ 0
VCC
Wire Wire Line
	2800 2700 2900 2700
Wire Wire Line
	3250 2100 3300 2100
Wire Wire Line
	3300 2100 3300 2200
Wire Wire Line
	3250 3800 3300 3800
Wire Wire Line
	3300 3800 3300 3900
Wire Wire Line
	2800 4300 2900 4300
Wire Wire Line
	2900 4400 2800 4400
Wire Wire Line
	2800 4600 2900 4600
Wire Wire Line
	2900 4700 2800 4700
Text GLabel 3250 5300 0    50   Input ~ 0
GND
Text GLabel 3250 3600 0    50   Input ~ 0
GND
Wire Wire Line
	3250 5300 3300 5300
Wire Wire Line
	3300 5300 3300 5200
Wire Wire Line
	3250 3600 3300 3600
Wire Wire Line
	3300 3600 3300 3500
Wire Wire Line
	2900 4100 2800 4100
Wire Wire Line
	3800 3700 3800 3300
Wire Wire Line
	3800 3300 3700 3300
Text GLabel 4450 2300 0    50   Input ~ 0
A0
Text GLabel 4450 2400 0    50   Input ~ 0
A1
Text GLabel 4450 2500 0    50   Input ~ 0
A2
Text GLabel 4450 2600 0    50   Input ~ 0
A3
Text GLabel 4450 2700 0    50   Input ~ 0
A4
Text GLabel 4450 2800 0    50   Input ~ 0
A5
Text GLabel 4450 2900 0    50   Input ~ 0
A6
Text GLabel 4450 3000 0    50   Input ~ 0
A7
Text GLabel 4450 3100 0    50   Input ~ 0
A8
Text GLabel 4450 3200 0    50   Input ~ 0
A9
Text GLabel 4450 3300 0    50   Input ~ 0
A10
Text GLabel 4450 3400 0    50   Input ~ 0
A11
Text GLabel 4450 3500 0    50   Input ~ 0
A12
Text GLabel 4450 3600 0    50   Input ~ 0
A13
Text GLabel 4450 3700 0    50   Input ~ 0
A14
Text GLabel 4450 3800 0    50   Input ~ 0
A15
Text GLabel 4450 3900 0    50   Input ~ 0
A16
Text GLabel 4450 4000 0    50   Input ~ 0
A17
Text GLabel 4450 4100 0    50   Input ~ 0
A18
Wire Wire Line
	4450 2300 4550 2300
Wire Wire Line
	4550 2400 4450 2400
Wire Wire Line
	4450 2500 4550 2500
Wire Wire Line
	4550 2600 4450 2600
Wire Wire Line
	4450 2700 4550 2700
Wire Wire Line
	4550 2800 4450 2800
Wire Wire Line
	4450 2900 4550 2900
Wire Wire Line
	4550 3000 4450 3000
Wire Wire Line
	4450 3100 4550 3100
Wire Wire Line
	4550 3200 4450 3200
Wire Wire Line
	4450 3300 4550 3300
Wire Wire Line
	4550 3400 4450 3400
Wire Wire Line
	4450 3500 4550 3500
Wire Wire Line
	4550 3600 4450 3600
Wire Wire Line
	4450 3700 4550 3700
Wire Wire Line
	4550 3800 4450 3800
Wire Wire Line
	4450 3900 4550 3900
Wire Wire Line
	4550 4000 4450 4000
Wire Wire Line
	4450 4100 4550 4100
Text GLabel 3800 2400 2    50   Input ~ 0
A0
Text GLabel 3800 2500 2    50   Input ~ 0
A1
Text GLabel 3800 2600 2    50   Input ~ 0
A2
Text GLabel 3800 2700 2    50   Input ~ 0
A3
Text GLabel 3800 2800 2    50   Input ~ 0
A4
Text GLabel 3800 2900 2    50   Input ~ 0
A5
Text GLabel 3800 3000 2    50   Input ~ 0
A6
Text GLabel 3800 3100 2    50   Input ~ 0
A7
Wire Wire Line
	3800 2400 3700 2400
Wire Wire Line
	3700 2500 3800 2500
Wire Wire Line
	3800 2600 3700 2600
Wire Wire Line
	3700 2700 3800 2700
Wire Wire Line
	3800 2800 3700 2800
Wire Wire Line
	3700 2900 3800 2900
Wire Wire Line
	3800 3000 3700 3000
Wire Wire Line
	3700 3100 3800 3100
Text GLabel 3800 4100 2    50   Input ~ 0
A8
Text GLabel 3800 4200 2    50   Input ~ 0
A9
Text GLabel 3800 4300 2    50   Input ~ 0
A10
Text GLabel 3800 4400 2    50   Input ~ 0
A11
Text GLabel 3800 4500 2    50   Input ~ 0
A12
Text GLabel 3800 4600 2    50   Input ~ 0
A13
Text GLabel 3800 4700 2    50   Input ~ 0
A14
Text GLabel 3800 4800 2    50   Input ~ 0
A15
Wire Wire Line
	3800 4100 3700 4100
Wire Wire Line
	3700 4200 3800 4200
Wire Wire Line
	3800 4300 3700 4300
Wire Wire Line
	3700 4400 3800 4400
Wire Wire Line
	3800 4500 3700 4500
Wire Wire Line
	3700 4600 3800 4600
Wire Wire Line
	3800 4700 3700 4700
Wire Wire Line
	3700 4800 3800 4800
Text GLabel 5100 4800 0    50   Input ~ 0
GND
Wire Wire Line
	5100 4800 5150 4800
Wire Wire Line
	5150 4800 5150 4700
Text GLabel 5100 2100 0    50   Input ~ 0
VCC
Wire Wire Line
	5100 2100 5150 2100
Wire Wire Line
	5150 2100 5150 2200
$Comp
L 74xx:74HC595 U1
U 1 1 5EE8EA49
P 3300 2800
F 0 "U1" H 3550 3500 50  0000 C CNN
F 1 "74HC595" H 3550 3400 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm_Socket_LongPads" H 3300 2800 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74hc595.pdf" H 3300 2800 50  0001 C CNN
	1    3300 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 2400 2900 2400
Text GLabel 2800 2400 0    50   Input ~ 0
SER
Text GLabel 5850 2300 2    50   Input ~ 0
D0
Text GLabel 5850 2400 2    50   Input ~ 0
D1
Text GLabel 5850 2500 2    50   Input ~ 0
D2
Text GLabel 5850 2600 2    50   Input ~ 0
D3
Text GLabel 5850 2700 2    50   Input ~ 0
D4
Text GLabel 5850 2800 2    50   Input ~ 0
D5
Text GLabel 5850 2900 2    50   Input ~ 0
D6
Text GLabel 5850 3000 2    50   Input ~ 0
D7
Wire Wire Line
	5750 2300 5850 2300
Wire Wire Line
	5750 2400 5850 2400
Wire Wire Line
	5750 2500 5850 2500
Wire Wire Line
	5750 2600 5850 2600
Wire Wire Line
	5750 2700 5850 2700
Wire Wire Line
	5750 2800 5850 2800
Wire Wire Line
	5750 2900 5850 2900
Wire Wire Line
	5750 3000 5850 3000
Text GLabel 4450 4300 0    50   Input ~ 0
WE
Text GLabel 4450 4600 0    50   Input ~ 0
OE
Text GLabel 4450 4500 0    50   Input ~ 0
CE
Wire Wire Line
	4550 4300 4450 4300
Wire Wire Line
	4550 4600 4450 4600
Wire Wire Line
	4550 4500 4450 4500
Text GLabel 3800 5000 2    50   Input ~ 0
QH'
Wire Wire Line
	3700 5000 3800 5000
$Comp
L MCU_Module:Arduino_UNO_R2 A1
U 1 1 5EEF29A6
P 6850 3650
F 0 "A1" H 7450 4750 50  0000 C CNN
F 1 "Arduino_UNO_R2" H 7450 4650 50  0000 C CNN
F 2 "Module:Arduino_UNO_R2" H 6850 3650 50  0001 C CIN
F 3 "https://www.arduino.cc/en/Main/arduinoBoardUno" H 6850 3650 50  0001 C CNN
	1    6850 3650
	1    0    0    -1  
$EndComp
Text GLabel 6250 3250 0    50   Input ~ 0
D0
Text GLabel 6250 3350 0    50   Input ~ 0
D1
Text GLabel 6250 3450 0    50   Input ~ 0
D2
Text GLabel 6250 3550 0    50   Input ~ 0
D3
Text GLabel 6250 3650 0    50   Input ~ 0
D4
Text GLabel 6250 3750 0    50   Input ~ 0
D5
Text GLabel 6250 3850 0    50   Input ~ 0
D6
Text GLabel 6250 3950 0    50   Input ~ 0
D7
Wire Wire Line
	6350 3250 6250 3250
Wire Wire Line
	6350 3350 6250 3350
Wire Wire Line
	6350 3450 6250 3450
Wire Wire Line
	6350 3550 6250 3550
Wire Wire Line
	6350 3650 6250 3650
Wire Wire Line
	6350 3750 6250 3750
Wire Wire Line
	6350 3850 6250 3850
Wire Wire Line
	6350 3950 6250 3950
Text GLabel 7050 2550 1    50   Input ~ 0
VCC
Wire Wire Line
	7050 2650 7050 2550
Text GLabel 6250 4250 0    50   Input ~ 0
WE
Text GLabel 6250 4150 0    50   Input ~ 0
OE
Text GLabel 6250 4050 0    50   Input ~ 0
CE
Wire Wire Line
	6350 4250 6250 4250
Wire Wire Line
	6350 4150 6250 4150
Wire Wire Line
	6350 4050 6250 4050
Text GLabel 7450 3650 2    50   Input ~ 0
A16
Text GLabel 7450 3750 2    50   Input ~ 0
A17
Text GLabel 7450 3850 2    50   Input ~ 0
A18
Wire Wire Line
	7450 3650 7350 3650
Wire Wire Line
	7350 3750 7450 3750
Wire Wire Line
	7450 3850 7350 3850
Text GLabel 7450 3950 2    50   Input ~ 0
SER
Text GLabel 7450 4050 2    50   Input ~ 0
CLK
Text GLabel 7450 4150 2    50   Input ~ 0
RCLK
Wire Wire Line
	7350 3950 7450 3950
Wire Wire Line
	7350 4050 7450 4050
Wire Wire Line
	7350 4150 7450 4150
Text GLabel 6250 4350 0    50   Input ~ 0
QH'
Wire Wire Line
	6350 4350 6250 4350
Text GLabel 6850 4850 3    50   Input ~ 0
GND
Wire Wire Line
	6850 4750 6850 4850
Wire Wire Line
	2800 4100 2800 3700
Wire Wire Line
	2800 3700 3800 3700
$EndSCHEMATC
