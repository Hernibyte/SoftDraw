#include "core/window.h"

#include <SDL3/SDL.h>

namespace SOFTDRAW
{

    render_window::render_window(c_cstr title, u32 width, u32 height)
    {
        m_window_props.m_title = title;
        m_window_props.m_width = width;
        m_window_props.m_height = height;
        
        m_window_props.m_framebuffer.resize(width * height);
        
        // SDL Initialize
        SDL_Init(SDL_INIT_VIDEO);

        m_window_props.m_window = SDL_CreateWindow(
            title,
            width,
            height,
            0
        );

        m_window_props.m_renderer = SDL_CreateRenderer(
            m_window_props.m_window, 
            0
        );

        m_window_props.m_frame_texture = SDL_CreateTexture(
            m_window_props.m_renderer, 
            SDL_PIXELFORMAT_XRGB8888,
            SDL_TEXTUREACCESS_STREAMING, 
            m_window_props.m_width, 
            m_window_props.m_height
        );

        SDL_SetTextureScaleMode(m_window_props.m_frame_texture, SDL_SCALEMODE_NEAREST);
    }

    render_window::~render_window()
    {
        SDL_DestroyTexture(m_window_props.m_frame_texture);
        SDL_DestroyRenderer(m_window_props.m_renderer);
        SDL_DestroyWindow(m_window_props.m_window);

        SDL_Quit();
    }

    void render_window::put_pixel(u32 x, u32 y, u32 color)
    {
        if (x < 0 || x >= m_window_props.m_width) return;
        if (y < 0 || y >= m_window_props.m_height) return;
        m_window_props.m_framebuffer[m_window_props.m_width * y + x] = color;
    }

    void render_window::clear(u32 color)
    {
        for (u32& pixel : m_window_props.m_framebuffer)
        {
            pixel = color;
        }
    }

    void render_window::put_line(std::pair<i32, i32> start, std::pair<i32, i32> end, u32 color) {
        int dx = abs(end.first - start.first);
        int dy = abs(end.second - start.second);

        int sx = (start.first < end.first) ? 1 : -1;   // dirección en X
        int sy = (start.second < end.second) ? 1 : -1;   // dirección en Y

        int err = dx - dy;             // error acumulado

        while (true) {
            put_pixel(start.first, start.second, color);          // dibujar el pixel actual

            if (start.first == end.first && start.second == end.second) break;

            int e2 = 2 * err;

            if (e2 > -dy) {
                err -= dy;
                start.first += sx;
            }
            if (e2 < dx) {
                err += dx;
                start.second += sy;
            }
        }
    }
    
    void render_window::put_triangle(std::pair<i32, i32> first_vertex_position, std::pair<i32, i32> second_vertex_position, std::pair<i32, i32> third_vertex_position)
    {
        put_line(first_vertex_position, second_vertex_position, 0xFF0000);
        put_line(second_vertex_position, third_vertex_position, 0xFF0000);
        put_line(third_vertex_position, first_vertex_position, 0xFF0000);
    }

    u32 render_window::get_counter()
    {
        return SDL_GetPerformanceCounter();
    }

    u32 render_window::get_frequency()
    {
        return SDL_GetPerformanceFrequency();
    }

    void render_window::delay(double ms_delay)
    {
        SDL_Delay(ms_delay);
    }

    bool render_window::should_close() const
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                return true;
            }
        }
        
        return false;
    }

    void render_window::display()
    {
        SDL_UpdateTexture(
            m_window_props.m_frame_texture,
            NULL,
            m_window_props.m_framebuffer.data(),
            m_window_props.m_width * sizeof(u32)
        );

        SDL_RenderClear(m_window_props.m_renderer);
        SDL_RenderTexture(
            m_window_props.m_renderer, 
            m_window_props.m_frame_texture, 
            0,
            0
        );
        SDL_RenderPresent(m_window_props.m_renderer);
    }

}