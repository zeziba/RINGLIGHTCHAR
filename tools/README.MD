# The tools contained in this folder are to facilitate the conversion of the html src/html to a single file which will be pushed out by the ESP8266

### convert-html.py

This file will convert the html file into a c++.h file which will contain the byte code in PROGMEM ustilizing the Arduino.h

This file can then be complied and used in the program

### convert-raw-literal.py

This file will place the files in the dist/ folder into a c++.h file which will be wrapped as a PROGMEM <VAR\> = R"rawliteral(...)"
This variable can be then used as is

### minify-html.py

This file with take all files in the src/html/* and run them through the minifier online from toptal.com
THe output files will be stored in mini/*