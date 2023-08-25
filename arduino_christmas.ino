#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
  
// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

#define PIN 6
 
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(6, 8, PIN,
  NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);


// Global Variables
// matrix_x, y used to drive the size of the LED matrix
// text_pass used to change text colours after each pass
// starpos used to position the star when switching to different colours
// starpos_incdec is used to move the star back and forth based on the boundaries of the matrix
// rand_messages is used to randmize the messge that is shown
// text_colours is an array of colours to use for the text, star_rgb used for star color values, star_colour used to drive the actual colour of the star
int matrix_x = matrix.width();
int matrix_y = matrix.height();
int text_pass = 0;
int starpos = 0;
boolean starpos_incdec = 1;
int rand_messages = 0;
unsigned int star_rgb[3] = {255,255,255};
uint16_t star_colour = matrix.Color(star_rgb[0], star_rgb[1], star_rgb[2]);
const uint16_t text_colours[] = {matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(255, 255, 0),matrix.Color(0, 0, 255), matrix.Color(255, 0, 255), matrix.Color(0, 255, 255), matrix.Color(255, 255, 255)};

// Merry Christmas Message
 String myMessages[]={
    "Merry Christmas", 
    "You've been naughty", 
    "Krampus is here", 
    "Ho Ho Ho!", 
    "Happy Holigays",
    "Meet me under the mistletoe", 
    "Happy for the Holidays",
    "Santa arrives tonight"};
 int myMessagesSizes[]={-122,-120,-98,-80,-95,-170,-140,-120};
 

// Code
void setup() 
{
  Serial.begin(9600);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(6);
  matrix.setTextColor(text_colours[0]);
}

void update_star()
{
  // Future code fix - Make dynamic based on matrix_x and matrix_y
  // In short, if the star hits the Y boundaries, move back

  int random_pixel = 0;
  uint32_t colour = 0;

  if (starpos+4 == 7) { starpos_incdec = 0; } 
  if (starpos == 0) { starpos_incdec = 1; }    

  // Clear screen, draw star
  matrix.fillScreen(0);
  star_colour = matrix.Color(star_rgb[1], star_rgb[0], star_rgb[2]);
  matrix.drawLine(0, starpos, 4, starpos+4, star_colour);
  matrix.drawLine(0, starpos+4, 4, starpos, star_colour);
  matrix.drawLine(0, starpos+2, 4, starpos+2, star_colour);
  matrix.drawLine(2, starpos, 2, starpos+4, star_colour); 

  if (starpos % 2)
  {
    random_pixel = random(0,41);
    colour = matrix.getPixelColor(random_pixel);        
    matrix.setPixelColor(random_pixel, 125, 125, 125);
    matrix.show();
    delay(1);
    matrix.setPixelColor(random_pixel, colour);       
  }
  matrix.show();      
  delay(1);
}

void color_morph(unsigned int* value, int get_brighter)
{
  // Updates the colour through updating the refence.  
  // Get brighter flag increments/decrements
  for (int i = 0; i < 255; i++)
  {
    if (get_brighter)
      (*value)--;
    else
      (*value)++;

    update_star();
  }
  if (starpos_incdec) { starpos++; } else { starpos--; } 
}

void rowColorWipe(uint32_t c1, uint32_t c2, uint32_t c3, uint16_t wait) 
{
  // Borrowed code to draw green, white and red bars
  // Sparkles were added by me
  int random_pixel = 0;
  uint32_t colour = 0;
  
  // Clear screen
  matrix.fillScreen(0);

  // Draw the bars
  for(int j=0; j<2; j++) {
    for(int q=0; q<12; q++){
      for(int y=0; y <= matrix.height() + 20; y=y+6) {
        matrix.drawLine(0, y+q-11, 5, y+q-6, c3);
        matrix.drawLine(0, y+q-10, 5, y+q-5, c1);
        matrix.drawLine(0, y+q-9, 5, y+q-4, c1);
        matrix.drawLine(0, y+q-8, 5, y+q-3, c3);
        matrix.drawLine(0, y+q-7, 5, y+q-2, c2);
        matrix.drawLine(0, y+q-6, 5, y+q-1, c2); 
      }

      for(int x=0; x <= 10; x++)
      {
        random_pixel = random(0,41);
        colour = matrix.getPixelColor(random_pixel);        
        matrix.setPixelColor(random_pixel, 255, 255, 255);
        matrix.show();
        delay(10);
        matrix.setPixelColor(random_pixel, colour);
        matrix.show();
      } 
      delay(wait);
    }
  }
}

void loop()
{
  // The main Arduino loop
  // Displays the christmas message, then the Christmas Wrapping bars, and then the star animation
  // Easier to have the message scroll here rather than using a separate method/function
  
  // Clears screen and sets message
  matrix.fillScreen(0);
  matrix.setCursor(matrix_x, 0);  
  matrix.print(myMessages[rand_messages]);
  // matrix.print(F("Merry Christmas")); // If using a single message

  // Enter this only if the message has been fully displayed. It's displayed by moving the cursor
  if(--matrix_x <= myMessagesSizes[rand_messages]) {
    // Reset message scroll
    matrix_x = matrix.width();
    matrix.setCursor(matrix_x, 0);

    // Christmas Wrapping bars
    rowColorWipe(matrix.Color(240, 0, 0), matrix.Color(240, 240, 240), matrix.Color(0, 240, 0), 100);

    // Cycle through the star colours
    color_morph(&star_rgb[0], 1); // transition to red
    color_morph(&star_rgb[1], 1); // transition to yellow
    color_morph(&star_rgb[0], 0); // transition to green 
    color_morph(&star_rgb[2], 1); // transition to aqua 
    color_morph(&star_rgb[0], 1); // transition to white
    color_morph(&star_rgb[1], 0); // transition to violet
    color_morph(&star_rgb[0], 0); // transition to blue
    color_morph(&star_rgb[2], 0); // transition to black (all off)
    
    if(++text_pass >= 8) text_pass = 0;
    matrix.setTextColor(text_colours[text_pass]);
    rand_messages = random(0,8); // Random number between 0 and 8 (n-1)
  } 
  matrix.show();
  delay(100); 
}

