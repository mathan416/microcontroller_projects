import time

import random
from random import seed
from random import randint

import picographics
from picographics import PicoGraphics, DISPLAY_PICO_DISPLAY, PEN_RGB332
from pimoroni import Button

import jpegdec
import sys

display = picographics.PicoGraphics(display=picographics.DISPLAY_PICO_DISPLAY, pen_type=PEN_RGB332)
display.clear()
display.update()

image_num = 1
sw, sh = display.get_bounds()
button_a = Button(12)
button_b = Button(13)
button_x = Button(14)
button_y = Button(15)

# Create a new JPEG decoder for our PicoGraphics
j = jpegdec.JPEG(display)

# Classes
class Ball:
    def __init__(self, x, y, r, dx, dy, pen):
        self.x = x
        self.y = y
        self.r = r
        self.dx = dx
        self.dy = dy
        self.pen = pen

# Functions
def show_image(idImage):
    # Open the JPEG file
    filename = "image_" + str(idImage) + ".jpeg"
    image = j.open_file(filename)

    # Decode the JPEG
    j.decode(0, 0, jpegdec.JPEG_SCALE_FULL)
    jw = j.get_width()
    jh = j.get_height()
    pw = int((sw - jw)/2)
    ph = int((sh - jh)/2)
    display.clear()
    j.decode(pw, ph, jpegdec.JPEG_SCALE_FULL)  

    # Display the result
    display.update()

def show_balls():
    display.set_pen(BG)
    for ball in balls:
        ball.x += ball.dx
        ball.y += ball.dy

        xmax = sw - ball.r
        xmin = ball.r
        ymax = sh - ball.r
        ymin = ball.r

        if ball.x < xmin or ball.x > xmax:
            ball.dx *= -1

        if ball.y < ymin or ball.y > ymax:
            ball.dy *= -1
            
        display.set_pen(ball.pen)
        display.circle(int(ball.x), int(ball.y), int(ball.r))
        display.update()

def show_random_balls():
    display.set_pen(BG)
    for ball in balls:
        ball.x = randint(0, sw)
        ball.y = randint(0, sh)
            
        display.set_pen(ball.pen)
        display.circle(int(ball.x), int(ball.y), int(ball.r))
        display.update()    

# initialise shapes
seed(999)

balls = []
for i in range(0, 5):
    r = random.randint(0, 10) + 3
    balls.append(
        Ball(
            random.randint(r, r + (sw - 2 * r)),
            random.randint(r, r + (sh - 2 * r)),
            5,
            (14 - 4) / 2,
            (14 - 4) / 2,
            display.create_pen(random.randint(200, 255), random.randint(200, 255), random.randint(200, 255)),
        )
    )

BG = display.create_pen(40, 40, 40)
show_image(image_num)
ballcount = 0

while True:
    ballcount += 1
    if button_a.read():
        print("A pressed")
        image_num += 1
        if image_num > 9: 
            image_num = 9
        show_image(image_num)
        
    elif button_b.read():
        print("B pressed")
        image_num -= 1
        if image_num < 1: 
            image_num = 1
        show_image(image_num)

    elif button_y.read():
        print("Y pressed - exiting")
        display.clear()
        display.update()
        sys.exit("B pressed - exiting")
        
    elif button_x.read():
        print("X pressed")
        while True:
            image_num = randint(1, 9)
            show_image(image_num)
            
            for _ in range(0, 20):
                show_random_balls()
                time.sleep(0.1)
            
            if button_y.read():
                print("Y pressed")
                image_num = 1
                show_image(image_num)
                break
            
            if button_b.read():
                print("B pressed - exiting")
                display.clear()
                display.update()
                sys.exit("B pressed - exiting")
                
    show_random_balls()
    
    if ballcount == 50:
        image_num = randint(1, 9)
        show_image(image_num)
        ballcount = 0;
        
    time.sleep(0.1)

