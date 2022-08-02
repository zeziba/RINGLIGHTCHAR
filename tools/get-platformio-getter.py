#!/usr/bin/python

import requests
import os

PLATFORMIO_URL = r"https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py"

DEST_DIR = os.path.dirname(__file__)
CHUNK_SIZE = 8192

if __name__ =="__main__":
    with open(os.path.join(DEST_DIR, PLATFORMIO_URL.split("/")[-1]), "wb") as file:
        with requests.get(PLATFORMIO_URL, stream=True) as response:
            for chunk in response.iter_content(chunk_size=CHUNK_SIZE):
                file.write(chunk)