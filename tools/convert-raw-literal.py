#!/usr/bin/python

from io import BufferedReader, FileIO, TextIOWrapper
import os
import logging

logging.basicConfig(level=logging.INFO)

INPUT_DIR = r"./dist"
OUTPUT_DIR = r"./src"

ENCODING = "utf-8"


PREGEN_MESSAGE = """
/*  
 *  This file is pregenerated by a script
 *  DO NOT CHANGE
 */
"""

COMPILER_MACRO_START = """
#ifndef _PROGRAM_HTML_
#define _PROGRAM_HTML_

#include <Arduino.h>

"""

COMPILER_MACRO_END = """
#endif
"""

def _write_progmem_to_file(
    file: TextIOWrapper | BufferedReader | FileIO, data: str, dir=""
) -> None:
    logging.debug(f"_write_progmem_to_file::Creating filename, extension variables")
    filename, extension = os.path.splitext(file.name)
    file_extension = f"{extension}".replace(".", "")
    filename = str(filename).replace("\\", "/")
    filename = "".join(x.replace("\\", "/") for x in filename).replace("/", "_").replace(".", "_")

    logging.debug(f"_write_progmem_to_file::Creating file_dir variable")
    file_dir = f"{dir}".replace(INPUT_DIR, "")
    file_dir = f"{file_dir}".replace("\\", "/")
    
    dir = "".join(x.replace("\\", "/") for x in dir)

    logging.info(f"_write_progmem_to_file::file.write data to file")
    file.write(f"{PREGEN_MESSAGE}\n\n")
    file.write(f"{COMPILER_MACRO_START}")
    
    file.write(f"// {os.path.normpath(dir)}\n")
    file.write(
        f"const char data_{filename}_{file_extension}[] PROGMEM = R\"rawliteral({data})rawliteral\";\n\n"
    )
    file.write(f"{COMPILER_MACRO_END}")


def write_progmem_to_file(file: str, data: str, dir="") -> None:
    logging.debug(f"write_progmem_to_file::os.makedirs to create directories for file")
    os.makedirs(os.path.dirname(file), exist_ok=True)

    with open(file, "w") as file:
        return _write_progmem_to_file(file=file, data=data, dir=dir)


def convert_dir(in_dir: str, out_dir: str) -> None:
    for root, dirs, files in os.walk(in_dir):
        for name in files:
            logging.info(f"convert_dir::attempting to convert {name}")
            if "hex_" in name:
                logging.info(
                    f"convert_dir::failed to convert {name} as it is already converted"
                )
                continue
            filename_new = f"hex_{name.split('.')[0]}.h"
            logging.info(f"convert_dir::converting {os.path.join(root,name)}")
            with open(os.path.join(root, name), "r") as file:

                nw_file_location = os.path.join(out_dir, filename_new)
                logging.info(
                    f"convert_dir::writing converted file {os.path.join(root, name)} -> {nw_file_location}"
                )
                write_progmem_to_file(nw_file_location, file.read(), os.path.join(root, name))


if __name__ == "__main__":
    logging.info("__main__::Starting convert program @ convert_dir")
    convert_dir(INPUT_DIR, OUTPUT_DIR)