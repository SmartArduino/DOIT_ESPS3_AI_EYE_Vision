#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#include <driver/gpio.h>

#define BOOT_BUTTON_GPIO        GPIO_NUM_NC

#define CODEC_TX_GPIO           GPIO_NUM_45
#define CODEC_RX_GPIO           GPIO_NUM_48
/*==========小智+魔眼============ */
/* LCD size */
#define GC9A01_LCD_H_RES   (240)
#define GC9A01_LCD_V_RES   (240)
/* LCD settings */
#define GC9A01_LCD_SPI1_NUM         (SPI3_HOST)
#define GC9A01_LCD_SPI2_NUM         (SPI2_HOST)
#define GC9A01_LCD_PIXEL_CLK_HZ    (20 * 1000 * 1000)
#define GC9A01_LCD_CMD_BITS        (8)
#define GC9A01_LCD_PARAM_BITS      (8)
#define GC9A01_LCD_COLOR_SPACE     (ESP_LCD_COLOR_SPACE_RGB)
#define GC9A01_LCD_BITS_PER_PIXEL  (16)
#define GC9A01_LCD_DRAW_BUFF_DOUBLE (1)
#define GC9A01_LCD_DRAW_BUFF_HEIGHT (240)
#define GC9A01_LCD_BL_ON_LEVEL     (1)
/* LCD-SPI2 pins */
#define GC9A01_SPI2_LCD_GPIO_SCLK       (GPIO_NUM_38)
#define GC9A01_SPI2_LCD_GPIO_MOSI       (GPIO_NUM_39)
#define GC9A01_SPI2_LCD_GPIO_RST        (GPIO_NUM_40)
#define GC9A01_SPI2_LCD_GPIO_DC         (GPIO_NUM_0)
#define GC9A01_SPI2_LCD_GPIO_CS         (GPIO_NUM_NC)
#define GC9A01_SPI2_LCD_GPIO_BL         (GPIO_NUM_NC)
#define GC9A01_SPI2_LCD_GPIO_MISO       (GPIO_NUM_NC)

/* LCD-SPI1 pins */
#define GC9A01_SPI1_LCD_GPIO_SCLK       (GPIO_NUM_42)
#define GC9A01_SPI1_LCD_GPIO_MOSI       (GPIO_NUM_2)
#define GC9A01_SPI1_LCD_GPIO_RST        (GPIO_NUM_1)
#define GC9A01_SPI1_LCD_GPIO_DC         (GPIO_NUM_41)
#define GC9A01_SPI1_LCD_GPIO_CS         (GPIO_NUM_NC)
#define GC9A01_SPI1_LCD_GPIO_BL         (GPIO_NUM_NC)
#define GC9A01_SPI1_LCD_GPIO_MISO       (GPIO_NUM_NC)
//分辨率
#define DISPLAY_WIDTH   240
#define DISPLAY_HEIGHT  240
//镜像和轴交换：通常不需要镜像或轴交换，除非你的硬件设计需要特定的显示方向。
#define DISPLAY_MIRROR_X true
#define DISPLAY_MIRROR_Y false
#define DISPLAY_SWAP_XY false
//默认不启用颜色反转。
#define DISPLAY_INVERT_COLOR    true
//RGB顺序：LH128R-IG01使用RGB垂直条纹排列，因此设置为 LCD_RGB_ELEMENT_ORDER_RGB。
#define DISPLAY_RGB_ORDER  LCD_RGB_ELEMENT_ORDER_BGR
//偏移量：通常不需要偏移，除非你的显示屏有特定的显示区域限制。
#define DISPLAY_OFFSET_X  0
#define DISPLAY_OFFSET_Y  0
//背光输出取反：默认不取反，除非你的背光控制电路需要取反信号。
#define DISPLAY_BACKLIGHT_OUTPUT_INVERT true
#define DISPLAY_BACKLIGHT_PIN GPIO_NUM_NC
#define DISPLAY_SPI_SCLK_HZ     (40 * 1000 * 1000)


//camera

/* Camera pins */
#define CAMERA_PIN_PWDN -1
#define CAMERA_PIN_RESET -1
#define CAMERA_PIN_XCLK GPIO_NUM_16
#define CAMERA_PIN_SIOD GPIO_NUM_4
#define CAMERA_PIN_SIOC GPIO_NUM_5

#define CAMERA_PIN_D7 GPIO_NUM_15
#define CAMERA_PIN_D6 GPIO_NUM_17
#define CAMERA_PIN_D5 GPIO_NUM_18
#define CAMERA_PIN_D4 GPIO_NUM_47
#define CAMERA_PIN_D3 GPIO_NUM_10
#define CAMERA_PIN_D2 GPIO_NUM_3
#define CAMERA_PIN_D1 GPIO_NUM_9
#define CAMERA_PIN_D0 GPIO_NUM_21
#define CAMERA_PIN_VSYNC GPIO_NUM_6
#define CAMERA_PIN_HREF GPIO_NUM_7
#define CAMERA_PIN_PCLK GPIO_NUM_8

#define XCLK_FREQ_HZ 10000000
#endif // _BOARD_CONFIG_H_
