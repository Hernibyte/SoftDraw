#include "core/window.h"

#include <algorithm>
#include <cmath>
#include <vector>

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
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STREAMING, 
            m_window_props.m_width, 
            m_window_props.m_height
        );

        SDL_SetTextureScaleMode(m_window_props.m_frame_texture, SDL_SCALEMODE_NEAREST);
        SDL_SetTextureBlendMode(m_window_props.m_frame_texture, SDL_BLENDMODE_BLEND);
    }

    render_window::~render_window()
    {
        SDL_DestroyTexture(m_window_props.m_frame_texture);
        SDL_DestroyRenderer(m_window_props.m_renderer);
        SDL_DestroyWindow(m_window_props.m_window);

        SDL_Quit();
    }

    void render_window::put_pixel(const u32 x, const u32 y, const u32 color)
    {
        if (x < 0 || x >= m_window_props.m_width) return;
        if (y < 0 || y >= m_window_props.m_height) return;
        m_window_props.m_framebuffer[m_window_props.m_width * y + x] = color;
    }

    void render_window::put_pixel(const vec_2d<u32> vec2d, const u32 color)
    {
        put_pixel(vec2d.get_x(), vec2d.get_y(), color);
    }

    void render_window::clear(const u32 color)
    {
        for (u32& pixel : m_window_props.m_framebuffer)
        {
            pixel = color;
        }
    }

    void render_window::put_line(vec_2d<f32> start, vec_2d<f32> end, u32 color) {
        const int dx = abs((i32)end.get_x() - (i32)start.get_x());
        const int dy = abs((i32)end.get_y() - (i32)start.get_y());

        const int sx = (start.get_x() < end.get_x()) ? 1 : -1;   // dirección en X
        const int sy = (start.get_y() < end.get_y()) ? 1 : -1;   // dirección en Y

        int err = dx - dy;             // error acumulado

        while (true) {
            put_pixel((i32)start.get_x(), (i32)start.get_y(), color);          // dibujar el pixel actual

            if (start.get_x() == end.get_x() && start.get_y() == end.get_y()) break;

            int e2 = 2 * err;

            if (e2 > -dy) {
                err -= dy;
                start.set_x(start.get_x() + (f32)sx);
            }
            if (e2 < dx) {
                err += dx;
                start.set_y(start.get_y() + (f32)sy);
            }
        }
    }
    
    void render_window::put_outline_triangle(const vec_2d<f32> first_vertex_position, const vec_2d<f32> second_vertex_position, const vec_2d<f32> third_vertex_position, u32 color)
    {
        put_line(first_vertex_position, second_vertex_position, color);
        put_line(second_vertex_position, third_vertex_position, color);
        put_line(third_vertex_position, first_vertex_position, color);
    }
    
    void render_window::put_filled_triangle(const vec_2d<f32> first_vertex_position, const vec_2d<f32> second_vertex_position, const vec_2d<f32> third_vertex_position, u32 color)
    {
        f32 x0 = first_vertex_position.get_x();
        f32 y0 = first_vertex_position.get_y();
        
        f32 x1 = second_vertex_position.get_x();
        f32 y1 = second_vertex_position.get_y();
        
        f32 x2 = third_vertex_position.get_x();
        f32 y2 = third_vertex_position.get_y();
        
        if (y0 > y1) { std::swap(x0, x1); std::swap(y0, y1); }
        if (y0 > y2) { std::swap(x0, x2); std::swap(y0, y2); }
        if (y1 > y2) { std::swap(x1, x2); std::swap(y1, y2); }

        auto fill_horizontal = [&](const int y, const float x_left, const float x_right)
        {
            const int start = static_cast<int>(std::ceil(x_left));
            const int end   = static_cast<int>(std::floor(x_right));
            for (int x = start; x <= end; ++x)
            {
                put_pixel(x, y, color);
            }
        };

        if (y1 > y0)
        {
            const float inv_slope1 = (x1 - x0) / (y1 - y0);
            const float inv_slope2 = (x2 - x0) / (y2 - y0);

            float x_left  = x0;
            float x_right = x0;

            for (int y = static_cast<int>(y0); y <= static_cast<int>(y1); ++y)
            {
                fill_horizontal(y, std::min(x_left, x_right), std::max(x_left, x_right));
                x_left  += inv_slope1;
                x_right += inv_slope2;
            }
        }

        if (y2 > y1)
        {
            const float inv_slope1 = (x2 - x1) / (y2 - y1);
            const float inv_slope2 = (x2 - x0) / (y2 - y0);

            float x_left  = x1;
            float x_right = x0 + inv_slope2 * (y1 - y0);

            for (int y = static_cast<int>(y1) + 1; y <= static_cast<int>(y2); ++y)
            {
                fill_horizontal(y, std::min(x_left, x_right), std::max(x_left, x_right));
                x_left  += inv_slope1;
                x_right += inv_slope2;
            }
        }

        put_line({x0, y0},{x1, y1}, color);
        put_line({x1, y1},{x2, y2}, color);
        put_line({x2, y2},{x0, y0}, color);
    }

    u64 render_window::get_counter()
    {
        return SDL_GetPerformanceCounter();
    }

    u64 render_window::get_frequency()
    {
        return SDL_GetPerformanceFrequency();
    }

    void render_window::delay(u32 ms_delay)
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

    void render_window::display() const
    {
        SDL_UpdateTexture(
            m_window_props.m_frame_texture,
            NULL,
            m_window_props.m_framebuffer.data(),
            (i32)(m_window_props.m_width * sizeof(u32))
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