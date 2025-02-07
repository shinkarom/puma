import argparse
from PIL import Image, ImageOps
import struct
import os

def convert_to_argb1555(pixel):
    """Convert a pixel (R, G, B, A) to ARGB1555 format and back to RGBA."""
    r, g, b, a = pixel[0] >> 3, pixel[1] >> 3, pixel[2] >> 3, pixel[3] >> 7
    a = 1 if pixel[3] == 255 else 0
    argb1555 = (a << 15) | (r << 10) | (g << 5) | b

    return argb1555
    
def convert_to_8bitpalette(pixel):
    r, g, b, a = pixel[0], pixel[1], pixel[2], pixel[3]
    
    if a < 255:
        # If the pixel is transparent, map it to 0.
        return 0
    
    # Convert each color channel to the appropriate bit depth
    r_3bit = (r >> 5) & 0b111  # Extract the top 3 bits of red
    g_3bit = (g >> 5) & 0b111  # Extract the top 3 bits of green
    b_2bit = (b >> 6) & 0b11   # Extract the top 2 bits of blue

    # Combine the bits into a single 8-bit RGB332 value
    rgb332 = (r_3bit << 5) | (g_3bit << 2) | b_2bit

    return rgb332

def save_as_binary(image, output_file):
    """Save the ARGB1555 pixel data to a binary file in big-endian format."""
    width, height = image.size
    pixel_data = []
    for y in range(height):
        for x in range(width):
            px8bit = convert_to_8bitpalette(image.getpixel((x, y)))
            pixel_data.append(px8bit)

    # Save to binary file in big endian
    with open(output_file, 'wb') as f:
        for value in pixel_data:
            f.write(struct.pack('B', value))

def main():
    parser = argparse.ArgumentParser(description="Resize image, convert to ARGB1555, and save binary file.")
    parser.add_argument("input", help="Input image file path")
    parser.add_argument("output", help="Output binary file path")

    args = parser.parse_args()

    # Load the image
    image = Image.open(args.input).convert("RGBA")

    # Save as binary file
    save_as_binary(image, args.output)
    print(f"Binary file saved as {args.output}")

if __name__ == "__main__":
    main()
