import png2bin
from PIL import Image

if __name__ == '__main__':
    #im2 = Image.open("HJoceanSmall.png") # read a png image using PIL
    #im2.show() # display the image
    #png2bin.write_image(im2, "HJoceanSmall.bin") # write the image to a bin file

    
    im = png2bin.read_image("AminNicer.bin") # read a bin image using png2bin
    im.show() # display the image

    im = png2bin.read_image("img0.bin") # read a bin image using png2bin
    im.show() # display the image


    im = png2bin.read_image("img1.bin") # read a bin image using png2bin
    im.show() # display the image


    im = png2bin.read_image("img2.bin") # read a bin image using png2bin
    im.show() # display the image


    im = png2bin.read_image("img3.bin") # read a bin image using png2bin
    im.show() # display the image


    im = png2bin.read_image("img4.bin") # read a bin image using png2bin
    im.show() # display the image

    
