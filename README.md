## 本仓库为 rmsc-mod 项目的嵌入式部分  

- **开发环境:** VScode with PlatformIO
- **开发平台:** Atmel AVR (5.1.0) > Arduino Uno R3
- **硬件信息:**
    - **主芯片:** ATMEGA328P 16MHz, 2KB RAM, 31.50KB Flash
    - **OLDE屏幕:** SH1106, 128x64, I2C
    - **旋转编码器:** EC11
- **字库:**
    - **原字体:** ```缝合像素字体/Fusion Pixel Font``` v2025.03.14 release
    - **字符集支持:** 请参考 [Fusion Pixel 10px 等宽模式](https://github.com/TakWolf/fusion-pixel-font/blob/master/docs/info-10px-monospaced.md)
    - **字形数据大小:** 全角字符 13 Byte, 半角字符 10 Byte
    - **像素大小:** 10px
- **许可证:**
    - **程序部分:** MIT License
    - **JSON 格式字体:** SIL Open Font License 1.1

考虑到芯片存储空间, 项目采用了外置字库的方式显示所需字符  
采用 ``` 缝合像素字体/Fusion Pixel Font ``` 项目的 ``` fusion-pixel-10px-monospaced-zh_hans.bdf ``` 字体文件  
将文件中 ```BITMAP``` 部分提取并进一步压缩后格式化为 ```.json``` 文件

> **[缝合像素字体 / Fusion Pixel Font](https://github.com/TakWolf/fusion-pixel-font)**  
> **[rmsc-mod 项目仓库](https://github.com/Voemp/rmsc-mod)**  
