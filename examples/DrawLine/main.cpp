#include "SoftDraw.h"

#define WIDTH 320
#define HEIGHT 200

int main()
{
    SOFTDRAW::render_window _window {"DrawLine Example", WIDTH, HEIGHT};
    
    while(!_window.should_close())
    {
        _window.clear(0x000000);

        _window.put_line(std::make_pair<i32, i32>(300, 50), std::make_pair<i32, i32>(WIDTH/2, HEIGHT/2), 0xFF0000);
        
        _window.display();
    }

}