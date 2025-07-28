#include <FastLED.h>

// ========== 硬件配置 ==========
#define LED_PIN 6           // 灯带数据引脚
#define BTN_PIN 2           // 按键引脚
#define NUM_LEDS 60         // 灯珠数量
#define MAX_BRIGHTNESS 225  // 最大亮度

// ========== 全局变量 ==========
CRGB leds[NUM_LEDS];        // LED数组
bool isOn = true;           // 开关状态
byte mode = 0;              // 当前模式，初始设为模式1(彩虹)
const byte MODE_COUNT = 5;  // 总模式数
uint8_t gHue = 0;           // 全局色相值
unsigned long buttonPressTime = 0;
bool lastButtonState = HIGH;

// 效果参数
uint8_t pulseBrightness = 255;
uint8_t scanPosition = 0;
bool scanDirection = true;

// 132 BPM节奏参数
uint8_t beatCounter = 0;
unsigned long lastBeatTime = 0;
const unsigned long beatDuration = 454;  // 132 BPM每拍持续时间(ms)

// 颜色定义
const CRGB PINK = CRGB(255, 105, 180);
const CRGB PURPLE = CRGB(180, 0, 255);
const CRGB DEEP_PURPLE = CRGB(100, 0, 200);
const CRGB HOT_PINK = CRGB(255, 50, 255);

// ========== 初始化设置 ==========
void setup() {
  Serial.begin(9600);
  Serial.println("RGB氛围灯启动");

  // 初始化LED
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(MAX_BRIGHTNESS);
  FastLED.setCorrection(TypicalLEDStrip);  // 增强色彩饱和度

  // 初始化按键
  pinMode(BTN_PIN, INPUT_PULLUP);

  // 启动动画
  startupAnimation();
}

// ========== 主循环 ==========
void loop() {
  // 处理按键
  handleButton();

  // 灯光控制
  if (isOn) {
    updateLights();
  } else {
    turnOffLights();
  }

  FastLED.show();
  // 添加小延迟保证串口稳定性
  delay(20);
}

// ========== 按键处理 ==========
void handleButton() {
  // 按键检测（带状态机）
  bool currentButtonState = digitalRead(BTN_PIN);

  // 检测按键按下（下降沿）
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    buttonPressTime = millis();
  }

  // 检测按键释放（上升沿）
  if (currentButtonState == HIGH && lastButtonState == LOW) {
    unsigned long pressDuration = millis() - buttonPressTime;

    if (pressDuration > 1000) {  // 长按开关灯
      isOn = !isOn;
      Serial.print("长按: 开关状态 -> ");
      Serial.println(isOn ? "开" : "关");
    } else if (isOn) {  // 短按切换模式
      mode = (mode + 1) % MODE_COUNT;
      Serial.print("短按切换模式 -> ");
      Serial.println(mode);
      // 模式切换时重置节拍
      beatCounter = 0;
      lastBeatTime = millis();
      // 模式切换特效
      modeChangeEffect();
    }
  }

  lastButtonState = currentButtonState;
}

// ========== 灯光效果 ==========
void updateLights() {
  switch (mode) {
    case 0:  // 能量脉冲
      energyPulse();
      break;

    case 1:  // 极速扫描
      speedScan();
      break;

    case 2:  // 彩虹循环
      rainbow();
      break;

    case 3:  // 节奏脉动
      rhythmPulse132();
      break;

    case 4:  // 光谱波浪
      spectrumWave();
      break;
  }
}

// 效果0: 能量脉冲 - 126BPM电子音乐节奏
void energyPulse() {
  // 126 BPM节奏 (每分钟126拍)
  uint8_t pulse = beatsin8(126 / 2, 100, 255, 0, 0);

  // 使用调色板
  if (pulse > 200) {
    // 粉色主脉冲
    fill_solid(leds, NUM_LEDS, CHSV(230, 255, pulse));
  } else if (pulse > 150) {
    // 蓝色次脉冲
    fill_solid(leds, NUM_LEDS, CHSV(160, 255, pulse));
  } else {
    // 紫色背景
    fill_solid(leds, NUM_LEDS, CHSV(192, 200, pulse / 2 + 100));
  }
}

