#include "SoftDraw.h"

#define WIDTH 320
#define HEIGHT 200

int main()
{
    SOFTDRAW::render_window _window {"DrawLine Example", WIDTH, HEIGHT};
    
    while(!_window.should_close())
    {
        _window.clear(0x000000FF);

        _window.put_line({ 300, 50 }, {WIDTH/2, HEIGHT/2 }, 0xFF0000FF);
        
        _window.display();
    }

}