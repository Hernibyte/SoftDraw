#include "SoftDraw.h"

#define WIDTH 320
#define HEIGHT 200

int main()
{
    SOFTDRAW::render_window _window {"DrawPixelMovement Example", WIDTH, HEIGHT};

    const double target_frame = 1.0 / 60.0;
    u32 frame = 0;
    
    while(!_window.should_close())
    {
        u64 start = _window.get_counter();

        _window.clear(0x000000FF);
        int x = frame % WIDTH;
        int y = HEIGHT/2;
        _window.put_pixel(x, y, 0x00FF00FF);
        
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