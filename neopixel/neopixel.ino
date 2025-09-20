// Change color only when the button is pressed (debounced)
#include <Adafruit_NeoPixel.h>

#define PIN 6         // NeoPixel data pin (to LED DIN)
#define CONTROL_PIN 2 // Push button pin (to GND when pressed)
#define NUMPIXELS 4   // Number of NeoPixels in strip

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Current color
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

// Simple press latch
bool isPressed = false;

void applyColor()
{
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(redColor, greenColor, blueColor));
  }
  pixels.show();
}

void pickRandomColor()
{
  redColor = random(0, 255);
  greenColor = random(0, 255);
  blueColor = random(0, 255);
}

void setup()
{
  // Initialize the NeoPixel library and button input
  pixels.begin();
  pinMode(CONTROL_PIN, INPUT_PULLUP); // Wire button between CONTROL_PIN and GND

  // Seed RNG (optional, improves randomness)
  randomSeed(analogRead(A0));

  // Start with a random color
  pickRandomColor();
  applyColor();
}

void loop()
{
  // Read the button (raw)
  bool reading = digitalRead(CONTROL_PIN); // HIGH = not pressed, LOW = pressed

  // Immediate trigger on press (falling edge)
  if (!isPressed && reading == LOW)
  {
    pickRandomColor();
    applyColor();
    isPressed = true; // ignore until release
  }

  // Re-arm on release
  if (isPressed && reading == HIGH)
  {
    isPressed = false;
  }
}