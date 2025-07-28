# Arduino RGB灯带系统

![Project Demo](demo.jpeg)

## 项目概述
An Arduino-based RGB lighting system, featuring 5 lighting modes.

## 🌟 Features

### 🎛️ 控制方式
- **按键控制**：
  - 短按：循环切换灯光模式
  - 长按(>1秒)：开关灯

## 硬件清单

### 必需组件
| 组件                | 数量 | 参考价格 |
| ------------------- | ---- | -------- |
| Arduino Uno R3      | 1    | ¥161     |
| WS2812B灯带60灯 5V  | 1    | ¥27      |
| 轻触开关 12×12 四脚 | 1    | ¥0.5     |
| 330Ω电阻            | 1    | ¥0.1     |

**总成本**：约¥200

## 接线图

```mermaid
graph TB
  subgraph 电源
    computer[Computer]
  end

  subgraph Arduino
    computer --> arduinoUSB[Arduino USB port]
    arduino5V[Arduino +5V]
    arduinoGND[Arduino GND]
    arduinoD2[Arduino D2]
    arduinoD6[Arduino D6]
  end

  subgraph 面包板
    arduino5V -->|红跳线| bbVCC[面包板正极区]
    bbGND[面包板负极区] -->|白跳线| arduinoGND
    arduinoD6 -->|绿跳线| resistor[330Ω电阻]
  end
  
  subgraph 灯带
    bbVCC --> ledVCC1[灯带5V]
    resistor -->|蓝跳线| din[灯带DIN（绿色线）]
    ledGND1[灯带GND] --> bbGND
  end

  subgraph 按键
    arduinoD2 -->|黄跳线| btnPin1[按键引脚1]
    btnPin1 -->|内部连接| btnPin2[按键引脚2]
    btnPin2 -->|按下时导通| btnPin4[按键引脚4]
    btnPin4 -->|内部连接| btnPin3[按键引脚3]
    btnPin3 -->|灰跳线| bbGND
    btnPin1[按键引脚1] -->|按下时导通| btnPin3
  end

  classDef computer fill:#f99,stroke:#333;
  classDef arduino fill:#9cf,stroke:#333;
  classDef bb fill:#ff9,stroke:#333;
  classDef led fill:#9f9,stroke:#333;
  classDef cap fill:#f9f,stroke:#333;
  classDef btn fill:#cdf,stroke:#333;
  
  class computer computer;
  class arduinoUSB,arduino5V,arduinoGND,arduinoD6,arduinoD2 arduino;
  class bbVCC,bbGND bb;
  class ledVCC1,ledVCC2,ledGND1,ledGND2,din,resistor led;
  class capPos,capNeg cap;
  class btnPin1,btnPin2,btnPin3,btnPin4 btn;
```

## 安装指南

### 1. 硬件组装
1. 将灯带VCC/GND连接到面包板正/负极区
2. 灯带DIN通过330Ω电阻接Arduino D6
3. 轻触开关对角脚分别接GND和D2

### 2. 软件 Setup

```bash
# Clone repository
git clone https://github.com/chenxing-dev/arduino-rgb-light.git

# Open in Arduino IDE
1. 安装库：FastLED
2. 选择开发板 (Arduino Uno)
3. 上传程序`rgb_desk_light.ino`
```

### 基本操作
1. **短按按键**：循环切换灯光模式
2. **长按按键(>1秒)**：开关灯

## 故障排除

| 问题       | 解决方案               |
| ---------- | ---------------------- |
| 按键无响应 | 确认开关对角脚接线正确 |

## 贡献指南
欢迎提交Pull Request！

## 许可证
[MIT License](LICENSE)
