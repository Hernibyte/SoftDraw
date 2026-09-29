#include "SoftDraw.h"

#define WIDTH 320
#define HEIGHT 200

#define FIRE_W WIDTH
#define FIRE_H HEIGHT
#define FIRE_PALETTE_SIZE 37

static uint8_t fire_pixels[FIRE_W * FIRE_H];

static const u32 fire_palette[FIRE_PALETTE_SIZE] = 
{
    0x070707FF, 0x1F0707FF, 0x2F0F07FF, 0x470F07FF, 0x571707FF, 0x671F07FF, 0x771F07FF,
    0x8F2707FF, 0x9F2F07FF, 0xAF3F07FF, 0xBF4707FF, 0xC74707FF, 0xDF4F07FF, 0xDF5707FF,
    0xDF5707FF, 0xD75F07FF, 0xD75F07FF, 0xD7670FFF, 0xCF6F0FFF, 0xCF770FFF, 0xCF7F0FFF,
    0xCF8717FF, 0xC78717FF, 0xC78F17FF, 0xC7971FFF, 0xBF9F1FFF, 0xBF9F1FFF, 0xBFA727FF,
    0xBFA727FF, 0xBFAF2FFF, 0xB7AF2FFF, 0xB7B72FFF, 0xB7B737FF, 0xCFCF6FFF, 0xDFDF9FFF,
    0xEFEFC7FF, 0xFFFFFFFF
};

int main()
{
    SOFTDRAW::render_window _window {"DoomFire Example", WIDTH, HEIGHT};

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

        _window.clear({0, 0, 0, 255});

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
                _window.put_raw_pixel(x, y, fire_palette[idx]);
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