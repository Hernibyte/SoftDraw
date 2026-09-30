#pragma once

#include "platform/default_types.h"

namespace SOFTDRAW
{
    
    template<typename T, i32 SIZE>
    struct vec
    {
        vec()
        {
            for(int i = 0; i < SIZE - 1; i++)
            {
                arr[i] = T();
            }
        }

        T arr[SIZE];
    };

    template<typename T>
    struct vec_2d
    {
        vec_2d() = default;
        
        vec_2d(const T x, const T y)
        {
            data.arr[0] = x;
            data.arr[1] = y;
        }
        
        [[nodiscard]] T get_x() const { return data.arr[0]; }
        [[nodiscard]] T get_y() const { return data.arr[1]; }
        
        void set_x(const T x) { data.arr[0] = x; }
        void set_y(const T y) { data.arr[1] = y; }
        
        vec<T, 2> data;
    };

}