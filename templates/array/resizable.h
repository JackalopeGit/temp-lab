#pragma once

#include <cstdlib>
#include "../../tools/exception.h"

namespace mtl 
{
    namespace array
    {
        template<typename value_t, typename size_t = size_t>
        struct resizable
        {
            [[nodiscard]] inline bool init(size_t size);
                          inline void init();

            void destroy();

            [[nodiscard]] inline value_t& operator[](size_t i);

            [[nodiscard]] inline const value_t& operator[](size_t i) const;

            [[nodiscard]] inline bool resize(size_t size);

            [[nodiscard]] inline       value_t& last()       { return p_array[size-1]; }
            [[nodiscard]] inline const value_t& last() const { return p_array[size-1]; }


                  value_t* begin()       { return p_array;        }
                  value_t* end  ()       { return p_array + size; }
            const value_t* begin() const { return p_array;        }
            const value_t* end  () const { return p_array + size; }

            value_t *p_array;
            size_t size;

        } __attribute__((packed));

        template<typename value_t, typename size_t>
        inline void resizable<value_t, size_t>::init()
        {
            p_array = nullptr;
            size = 0;
        }

        template<typename value_t, typename size_t> [[nodiscard]] inline
        bool resizable<value_t, size_t>::init(size_t size_)
        {
#ifndef NDEBUG
            if (size_ <= 0)
            {
                error_output("dynamic array init aborted: size <= 0");
                return false;
            }
#endif
            p_array = static_cast<value_t*>(malloc(sizeof(value_t) * size_));
            if (p_array == nullptr)
                return false;

            size = size_;
            return true;
        }

        template<typename value_t, typename size_t>
        void resizable<value_t, size_t>::destroy()
        {
            free(p_array);
        }

        template<typename value_t, typename size_t>
        [[nodiscard]] inline value_t& resizable<value_t, size_t>::operator[](size_t i)
        {
            #ifndef NDEBUG
            if (i >= size)
                exception("dynamic array subscript aborted:", "i >= size");
            #endif
            return p_array[i];
        }

        template<typename value_t, typename size_t>
        [[nodiscard]] inline const value_t& resizable<value_t, size_t>::operator[](size_t i) const
        {
            #ifndef NDEBUG
            if (i >= size)
                exception("dynamic array subscript aborted:", "i >= size");
            #endif
            return p_array[i];
        }

        template<typename value_t, typename size_t> [[nodiscard]] inline
        bool resizable<value_t, size_t>::resize(size_t size_)
        {
            value_t* new_p_array = static_cast<value_t*>(realloc(p_array, sizeof(value_t) * size_));
            if (new_p_array == nullptr && size_ != 0)
                return false;
            p_array = new_p_array;
            size    = size_;
            return true;
        }
    }
}
