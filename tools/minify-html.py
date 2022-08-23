#!/usr/bin/python

import os
import logging
from shutil import rmtree

logging.basicConfig(level=logging.INFO)

MINIFY_HTML_URL = r"https://www.toptal.com/developers/html-minifier/raw"
MINIFY_JS_URL = r"https://www.toptal.com/developers/javascript-minifier/api/raw"
MINIFY_CSS_URL = r"https://www.toptal.com/developers/cssminifier/raw"
URL_FILE_EXT = {"js": MINIFY_JS_URL, "css": MINIFY_CSS_URL, "html": MINIFY_HTML_URL}

INPUT_DIR = r"./src/html"
OUTPUT_DIR = r"./mini"

ENCODING = "utf-8"


def minify(input_file: str) -> str:
    filename, extension = os.path.splitext(input_file)
    url = URL_FILE_EXT[extension.strip(".")]

    import requests
    
    logging.info(f"minify::starting to minify file {input_file}")

    with open(input_file, "r") as file:
        data = {"input": file.read()}
        logging.debug(
            f"minify::Attempting to 'POST' @ {url} with \n{('*'*50)}  DATA  {('*'*50)}\n{data}\n{('*'*50)}  END  {('*'*50)}"
        )
        with requests.post(url=url, data=data) as response:
            logging.debug(
                f"minify::Response @ \n{('*'*50)}  DATA  {('*'*50)}\n{response.text}\n{('*'*50)}  END  {('*'*50)}"
            )

            return response.text


def minify_input_dir(input_dir: str, out_dir: str) -> None:
    for root, dirs, files in os.walk(input_dir):
        # Assume that nmp run build -> parcel build <src_location> --no-source-maps -> index.html + ect...
        for name in files:
            logging.info(
                f"minify_input_dir::Creating folder structure for files if needed"
            )
            file_dir = f"{root}".replace(input_dir, "")
            file_dir = f"{file_dir}".replace("\\", "/")
            # out_file = os.path.join(out_dir, file_dir, name)
            out_file = f"{out_dir}{file_dir}/{name}"
            os.makedirs(os.path.dirname(out_file), exist_ok=True)
            logging.debug(f"minify_input_dir::created folders if needed @ {out_file}")

            logging.info(
                f"minify_input_dir::Minifying {os.path.join(root, name)} to {out_file}"
            )
            mini = minify(os.path.join(root, name))

            logging.debug(
                f"minify_input_dir::Creating file @ {os.path.join(root, name)} -> {out_file}"
            )
            try:
                with open(out_file, "w") as file:
                    file.write(mini)
                logging.debug(f"minify_input_dir::Created file @ {out_file}")
            except FileNotFoundError as err:
                logging.critical(
                    f"minify_input_dir::Failed to create file @ {os.path.join(root, name)} to {out_file}"
                )
                raise err


if __name__ == "__main__":
    logging.info("__main__::Attempting to remove old minified files")
    rmtree(OUTPUT_DIR)
    logging.info("__main__::Starting minify program @ minify_input_dir")
    minify_input_dir(INPUT_DIR, OUTPUT_DIR)
