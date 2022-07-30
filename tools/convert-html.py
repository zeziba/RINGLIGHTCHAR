#!/usr/bin/python

from io import BufferedReader, FileIO, TextIOWrapper
import os
import logging
from re import I

logging.basicConfig(level=logging.DEBUG)

INPUT_DIR = r"./dist"
OUTPUT_DIR = r"./data"

ENCODING = "utf-8"


def ascii_to_hex(text: str) -> str:
    output = []

    logging.info(f"ascii_to_hex::converting {text[:10]}... to hex")
    for char in text:
        output.append(hex(ord(char)))

    return ",".join(output)


def _write_progmem_to_file(
    file: TextIOWrapper | BufferedReader | FileIO, data: str, dir=""
) -> None:
    logging.debug(f"_write_progmem_to_file::Creating filename, extension variables")
    filename, extension = os.path.splitext(file.name)
    file_extension = f"{extension}".replace(".", "")

    logging.debug(f"_write_progmem_to_file::Creating file_dir variable")
    file_dir = f"{dir}".replace(INPUT_DIR, "")
    file_dir = f"{file_dir}".replace("\\", "/")

    logging.info(f"_write_progmem_to_file::file.write data to file")
    file.write(f"// /{file_dir}/{file.name}")
    file.write(
        f"const char *data_{filename}_{file_extension}_path PROGMEM = /{dir}/;\n"
    )
    file.write(
        f"const char data_{filename}_{extension}[] PROGMEM = {data.upper()};\n\n"
    )


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
            filename_new = f"hex_{name}"
            logging.info(f"convert_dir::converting {os.path.join(root,name)}")
            with open(os.path.join(root, name), "r") as file:
                hexed = ascii_to_hex(file.read())

                nw_file_location = os.path.join(out_dir, filename_new)
                logging.info(
                    f"convert_dir::writing converted file {os.path.join(root, name)} -> {nw_file_location}"
                )
                write_progmem_to_file(nw_file_location, hexed, os.path.join(root, name))


if __name__ == "__main__":
    logging.info("__main__::Starting convert program @ convert_dir")
    convert_dir(INPUT_DIR, OUTPUT_DIR)
