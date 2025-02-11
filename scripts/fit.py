import argparse
from PIL import Image, ImageOps
import struct
import os

def convert_to_8bitpalette_with_error(pixel, error):
    """Convert RGBA pixel to RGB332 format and apply error diffusion."""
    r, g, b, a = pixel
    if a < 255:
        # If the alpha channel is less than 255, treat as transparent
        return 0, (0, 0, 0)
    
    # Add the error to the current pixel (clamp result to 0-255)
    r = max(0, min(255, r + error[0]))
    g = max(0, min(255, g + error[1]))
    b = max(0, min(255, b + error[2]))

    # Convert each channel to palette ranges (RGB332)
    r_3bit = (r >> 5) & 0b111
    g_3bit = (g >> 5) & 0b111
    b_2bit = (b >> 6) & 0b11

    # Combine into RGB332
    rgb332 = (r_3bit << 5) | (g_3bit << 2) | b_2bit

    # Calculate the quantization error
    r_out = (r_3bit << 5)  # Back to full byte scale
    g_out = (g_3bit << 5)
    b_out = (b_2bit << 6)
    error = (r - r_out, g - g_out, b - b_out)

    return rgb332, error

def convert_to_8bitpalette(pixel):
    """Convert RGBA pixel to RGB332 format and apply error diffusion."""
    r, g, b, a = pixel
    if a < 255:
        # If the alpha channel is less than 255, treat as transparent
        return 0, (0, 0, 0)

    # Convert each channel to palette ranges (RGB332)
    r_3bit = (r >> 5) & 0b111
    g_3bit = (g >> 5) & 0b111
    b_2bit = (b >> 6) & 0b11

    # Combine into RGB332
    rgb332 = (r_3bit << 5) | (g_3bit << 2) | b_2bit

    return rgb332

def save_as_binary(image, output_file, apply_dither):
    """Apply dithering and save the pixel data to a binary file."""
    width, height = image.size
    pixel_data = [[0] * width for _ in range(height)]  # For storing processed pixels
    error_buffer = [[(0, 0, 0)] * width for _ in range(height)]  # Error tracking

    for y in range(height):
        for x in range(width):
            # Get pixel and cumulative error
            pixel = image.getpixel((x, y))
            error = error_buffer[y][x]

            if apply_dither:
                pixel8, quant_error = convert_to_8bitpalette_with_error(pixel, error)
            else:
                # Standard non-dithered conversion
                pixel8 = convert_to_8bitpalette(pixel)
                quant_error = (0, 0, 0)

            # Save the processed pixel
            pixel_data[y][x] = pixel8

            # Distribute quantization error to adjacent pixels
            if apply_dither:
                if x + 1 < width:
                    error_buffer[y][x + 1] = (
                        error_buffer[y][x + 1][0] + quant_error[0] * 7 // 16,
                        error_buffer[y][x + 1][1] + quant_error[1] * 7 // 16,
                        error_buffer[y][x + 1][2] + quant_error[2] * 7 // 16,
                    )
                if y + 1 < height:
                    if x > 0:
                        error_buffer[y + 1][x - 1] = (
                            error_buffer[y + 1][x - 1][0] + quant_error[0] * 3 // 16,
                            error_buffer[y + 1][x - 1][1] + quant_error[1] * 3 // 16,
                            error_buffer[y + 1][x - 1][2] + quant_error[2] * 3 // 16,
                        )
                    error_buffer[y + 1][x] = (
                        error_buffer[y + 1][x][0] + quant_error[0] * 5 // 16,
                        error_buffer[y + 1][x][1] + quant_error[1] * 5 // 16,
                        error_buffer[y + 1][x][2] + quant_error[2] * 5 // 16,
                    )
                    if x + 1 < width:
                        error_buffer[y + 1][x + 1] = (
                            error_buffer[y + 1][x + 1][0] + quant_error[0] * 1 // 16,
                            error_buffer[y + 1][x + 1][1] + quant_error[1] * 1 // 16,
                            error_buffer[y + 1][x + 1][2] + quant_error[2] * 1 // 16,
                        )

    # Save to binary file in big-endian
    with open(output_file, 'wb') as f:
        for y in range(height):
            for x in range(width):
                f.write(struct.pack('B', pixel_data[y][x]))

def main():
    parser = argparse.ArgumentParser(description="Resize image, convert to RGB332 with optional dithering, and save binary file.")
    parser.add_argument("input", help="Input image file path")
    parser.add_argument("output", help="Output binary file path")
    parser.add_argument("--dither", action="store_true", help="Enable Floyd-Steinberg dithering")

    args = parser.parse_args()

    # Load the image
    image = Image.open(args.input).convert("RGBA")

    # Save as binary file
    save_as_binary(image, args.output, args.dither)
    print(f"Binary file saved as {args.output}")

if __name__ == "__main__":
    main()