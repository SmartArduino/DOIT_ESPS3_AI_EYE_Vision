# DOIT_ESPS3_AI_EYE_Vision

*   首先致谢虾哥的开源项目：https://github.com/78/xiaozhi-esp32
*   其次致谢：https://github.com/xinnan-tech/xiaozhi-esp32-server


## 项目简介
ESP32-S3作为主控，驱动两个1.28寸双目，一个摄像头，并且支持4个触目节点，软件硬件全开源

特色：

1. 多模态S3 AI开发板。
2. 双目，完全支持小智。
3. 软件、硬件完全开源，硬件资料可以直接生产

## 效果视频
https://www.bilibili.com/video/BV15578zTEWF/ 

## 使用说明
1. 获取代码：git clone https://github.com/SmartArduino/DOIT_ESPS3_AI_EYE_Vision.git
2. 使用vscode打开工程（需espidf版本>5.3.2）,设置目标芯片为esp32s3，命令：idf.py set-target esp32s3
3. 编译工程：idf.py build
4. 修改menuconfig:idf.py menuconfig
    将Xiaozhi Assistant->Board Type设置为Doit-AI-EYE-VISION
5. 烧录代码:idf.py flash


## 硬件部分
双目摄像头硬件资料.zip
## 技术支持
![alt text](image.png)

## 购买链接
