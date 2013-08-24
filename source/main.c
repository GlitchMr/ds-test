#include <nds.h>
#include <nds/touch.h>
#include <stdlib.h>
#define WIDTH 256
#define HEIGHT 192

void draw_point(int x, int y) {
    VRAM_A[x + y * WIDTH] = RGB15(x * 32 / WIDTH, y * 32 / HEIGHT, 31);
}

int main(void) {
    // Initialize first screen
    videoSetMode(MODE_FB0);
    vramSetBankA(VRAM_A_LCD);

    touchPosition prev = {.px=0, .py=0};
    while (1) {
        touchPosition touch;
        touchRead(&touch);
        if (touch.px == 0 && touch.py == 0 || prev.px == 0 && prev.py == 0) {
            prev = touch;
            draw_point(touch.px, touch.py);
        }
        else {
            while (prev.px != touch.px && prev.py != touch.py) {
                int diff_x = prev.px - touch.px;
                int diff_y = prev.py - touch.py;
                if (abs(diff_x) > abs(diff_y)) {
                    prev.px += diff_x > 0 ? -1 : 1;
                }
                else {
                    prev.py += diff_y > 0 ? -1 : 1;
                }
                draw_point(prev.px, prev.py);
            }
        }
        swiWaitForVBlank();
    }

    // C programs have to return something
    return 0;
}
