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