from PIL import Image

im = Image.open(input("file: "))
source = im.split()

with open(input("output file: "), 'wb') as f:
    f.write( im.size[1].to_bytes(8,"little"))
    f.write( im.size[0].to_bytes(8,"little"))
    for height in range(im.size[0]):
        for width in range(im.size[1]):
            f.write((chr(source[0].getpixel((height, width)))).encode('charmap'))
            print(source[0].getpixel((height, width)))

print("width: {} height: {}".format(im.size[0],im.size[1]))