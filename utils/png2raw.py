from PIL import Image
source = Image.open(input("file: ")).split()

with open(input("output file: "), 'wb') as f:
    f.write( im.size[1].to_bytes(8,"little"))
    f.write( im.size[0].to_bytes(8,"little"))
    for height in range(im.size[0]):
        for width in range(im.size[1]):
            for i in range(0,3):
                f.write((chr(source[i].getpixel((height, width)))).encode('charmap'))

print("w: {} h: {}".format(im.size[0],im.size[1]))