from PIL import Image
im = Image.open(input("file: "))
source = im.split()

R,G,B,A = 0,1,2,3

with open(input("output file: "), 'wb') as f:
    f.write( im.size[1].to_bytes(8,"little"))
    f.write( im.size[0].to_bytes(8,"little"))
    for height in range(im.size[0]):
        for width in range(im.size[1]):
            f.write((chr(source[R].getpixel((height, width)))).encode('charmap'))
            f.write((chr(source[G].getpixel((height, width)))).encode('charmap'))
            f.write((chr(source[B].getpixel((height, width)))).encode('charmap'))
            f.write((chr(source[A].getpixel((height, width)))).encode('charmap'))

print("width: {} height: {}".format(im.size[0],im.size[1]))