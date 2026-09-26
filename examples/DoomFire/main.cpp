#include "SoftDraw.h"

#define WIDTH 320
#define HEIGHT 200

#define FIRE_W WIDTH
#define FIRE_H HEIGHT
#define FIRE_PALETTE_SIZE 37

static uint8_t fire_pixels[FIRE_W * FIRE_H];

static const u32 fire_palette[FIRE_PALETTE_SIZE] = 
{
    0x070707, 0x1F0707, 0x2F0F07, 0x470F07, 0x571707, 0x671F07, 0x771F07,
    0x8F2707, 0x9F2F07, 0xAF3F07, 0xBF4707, 0xC74707, 0xDF4F07, 0xDF5707,
    0xDF5707, 0xD75F07, 0xD75F07, 0xD7670F, 0xCF6F0F, 0xCF770F, 0xCF7F0F,
    0xCF8717, 0xC78717, 0xC78F17, 0xC7971F, 0xBF9F1F, 0xBF9F1F, 0xBFA727,
    0xBFA727, 0xBFAF2F, 0xB7AF2F, 0xB7B72F, 0xB7B737, 0xCFCF6F, 0xDFDF9F,
    0xEFEFC7, 0xFFFFFF
};

int main()
{
    SOFTDRAW::render_window _window {"DrawPixel Example", WIDTH, HEIGHT};

    const double target_frame = 1.0 / 60.0;
    u32 frame = 0;

    for (int i = 0; i < FIRE_W * FIRE_H; i++)
    {
        fire_pixels[i] = 0;
    }

    for(int i = 0; i < FIRE_W; i++)
    {
        fire_pixels[(FIRE_H - 1) * FIRE_W + i] = FIRE_PALETTE_SIZE - 1;
    }
    
    while(!_window.should_close())
    {
        u64 start = _window.get_counter();

        _window.clear(0x000000);

        for (u32 x = 0; x < FIRE_W; x++)
        {
            for (u32 y = 1; y < FIRE_H; y++)
            {
                u32 src = FIRE_W * y + x;
                u32 rand_idx = rand() % 4;
                i32 dst = (i32)src - FIRE_W;
                i32 dst_x = (i32)(src % FIRE_W) - (i32)rand_idx + 1;
                if (dst < 0 || dst_x < 0 || dst_x >= FIRE_W)
                {
                    continue;
                }
                else
                {
                    dst = dst - (i32)(src % FIRE_W) + dst_x;
                    uint8_t src_val = fire_pixels[src];
                    uint8_t decay = rand_idx & 1;
                    fire_pixels[dst] = (src_val > decay) ? (src_val - decay) : 0;
                }                
            }
        }

        for (u32 y = 0; y < FIRE_H; y++)
        {
            for (u32 x = 0; x < FIRE_W; x++)
            {
                uint8_t idx = fire_pixels[y * FIRE_W + x];
                _window.put_pixel(x, y, fire_palette[idx]);
            }
        }
        
        _window.display();

        u64 end = _window.get_counter();

        double elapsed = (double)(end - start) / (double)_window.get_frequency();

        if (elapsed < target_frame)
        {
            _window.delay((target_frame - elapsed) * 1000.0);
        }

        frame++;
    }

}