# Micro Message Pop

## 简介

Micro Message Pop 是一个基于 ESP8266/ESP32 的微型消息推送与提醒设备，支持 MQTT 消息、蜂鸣器、RGB 灯等功能。
支持通过串口动态配置 WiFi 网络参数，适合物联网场景下的快速部署和远程控制。

---

## 快速开始

1. **硬件准备**
   - ESP8266 或 ESP32 开发板
   - 蜂鸣器、RGB LED（NeoPixel）
   - Micro USB 数据线

2. **编译环境**
   - [PlatformIO](https://platformio.org/)
   - VS Code

3. **编译与烧录**
   - 克隆本项目
   - 配置 `platformio.ini`
   - 连接开发板，点击 PlatformIO 的“Upload”按钮

---

## 串口配置模式说明

设备支持**随时通过串口进入配置模式**，用于设置或修改 WiFi 账号和密码。

### 进入配置模式的方式

- **开机时未检测到有效 WiFi 配置**，自动进入配置模式
- **WiFi 连接失败**时，自动进入配置模式
- **任意时刻**，在串口监视器输入 `c` 或 `C`（回车），即可强制进入配置模式

### 配置菜单

串口会显示如下菜单：

```
Please select an operation:
1. Configure WiFi
2. View hardware resources and network status
3. Exit
Or send 'c' or 'C' at any time to enter configuration mode.
```

- 输入 `1` 或 `c`/`C`（加回车）进入 WiFi 配置
- 输入 `2` 查看硬件和网络状态
- 输入 `3` 退出配置模式

**所有菜单项均支持带回车符（如 `1\n`、`c\n`）输入**

---

## MQTT 功能

- 支持 MQTT 消息推送
- 主题格式：`users/<USER_ID>/groups/<GROUP_ID>/<DEVICE_ID>/commands`
- 支持 LED 控制、蜂鸣器控制等命令

---

## 代码结构

- `main.cpp`：主流程
- `serial_config.*`：串口与 WiFi 配置
- `mqtt_handler.*`：MQTT 连接与消息处理
- `buzzer_handler.*`：蜂鸣器控制
- `neopixel_handler.*`：RGB LED 控制

---

## License

本项目基于 GNU AGPL v3 开源协议发布。

