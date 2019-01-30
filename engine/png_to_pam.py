#!/usr/bin/env python3
from PIL import Image
import argparse
import pathlib


def image_to_pam(image_path, pam_path):
    im = Image.open(image_path)  # Can be many different formats.
    pix = im.load()
    width, height = im.size
    channels = len(im.mode)
    assert channels == 3 or channels == 4

    bytes = []
    for y in range(height):
        for x in range(width):
            for c in range(channels):
                bytes.append(pix[x, y][c])

    with open(pam_path, 'w') as f:
        f.write("P7\n")
        f.write(f"WIDTH {width}\n")
        f.write(f"HEIGHT {height}\n")
        f.write(f"DEPTH {channels}\n")
        f.write("MAXVAL 255\n")
        if channels == 3:
            f.write("TUPLTYPE RGB\n")
        elif channels == 4:
            f.write("TUPLTYPE RGB_ALPHA\n")
        f.write("ENDHDR\n")

    with open(pam_path, 'ab') as f:
        f.write(bytearray(bytes))


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Converts any image file to PAM file type')
    parser.add_argument('input', nargs=1, help='The file to read')
    parser.add_argument('-o', '--output', nargs=1, help='The file to write', required=False)
    parser.add_argument('-r', '--reverse', action='store_true', help='Converts a .pam to .png instead', required=False)
    args = vars(parser.parse_args())

    if not args["reverse"]:
        input_file = args["input"][0]
        output = input_file[:-len(pathlib.Path(input_file).suffix)] + ".pam"
        if args["output"] is not None:
            output = args["output"][0]

        image_to_pam(args["input"][0], output)
    # pix[x,y] = value  # Set the RGBA Value of the image (tuple)
    # im.save('alive_parrot.png')  # Save the modified pixels as .png
