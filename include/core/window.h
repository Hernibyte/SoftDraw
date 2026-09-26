#pragma once

#include <vector>
#include <utility>

#include "platform/default_types.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace SOFTDRAW
{

    class render_window
    {
    public:
        render_window(c_cstr title, u32 width, u32 height);
        ~render_window();

        void put_pixel(u32 x, u32 y, u32 color);
        void clear(u32 color);

        void put_line(std::pair<i32, i32> start, std::pair<i32, i32> end, u32 color);
        
        u32 get_counter();
        u32 get_frequency();

        void delay(double ms_delay);

        bool should_close() const;
        void display();

    private:
        struct window_props
        {
            SDL_Window* m_window;
            SDL_Renderer* m_renderer;
            SDL_Texture* m_frame_texture;

            c_cstr m_title;
            u32 m_width;
            u32 m_height;

            std::vector<u32> m_framebuffer;
        };
        window_props m_window_props;
    };

}