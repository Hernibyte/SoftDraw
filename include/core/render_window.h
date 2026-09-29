#pragma once

#include <vector>
#include <utility>

#include "data_structures/vec.h"
#include "data_structures/color.h"
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

        void put_raw_pixel(const u32 x, const u32 y, const u32 hex_color);
        void put_pixel(const u32 x, const u32 y, const u32_color color);
        void put_pixel(const vec_2d<u32> vec2d, const u32_color color);
        void clear(const u32_color color);

        void put_line(vec_2d<f32> start, vec_2d<f32> end, u32_color color);

        void put_outline_triangle(const vec_2d<f32> first_vertex_position, const vec_2d<f32> second_vertex_position, const vec_2d<f32> third_vertex_position, u32_color color);
        void put_filled_triangle(const vec_2d<f32> first_vertex_position, const vec_2d<f32> second_vertex_position, const vec_2d<f32> third_vertex_position, u32_color color);

        u64 get_counter();
        u64 get_frequency();

        void delay(u32 ms_delay);

        bool should_close() const;
        void display() const;

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