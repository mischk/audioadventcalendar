### Audio Adventskalendar V2

erweiterte Version des Audio Adventskalenders, den ich 2011 gebastelt habe. Die Bauanleitung findet man auf [Expli](http://www.expli.de/anleitung/audio-adventskalender-4698/). Diese neue Version benutzt statt des Tageszählers im permanenten Speicher vom Arduino eine Echtzeituhr. Das Modul ist ein RTC DS1307 ähnlich dem Modul von Adafruit (https://learn.adafruit.com/ds1307-real-time-clock-breakout-board-kit). Diese gibt es auch billig bei E-Bay und sie sind grundsätzlich miteinander kompatibel, da das Modul nur die Pins des Chips nach außen führt, um sie besser verlöten zu können. Alternativ kann man auch einen DS1307 selbst verkabeln.

Die Verkabelung ist folgendermaßen (zusätzlich zu der, wie sie in der Anleitung beschrieben wird):

    DS1307    Arduino

    SCL       A5
    SDA       A4
    VCC       A3
    GND       A2


![Audio Advent Calendar](http://www.expli.de/uploads/images/132250719039_29009.jpg)

Viel Spaß damit!