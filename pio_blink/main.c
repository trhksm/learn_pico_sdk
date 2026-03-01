#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "blink.pio.h"
#include <stdio.h>

//未定義な場合上書きする定数値
#ifndef LED1_GPIO
#define LED1_GPIO 1
#endif
#ifndef LED2_GPIO
#define LED2_GPIO 2
#endif
#ifndef LED3_GPIO
#define LED3_GPIO 3
#endif
#ifndef LED4_GPIO
#define LED4_GPIO 4
#endif

/* 関数プロトタイプ宣言
 * @param pio (pio1 or pio2)
 * @param state machine番号 (0~3)
 * @param offset PIO命令をロードした開始位置 
 * @param pin GPIOの番号
 * @param freq 点滅周波数
 */
void blink_pin(PIO pio, uint sm, uint offset, uint pin, uint freq);

int main() {
    // UART初期化
    stdio_init_all();
    sleep_ms(2000);

    // (デバッグ falseで停止)GPIOの番号チェック
    assert(LED1_GPIO < 31);
    assert(LED2_GPIO < 31);
    assert(LED3_GPIO < 31);
    assert(LED4_GPIO < 31);

    // 使うPIO(pio0 or pio1)
    PIO pio = pio0;
    // 使うSM番号(*2)
    uint sm[4];
    // PIO命令をロード
    uint offset = pio_add_program(pio, &blink_program);
    // PIOの開始位置とPIOの使った番号0/1表示
    printf("Loaded program at %u on pio %u \n", offset,PIO_NUM(pio));

    // 使ってないsmを割り当て、LEDを光らす( *4 )
    sm[0] = pio_claim_unused_sm(pio, true);
    blink_pin(pio, sm[0], offset, LED1_GPIO, 1);

    sm[1] = pio_claim_unused_sm(pio, true);
    blink_pin(pio, sm[1], offset, LED2_GPIO, 2);

    sm[2] = pio_claim_unused_sm(pio, true);
    blink_pin(pio, sm[2], offset, LED3_GPIO, 3);

    sm[3] = pio_claim_unused_sm(pio, true);
    blink_pin(pio, sm[3], offset, LED4_GPIO, 4);

    printf("All sm was assigned \n"
        "sm0 : %u ;\n sm1 : %u ;\n sm2 : %u ;\n sm3 : %u ;\n"
        , sm[0], sm[1], sm[2], sm[3]);
    
    printf("All LEDs would be flashing \n");
    while (true) {
        tight_loop_contents();
    }
}

void blink_pin(PIO pio, uint sm, uint offset, uint pin, uint freq) {
    //周波数0以上を保証
    hard_assert(freq > 0);
    //pioのsmを初期化(.pio.hファイルが自動生成)
    blink_program_init(pio, sm, offset, pin);
    //smを実行し始める
    pio_sm_set_enabled(pio, sm, true);

    printf("Blinking pin %d at %d Hz\n", pin, freq);
    //TX FIFOに値を保存(目標の半分 - 消費する命令数)
    pio->txf[sm] = (clock_get_hz(clk_sys) / (2 * freq)) - 3;
}