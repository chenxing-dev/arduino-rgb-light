// 最小测试代码（点亮首颗LED为红色）
#include <FastLED.h>
#define LED_PIN 6
#define NUM_LEDS 1 // 仅测试第一颗LED

CRGB leds[NUM_LEDS];

void setup() {
  delay(1000); // 给灯带上电稳定时间
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  leds[0] = CRGB::Red; // 设置为红色
  FastLED.show();
}

void loop() {} // 保持常亮