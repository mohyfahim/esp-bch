#include <stdio.h>
#include "bch.h"
#include "esp_heap_caps.h"
#include <string.h>
#include "esp_log.h"
#include <inttypes.h>
#include "esp_timer.h"
#include "esp_rom_md5.h"

static const char *TAG = "example";

void app_main(void)
{
    long unsigned int t = esp_timer_get_time();
    uint8_t data[4] = {0b10101101, 0b01111010, 0b11010110, 0b10110101};     // 32 bit data
    uint8_t err_data[4] = {0b10001101, 0b11111010, 0b11010110, 0b10110101}; // 2 - 8 err locations
    unsigned int errloc[40] = {0};
    struct bch_control *bch;
    bch = init_bch(7, 15, 0);

    ESP_LOGW(TAG, "size of ecc %d bytes", bch->ecc_bytes);
    ESP_LOGW(TAG, "size of ecc %d bits", bch->ecc_bits);
    uint8_t *ecc = heap_caps_malloc(bch->ecc_bytes, MALLOC_CAP_8BIT);
    memset(ecc, 0, bch->ecc_bytes);
    ESP_LOGW(TAG, "size of data = %d ", bch->n - bch->ecc_bits);

    encode_bch(bch, data, 4, ecc);

    for (int i = 0; i < bch->ecc_bytes; i++)
    {
        printf("%d ", ecc[i]);
    }
    printf("\n");

    int err = decode_bch(bch, err_data, 4, ecc, NULL, NULL, errloc);
    ESP_LOGW(TAG, "decode err is = %d", err);

    for (int i = 0; i < 4; i++)
    {
        printf("%u", err_data[i]);
    }
    printf("\n");

    for (int i = 0; i < 40; i++)
    {
        printf("%d ", errloc[i]);
        if (errloc[i] != 0 && errloc[i] < 8 * 4)
        {
            err_data[errloc[i] / 8] ^= 1 << (errloc[i] % 8);
        }
    }
    printf("\n");

    for (int i = 0; i < 4; i++)
    {
        printf("%u ", data[i]);
    }
    printf("\n");
    for (int i = 0; i < 4; i++)
    {
        printf("%u ", err_data[i]);
    }
    printf("\n");

    md5_context_t ctx;
    uint8_t digest[16] = {0};
    esp_rom_md5_init(&ctx);
    esp_rom_md5_update(&ctx, err_data, 4);
    esp_rom_md5_final(digest, &ctx);
    for (int i = 0; i < 16; i++)
    {
        printf("%u ", digest[i]);
    }
    printf("\n");
    ESP_LOGW(TAG, "%lu", (long unsigned int)esp_timer_get_time() - t);
}
