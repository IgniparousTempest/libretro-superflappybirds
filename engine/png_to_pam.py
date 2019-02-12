#!/usr/bin/env python3
from PIL import Image
import argparse
import pathlib


def image_to_pam(image_path, pam_path, verbose):
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

    with open(pam_path, 'w', newline='\n') as f:
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

        if (verbose):
            print("P7")
            print(f"WIDTH {width}")
            print(f"HEIGHT {height}")
            print(f"DEPTH {channels}")
            print("MAXVAL 255")
            if channels == 3:
                print("TUPLTYPE RGB")
            elif channels == 4:
                print("TUPLTYPE RGB_ALPHA")
            print("ENDHDR")

    with open(pam_path, 'ab') as f:
        f.write(bytearray(bytes))


def pam_to_png(pam_path, image_path, verbose):
    with open(pam_path, 'rb') as f:
        data = f.read().split(b'\n', 7)

        magic_number = data[0].decode('unicode_escape')
        if verbose: print(magic_number)
        assert magic_number == "P7"

        keyword, width = data[1].decode('unicode_escape').split(' ')
        width = int(width)
        if verbose: print(keyword, width)
        assert keyword == "WIDTH"

        keyword, height = data[2].decode('unicode_escape').split(' ')
        height = int(height)
        if verbose: print(keyword, height)
        assert keyword == "HEIGHT"

        keyword, channels = data[3].decode('unicode_escape').split(' ')
        channels = int(channels)
        if verbose: print(keyword, channels)
        assert keyword == "DEPTH"
        assert channels == 3 or channels == 4

        maxval = data[4].decode('unicode_escape')
        assert maxval == "MAXVAL 255"
        if verbose: print(maxval)

        tuple_type = data[5].decode('unicode_escape')
        if channels == 3:
            assert tuple_type == "TUPLTYPE RGB"
        elif channels == 4:
            assert tuple_type == "TUPLTYPE RGB_ALPHA"
        if verbose: print(tuple_type)

        end = data[6].decode('unicode_escape')
        assert end == "ENDHDR"
        if verbose: print(end)

        im_data = data[7]

        im = Image.new('RGBA', (width, height), (0, 0, 0, 0))
        pix = im.load()

        for y in range(height):
            for x in range(width):
                colour = [0] * channels
                for c in range(channels):
                    colour[c] = im_data[(y * width + x) * channels + c]
                pix[x, y] = tuple(colour)
    im.save(image_path)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Converts any image file to PAM file type')
    parser.add_argument('input', nargs=1, help='The file to read')
    parser.add_argument('-o', '--output', nargs=1, help='The file to write', required=False)
    parser.add_argument('-r', '--reverse', action='store_true', help='Converts a .pam to .png instead', required=False)
    parser.add_argument('-v', '--verbose', action='store_true', help='Displays process', required=False)
    args = vars(parser.parse_args())

    input_file = args["input"][0]

    if not args["reverse"]:
        output = input_file[:-len(pathlib.Path(input_file).suffix)] + ".pam"
        if args["output"] is not None:
            output = args["output"][0]

        image_to_pam(args["input"][0], output, args["verbose"])
    else:
        output = input_file[:-len(pathlib.Path(input_file).suffix)] + ".png"
        if args["output"] is not None:
            output = args["output"][0]

        pam_to_png(input_file, output, args["verbose"])
    # pix[x,y] = value  # Set the RGBA Value of the image (tuple)
    # im.save('alive_parrot.png')  # Save the modified pixels as .png