// 效果1: 极速扫描 - 快速移动的光条
void speedScan() {
  // 淡出效果
  fadeToBlackBy(leds, NUM_LEDS, 50);

  // 移动扫描位置
  if (scanDirection) {
    scanPosition++;
    if (scanPosition >= NUM_LEDS - 5) scanDirection = false;
  } else {
    scanPosition--;
    if (scanPosition <= 5) scanDirection = true;
  }

  // 创建扫描光条
  for (int i = 0; i < 5; i++) {
    int pos = scanPosition + i;
    if (pos < NUM_LEDS) {
      // 蓝-紫-红渐变
      uint8_t hue = 160 + i * 20;
      leds[pos] = CHSV(hue, 255, 255);
    }
  }
}

// 效果2: 彩虹循环
void rainbow() {
  fill_rainbow(leds, NUM_LEDS, gHue, 7);
  gHue++;
}

// 效果3: 节奏脉动（132 BPM适配）
void rhythmPulse132() {
  // 节拍检测（132 BPM = 每拍454ms）
  if (millis() - lastBeatTime > beatDuration) {
    beatCounter = (beatCounter + 1) % 4;  // 4拍循环
    lastBeatTime = millis();

    // 强拍处理（每小节第1拍）
    if (beatCounter == 0) {
      // 中心爆闪效果
      leds[NUM_LEDS / 2] = CRGB(255, 255, 255);  // 白色核心
      // 向外扩散紫色光波
      for (int r = 1; r < 10; r++) {
        int left = NUM_LEDS / 2 - r;
        int right = NUM_LEDS / 2 + r;
        if (left >= 0) leds[left] = CHSV(200, 255, 200);  // 蓝紫色
        if (right < NUM_LEDS) leds[right] = CHSV(200, 255, 200);
      }
    }
  }

  // 非强拍时的动态效果
  EVERY_N_MILLISECONDS(30) {
    // 流动色彩背景（粉紫色渐变）
    for (int i = 0; i < NUM_LEDS; i++) {
      float ratio = (float)i / (NUM_LEDS - 1);
      leds[i] = blend(
        blend(DEEP_PURPLE, PURPLE, ratio * 128),
        blend(PURPLE, HOT_PINK, (ratio - 0.5) * 256),
        ratio * 255);
    }

    // 添加星光粒子（随节奏密度变化）
    if (random8() < 30) {
      int pos = random16(NUM_LEDS);
      leds[pos] = CRGB(255, 105, 180);  // 亮粉色粒子
    }

    // 淡出中心爆闪
    fadeToBlackBy(leds, NUM_LEDS, 15);
  }
}

// 效果4: 光谱波浪 - 流动的色彩波浪
void spectrumWave() {
  // 淡出效果
  fadeToBlackBy(leds, NUM_LEDS, 20);

  // 移动波浪位置
  static uint8_t wavePos = 0;
  wavePos += beatsin8(132 / 2, 2, 10);

  // 创建流动波浪
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t wave = sin8(i * 5 + wavePos);
    uint8_t bri = wave > 128 ? wave : 0;
    leds[i] = CHSV(gHue + i * 5, 255, bri);
  }

  EVERY_N_MILLISECONDS(100) {
    gHue++;
  }
}

// ========== 辅助函数 ==========

void turnOffLights() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
}

void modeChangeEffect() {
  // 模式切换效果
  CRGB effectColor = CHSV(mode * 51, 255, 255);  // 每种模式不同颜色

  // 方向扫描效果
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = effectColor;
    if (i > 0) leds[i - 1] = CRGB::Black;
    FastLED.show();
    delay(10);
  }

  FastLED.clear();
}

void startupAnimation() {
  // 启动序列
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(i * 4, 255, 255);
    FastLED.show();
    delay(132 / 2);  // 132BPM节奏
  }

  FastLED.clear();
}