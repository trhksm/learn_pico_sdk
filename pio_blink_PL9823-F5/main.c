#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "blink_PL9823F5.pio.h"
#include <stdio.h>
#include <stdint.h>

//未定義な場合上書きする定数値
#ifndef LED_RED_GPIO
#define LED_RED_GPIO 6
#endif
#ifndef LED_GREEN_GPIO
#define LED_GREEN_GPIO 7
#endif
#ifndef LED_BLUE_GPIO
#define LED_BLUE_GPIO 8
#endif
#ifndef LED_WHITE_GPIO
#define LED_WHITE_GPIO 9
#endif

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_t;

/*
 * @param pio (pio0 or pio1)
 * @param state machine番号 (0~3)
 * @param offset PIO命令をロードした開始位置 
 * @param pin GPIOの番号
 * @param freq 周波数
 * @param rgb_value RGBの値
 */
void set_PL9823F5(PIO pio, uint sm, uint offset, uint pin, rgb_t rgb_value);
/*
 * @brief 光らすrgb値を送る
 * @param pio
 * @param sm
 * @param color rgb値を保存した構造体
 */
void send_PL9823F5(PIO pio, uint sm, rgb_t color);
/* @brief rgb値の構造体をsend用のbitの形に変える
 * @param rgb_value
*/
uint32_t convert_rgb(rgb_t rgb_value);

int main() {
    // UART初期化
    stdio_init_all();
    sleep_ms(2000);

    // (デバッグ falseで停止)GPIOの番号チェック
    assert(LED_RED_GPIO < 30);
    assert(LED_GREEN_GPIO < 30);
    assert(LED_BLUE_GPIO < 30);
    assert(LED_WHITE_GPIO < 30);

    // 使うPIO(pio0 or pio1)
    PIO pio = pio0;
    // 使うSM番号(*2)
    uint sm[4];
    // PIO命令をロード
    uint offset = pio_add_program(pio, &blink_PL9823F5_program);
    // PIOの開始位置とPIOの使った番号0/1表示
    printf("Loaded program at %u on pio %u \n", offset, PIO_NUM(pio));

    //RGB設定
    rgb_t red = {255,0,0};
    rgb_t green = {0,255,0};
    rgb_t blue = {0,0,255};
    rgb_t white = {255,255,255};
    rgb_t off = {0,0,0};
    uint32_t hz[4] = {1,2,3,4};
    rgb_t oncol[4] = {
        red,
        green,
        blue,
        white
    };

    // 使ってないsmを割り当て、LEDを光らす( *4 )
    sm[0] = pio_claim_unused_sm(pio, true);
    set_PL9823F5(pio, sm[0], offset, LED_RED_GPIO, red);

    sm[1] = pio_claim_unused_sm(pio, true);
    set_PL9823F5(pio, sm[1], offset, LED_GREEN_GPIO, green);

    sm[2] = pio_claim_unused_sm(pio, true);
    set_PL9823F5(pio, sm[2], offset, LED_BLUE_GPIO, blue);

    sm[3] = pio_claim_unused_sm(pio, true);
    set_PL9823F5(pio, sm[3], offset, LED_WHITE_GPIO, white);

    printf("All sm was assigned "
        "\nsm0 : %u ;\n sm1 : %u ;\n sm2 : %u ;\n sm3 : %u ;\n"
        , sm[0], sm[1], sm[2], sm[3]);
    
    printf("All LEDs would be flashing \n");

    bool state_on[4] = {false, false, false, false};
    //現在時刻を起動してからの経過時間に変換
    uint32_t now = to_ms_since_boot(get_absolute_time());
    //切り替え時間の保存
    uint32_t next_ms[4] = {now, now, now, now};
    
    while (true) {
        now = to_ms_since_boot(get_absolute_time());

        for (int i = 0; i < 4; i++) {
            //切り替え間隔の計算
            uint32_t half_period_ms = 500 / hz[i];
            //切り替え時間よりすぎてたら
            if ((int32_t)(now - next_ms[i]) >= 0) {
                //切り替え
                state_on[i] = !state_on[i];
                send_PL9823F5(pio, sm[i], state_on[i] ? oncol[i] : off);
                //次の切り替え時間を更新 += half_period_msだと正しいタイミングを追求する
                next_ms[i] = now + half_period_ms;
            }
        }
        //忙待ちループと知らせる
        tight_loop_contents();
    }
}

uint32_t convert_rgb (rgb_t rgb_value) {
    return (uint32_t)rgb_value.r << 16 | (uint32_t)rgb_value.g << 8 | (uint32_t)rgb_value.b;
}

void send_PL9823F5(PIO pio, uint sm, rgb_t color) {
    uint32_t rgb = convert_rgb(color);
    //ブロッキングしながらFIFOにデータを送る
    pio_sm_put_blocking(pio, sm, rgb << 8);
    sleep_us(300);
}

void set_PL9823F5(PIO pio, uint sm, uint offset, uint pin, rgb_t rgb_value) {
    //pioのsmを初期化(.pio.hファイルが自動生成)
    blink_PL9823F5_program_init(pio, sm, offset, pin);
    //smを実行し始める
    pio_sm_set_enabled(pio, sm, true);

    printf("\nBlinking pin %d\n", pin);
    printf("color : [%i, %i, %i]\n\n", rgb_value.r, rgb_value.g, rgb_value.b);
}