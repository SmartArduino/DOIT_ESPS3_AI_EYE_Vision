
#include "wifi_board.h"
#include "audio_codecs/vb6824_audio_codec.h"
#include "display/lcd_display.h"
#include "application.h"
#include "button.h"
#include "config.h"
#include "iot/thing_manager.h"

#include <wifi_station.h>
#include <esp_log.h>
#include <esp_lcd_panel_vendor.h>
#include <driver/spi_common.h>
#include <esp_lcd_gc9a01.h>

#include "font_awesome_symbols.h"
#include "settings.h"

#include <wifi_station.h>
#include <wifi_configuration_ap.h>
#include <ssid_manager.h>
#include "assets/lang_config.h"

#include "display/eye_display.h"
#include "esp32_camera.h"

#define TAG "CustomBoard"

LV_FONT_DECLARE(font_puhui_20_4);   //表示名为 puhui 的字体,是一个常规字体，用于显示文本内容，大小为 14
LV_FONT_DECLARE(font_awesome_20_4); //表示名为 font_awesome 的字体,是一个图标字体库，用于显示各种图标，大小为 14



class CustomBoard : public WifiBoard {
private:
    Button boot_button_;
    VbAduioCodec audio_codec;
    LcdDisplay* display_;
    Esp32Camera* camera_;

    /* LCD IO and panel */
   esp_lcd_panel_io_handle_t lcd_io1 = NULL;
   esp_lcd_panel_handle_t lcd_panel1 = NULL;
   esp_lcd_panel_io_handle_t lcd_io2 = NULL;
   esp_lcd_panel_handle_t lcd_panel2 = NULL;



    void InitializeButtons() {
        boot_button_.OnClick([this]() {
            if (audio_codec.InOtaMode(1) == true) {
                ESP_LOGI(TAG, "OTA mode, do not enter chat");
                return;
            }
            auto &app = Application::GetInstance();
            app.ToggleChatState();
        });
        boot_button_.OnPressRepeat([this](uint16_t count) {
            if(count >= 3 && audio_codec.InOtaMode(1) == false){
                ResetWifiConfiguration();
            }
        });
        boot_button_.OnLongPress([this]() {
            if (esp_timer_get_time() > 20 * 1000 * 1000) {
                ESP_LOGI(TAG, "Long press, do not enter OTA mode %ld", (uint32_t)esp_timer_get_time());
                return;
            }
            audio_codec.OtaStart(0);
        });
    }
    // 物联网初始化，添加对 AI 可见设备
    void InitializeIot() {
        auto& thing_manager = iot::ThingManager::GetInstance();
        thing_manager.AddThing(iot::CreateThing("Speaker"));
        thing_manager.AddThing(iot::CreateThing("Screen"));
    }


    // GC9A01-SPI2初始化-用于显示小智
    void InitializeSpiEye1() {
        const spi_bus_config_t buscfg = {       
            .mosi_io_num = GC9A01_SPI1_LCD_GPIO_MOSI,
            .miso_io_num = GPIO_NUM_NC,
            .sclk_io_num = GC9A01_SPI1_LCD_GPIO_SCLK,
            .quadwp_io_num = GPIO_NUM_NC,
            .quadhd_io_num = GPIO_NUM_NC,
            .max_transfer_sz = GC9A01_LCD_H_RES * GC9A01_LCD_V_RES * sizeof(uint16_t), // 增大传输大小,
        };
        ESP_ERROR_CHECK(spi_bus_initialize(GC9A01_LCD_SPI1_NUM, &buscfg, SPI_DMA_CH_AUTO));
    }

