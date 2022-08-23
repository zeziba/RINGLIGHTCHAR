## Install Instructions

Do the following

- Download and install vscode or other IDE
- Download and install npm
- Download and install python 3.8+

Once the above are installed then it is possible to finish the installation process by doing the following

1. Prepare the virutal environment for python and platformio using a terminal
    > Run ``python -m pip install --upgrade pip``

    > Run ``python -m venv ./.venv`` at this point in vscode you can set this virtual enviroment as the default

    > Run ``.\.venv\Scripts\activate`` to activate the virtual enviroment

    > Run ``python -m pip install -r requirements.txt``
2. Download the platformio script to install it into the virtual environment
  - Go to ``https://docs.platformio.org/en/latest//core/installation/methods/installer-script.html`` and select the method you need
  - The following assumes you are following the windows methods, I have included a getter script to get the windows specific script from platformio
    > Run ``python ./tools/get-platformio-getter.py``
3. Activate PlatformIO in vscode through the extensions tab, this will finialize the PlatformIO install and utilize a virtual enviroment for both python and it
4. Install dependencies for npm
    > Run ``npm install``
5. Create ``./src/env.h`` file following this format

```
#ifndef _ENV_H_
#define _ENV_H_

#include <Arduino.h>

#define SERIAL_ENABLE 0 // 0 is false 1 is true
#define BAUD_RATE 115200 // Baud rate for serial
#define SSID_M "<Your Specific SSID>"
#define PSWD_M "<Your Specific PASS>"

#endif
```

6. Change partitions_singleapp.csv as needed and the file name in platformio.ini as needed for your specific needs

At this point the project is set up and ready to be used for development, once the project is ready for deployment follow these instructions before building

#### Note

Might need to run ``npm build`` to compile html

1. Minify the src/html to mini/
    > Run ``python ./tools/minify-html.py``
2. Bundle the mini/* files into dist/index.html
    > Run ``python ./tools/convert-raw-literal.py``
3. Update your code as needed with the name of the variable inside of hex_index.h file to use the index page form your src/html

## Notes:
- All server functions are defined in sserver.h/.cpp
- The file neo_pixel_effects.h is used to change the neopixel
- pins.h is used to define the pins for the **Wemos D1 mini lite**
- The src/html/* files are developed as a standard web page will. The web page is stored on the stack {not heap} for memory issues
  - This leads to a rather large size max for the web page of roughly ~500kbs unless there is some optimizations done
- This project uses black for python formatting
- This project used vscode .editorconfig for c/c++ formatting
- PlatformIO can load any files in the ./data/* folder into the memory of microcontroller using the Build/Uplaod Filesystem image tools
  - These files can then be utilized with the ``<LittleFS.h>`` which can be downloaded with PlatformIO

