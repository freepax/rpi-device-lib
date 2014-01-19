rpi-device-lib
==============

Libraries supporting devices which can be hooked up to a raspberry pi Model B rev 2


* This project is developed on a ubuntu 13.10 Acer laptop.
* The tools in use is cloned from here: git://github.com/raspberrypi/tools.git
* To use a different compiler, edit the top level CMakeLists.txt file.

This project is at time of writing (2014-01-19) just established and as such a lot of things is unfinished, in particular
* l3gd20 is just sketched - I have not received my device yet, but will finish as soon as I does.
* ads1015 code need a propper way of calculating voltage (and possible more)
* mcp4725 need voltage calculation (and possible more)