    // GC9A01-SPI2初始化-用于显示魔眼
    void InitializeGc9a01DisplayEye1() {
        ESP_LOGI(TAG, "Init GC9A01 display1");

        ESP_LOGI(TAG, "Install panel IO1");
        ESP_LOGD(TAG, "Install panel IO1");
        const esp_lcd_panel_io_spi_config_t io_config = {
            .cs_gpio_num = GC9A01_SPI1_LCD_GPIO_CS,
            .dc_gpio_num = GC9A01_SPI1_LCD_GPIO_DC,
            .spi_mode = 0,
            .pclk_hz = GC9A01_LCD_PIXEL_CLK_HZ,
            .trans_queue_depth = 10,
            .lcd_cmd_bits = GC9A01_LCD_CMD_BITS,
            .lcd_param_bits = GC9A01_LCD_PARAM_BITS,
            

        };
        esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)GC9A01_LCD_SPI1_NUM, &io_config, &lcd_io1);
        lcd_io_eye = lcd_io1;

        ESP_LOGD(TAG, "Install LCD1 driver");
        esp_lcd_panel_dev_config_t panel_config = {
            .reset_gpio_num = GC9A01_SPI1_LCD_GPIO_RST,
            .color_space = GC9A01_LCD_COLOR_SPACE,
            .bits_per_pixel = GC9A01_LCD_BITS_PER_PIXEL,
            
        };
        panel_config.rgb_endian = DISPLAY_RGB_ORDER;
        esp_lcd_new_panel_gc9a01(lcd_io1, &panel_config, &lcd_panel1);
        lcd_panel_eye = lcd_panel1;
        
        esp_lcd_panel_reset(lcd_panel1);
        esp_lcd_panel_init(lcd_panel1);
        esp_lcd_panel_invert_color(lcd_panel1, true);
        esp_lcd_panel_disp_on_off(lcd_panel1, true);
    }

    // GC9A01-SPI2初始化-用于显示魔眼
    void InitializeSpiEye2() {
        const spi_bus_config_t buscfg = {       
            .mosi_io_num = GC9A01_SPI2_LCD_GPIO_MOSI,
            .miso_io_num = GPIO_NUM_NC,
            .sclk_io_num = GC9A01_SPI2_LCD_GPIO_SCLK,
            .quadwp_io_num = GPIO_NUM_NC,
            .quadhd_io_num = GPIO_NUM_NC,
            .max_transfer_sz = GC9A01_LCD_H_RES * GC9A01_LCD_V_RES * sizeof(uint16_t),
        };
        ESP_ERROR_CHECK(spi_bus_initialize(GC9A01_LCD_SPI2_NUM, &buscfg, SPI_DMA_CH_AUTO));
    }

    // GC9A01-SPI2初始化-用于显示魔眼
    void InitializeGc9a01DisplayEye2() {
        ESP_LOGI(TAG, "Init GC9A01 display2");

        ESP_LOGI(TAG, "Install panel IO2");
        ESP_LOGD(TAG, "Install panel IO2");
        const esp_lcd_panel_io_spi_config_t io_config = {
            .cs_gpio_num = GC9A01_SPI2_LCD_GPIO_CS,
            .dc_gpio_num = GC9A01_SPI2_LCD_GPIO_DC,
            .spi_mode = 0,
            .pclk_hz = GC9A01_LCD_PIXEL_CLK_HZ,
            .trans_queue_depth = 10,
            .lcd_cmd_bits = GC9A01_LCD_CMD_BITS,
            .lcd_param_bits = GC9A01_LCD_PARAM_BITS,


        };
        esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)GC9A01_LCD_SPI2_NUM, &io_config, &lcd_io2);
        lcd_io_eye2 = lcd_io2;

        ESP_LOGD(TAG, "Install LCD2 driver");
        esp_lcd_panel_dev_config_t panel_config = {
            .reset_gpio_num = GC9A01_SPI2_LCD_GPIO_RST,
            .color_space = GC9A01_LCD_COLOR_SPACE,
            .bits_per_pixel = GC9A01_LCD_BITS_PER_PIXEL
        };
        panel_config.rgb_endian = DISPLAY_RGB_ORDER;
        esp_lcd_new_panel_gc9a01(lcd_io2, &panel_config, &lcd_panel2);
        lcd_panel_eye2 = lcd_panel2;
        esp_lcd_panel_reset(lcd_panel2);
        esp_lcd_panel_init(lcd_panel2);
        esp_lcd_panel_invert_color(lcd_panel2, true);
        esp_lcd_panel_disp_on_off(lcd_panel2, true);
        
    }

    //初始化双屏
    void InitializeDualScreenEye(){
        // 初始化第一块屏幕
        InitializeSpiEye1();
        InitializeSpiEye2();
        InitializeGc9a01DisplayEye1();
        InitializeGc9a01DisplayEye2();

        // 创建双屏显示对象
        display_ = new DualScreenDisplay(
            lcd_io1, lcd_panel1,
            lcd_io2, lcd_panel2,
            DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y,
            DISPLAY_MIRROR_X, DISPLAY_MIRROR_Y, DISPLAY_SWAP_XY,
            {
                .text_font = &font_puhui_20_4,
                .icon_font = &font_awesome_20_4,
                .emoji_font = font_emoji_64_init(),
            }
        );
    }

    void InitializeCamera() {
        // Open camera power

        camera_config_t config = {};
        config.ledc_channel = LEDC_CHANNEL_0;  // LEDC通道选择  用于生成XCLK时钟 但是S3不用
        config.ledc_timer = LEDC_TIMER_0; // LEDC timer选择  用于生成XCLK时钟 但是S3不用
        config.pin_d0 = CAMERA_PIN_D0;
        config.pin_d1 = CAMERA_PIN_D1;
        config.pin_d2 = CAMERA_PIN_D2;
        config.pin_d3 = CAMERA_PIN_D3;
        config.pin_d4 = CAMERA_PIN_D4;
        config.pin_d5 = CAMERA_PIN_D5;
        config.pin_d6 = CAMERA_PIN_D6;
        config.pin_d7 = CAMERA_PIN_D7;
        config.pin_xclk = CAMERA_PIN_XCLK;
        config.pin_pclk = CAMERA_PIN_PCLK;
        config.pin_vsync = CAMERA_PIN_VSYNC;
        config.pin_href = CAMERA_PIN_HREF;
        config.pin_sccb_sda = CAMERA_PIN_SIOD;   // 这里写-1 表示使用已经初始化的I2C接口
        config.pin_sccb_scl = CAMERA_PIN_SIOC;
        // config.sccb_i2c_port = 1;
        config.pin_pwdn = CAMERA_PIN_PWDN;
        config.pin_reset = CAMERA_PIN_RESET;
        config.xclk_freq_hz = XCLK_FREQ_HZ;
        config.pixel_format = PIXFORMAT_RGB565;
        config.frame_size = FRAMESIZE_VGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
        config.fb_location = CAMERA_FB_IN_PSRAM;
        config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;

        camera_ = new Esp32Camera(config);
    }
