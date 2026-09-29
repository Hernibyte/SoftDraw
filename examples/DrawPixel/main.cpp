#include "SoftDraw.h"

#define WIDTH 320
#define HEIGHT 200

int main()
{
    SOFTDRAW::render_window _window {"DrawPixel Example", WIDTH, HEIGHT};
    
    while(!_window.should_close())
    {
        _window.clear({0, 0, 0, 255});

        _window.put_pixel(WIDTH/2, HEIGHT/2, {0, 255, 0, 255} );
        
        _window.display();
    }

}