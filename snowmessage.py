import scrollphathd as sphd
from scrollphathd.fonts import font5x7
import datetime
import time
import signal
import random

str_len = 0
scroll_x = 0

while True:
    # Show snow for a period of time
    for a in range(0,5,1):
        x1 = random.randint(0,16)
        z1 = random.randint(-1,2)
        x2 = random.randint(0,16)
        z2 = random.randint(-1,2)
        x3 = random.randint(0,16)
        z3 = random.randint(-1,2)
        x4 = random.randint(0,16)
        z4 = random.randint(-1,2)
        sphd.clear()
        sphd.show()
        sphd.set_brightness(0.5)

        for y in range(0,30,1):
            for z in range(y,0,-1):
                sphd.set_pixel(x1, z+z1, (z/y)/2)
                sphd.set_pixel(x2, z+z2, (z/y)/2)
                sphd.set_pixel(x3, z+z3, (z/y)/2)
                sphd.set_pixel(x4, z+z4, (z/y)/2)
            sphd.show()
            time.sleep(0.05)

    # Show message for a period of time
    y = datetime.datetime.now()
    xmas = datetime.datetime(y.year, 12, 25) - datetime.datetime.now()
    daysleft = xmas.days
    hoursleft = xmas.seconds/3600
    sphd.set_brightness(0.5)

    sphd.clear()

    if ((daysleft > 1) or (daysleft == 0)) and ((hoursleft > 1) or (hoursleft == 0)): 
    	str_len = sphd.write_string("Ho ho ho! It's %i days and %i hours until xmas!    " %(daysleft, hoursleft), x=0, y=0, font=font5x7)
    elif ((daysleft > 1) or (daysleft == 0)) and (hoursleft == 1):
        str_len = sphd.write_string("Ho ho ho! It's %i days and %i hour until xmas!    " %(daysleft, hoursleft), x=0, y=0, font=font5x7)
    elif (daysleft == 1) and ((hoursleft > 1) or (hoursleft == 0)): 
        str_len = sphd.write_string("Ho ho ho! It's %i day and %i hours until xmas!    " %(daysleft, hoursleft), x=0, y=0, font=font5x7)
    elif (daysleft == 1) and (hoursleft == 1):
        str_len = sphd.write_string("Ho ho ho! It's %i day and %i hour until xmas!    " %(daysleft, hoursleft), x=0, y=0, font=font5x7)
    else:
        str_len = sphd.write_string("Ho ho ho! It's %i days and %i hours until xmas!    " %(daysleft, hoursleft), x=0, y=0, font=font5x7)

    while True:
        sphd.scroll_to(scroll_x, 0)
        sphd.show()
        time.sleep(0.02)
        scroll_x += 1
        if scroll_x >= str_len:
            scroll_x = 0
            break