public:
    CustomBoard() : boot_button_(BOOT_BUTTON_GPIO), audio_codec(CODEC_TX_GPIO, CODEC_RX_GPIO){          
        InitializeButtons();
        InitializeIot();
        // InitializeDualScreenEye();
        InitializeCamera();

        // GetBacklight()->RestoreBrightness();
        audio_codec.OnWakeUp([this](const std::string& command) {
            if (command == std::string(vb6824_get_wakeup_word())){
                if(Application::GetInstance().GetDeviceState() != kDeviceStateListening){
                    Application::GetInstance().WakeWordInvoke("你好小智");
                }
            }else if (command == "开始配网"){
                ResetWifiConfiguration();
            }
        });
    
    }

    virtual AudioCodec* GetAudioCodec() override {
        return &audio_codec;
    }

    virtual Display* GetDisplay() override {
        return display_;
    }

    virtual Backlight* GetBacklight() override {
        if (DISPLAY_BACKLIGHT_PIN != GPIO_NUM_NC) {
            static PwmBacklight backlight(DISPLAY_BACKLIGHT_PIN, DISPLAY_BACKLIGHT_OUTPUT_INVERT);
            return &backlight;
        }
        return nullptr;
    }

    void EnterWifiConfigMode() {
        auto& application = Application::GetInstance();
        application.SetDeviceState(kDeviceStateWifiConfiguring);

        auto& wifi_ap = WifiConfigurationAp::GetInstance();
        wifi_ap.SetLanguage(Lang::CODE);
        wifi_ap.SetSsidPrefix("Xiaozhi");
        wifi_ap.Start();

        // 显示 WiFi 配置 AP 的 SSID 和 Web 服务器 URL
        std::string hint = Lang::Strings::CONNECT_TO_HOTSPOT;
        hint += wifi_ap.GetSsid();
        hint += Lang::Strings::ACCESS_VIA_BROWSER;
        hint += wifi_ap.GetWebServerUrl();
        hint += "\n\n";
        
        // 播报配置 WiFi 的提示
        application.Alert(Lang::Strings::WIFI_CONFIG_MODE, hint.c_str(), "", Lang::Sounds::P3_WIFICONFIG);

        vTaskDelay(pdMS_TO_TICKS(3500));

        // 可以释放opus编码器的内存
        
        // Wait forever until reset after configuration
        while (true) {
            int free_sram = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
            int min_free_sram = heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL);
            ESP_LOGI(TAG, "Free internal: %u minimal internal: %u", free_sram, min_free_sram);
            vTaskDelay(pdMS_TO_TICKS(10000));
        }
    }
    // virtual Camera* GetCamera() override {
    //     return camera_;
    // }
};

DECLARE_BOARD(CustomBoard);
