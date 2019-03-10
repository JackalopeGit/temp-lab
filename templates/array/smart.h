#pragma once

#include "resizable.h"

namespace mtl
{
    namespace array
    {
        template<typename value_t, typename size_t = size_t>
        struct smart : resizable<value_t, size_t>
        {
            using resizable<value_t, size_t>::destroy;

            [[nodiscard]] inline bool init(size_t size);

            template<typename callback_data_t>
            inline void destroy(void(*pf_destroy_callback)(value_t, callback_data_t), callback_data_t callback_data);

            [[nodiscard]] inline       value_t& operator[](size_t i);
            [[nodiscard]] inline const value_t& operator[](size_t i) const;

            [[nodiscard]] inline       value_t& last()       { return this->p_array[count-1]; }
            [[nodiscard]] inline const value_t& last() const { return this->p_array[count-1]; }

            [[nodiscard]] inline bool prepare_size  (size_t new_count);

            [[nodiscard]] inline bool insert(/*dummy*/);
            [[nodiscard]] inline bool insert(const value_t &value);
            [[nodiscard]] inline bool insert(const value_t &value, size_t index);

            inline void insert_nocheck(const value_t &value);

            inline void insert_nocheck(const value_t &value, size_t index);

            inline void remove(size_t i);
            inline void remove_by_iterator(value_t* iterator);
            inline void remove_by_value(value_t value);

            inline void cut();

            [[nodiscard]] inline bool cut(size_t new_count);


            template<typename callback_data_t>
            inline void clean(void(*pf_destroy)(value_t, callback_data_t),
                              callback_data_t destroy_data);

            template<typename callback_data_t>
            inline void clean_if(bool(*pf_condition)(value_t),
                                 void(*pf_destroy)(value_t, callback_data_t),
                                 callback_data_t destroy_data);

                 value_t* begin()       { return this->p_array;         }
                 value_t* end  ()       { return this->p_array + count; }
           const value_t* begin() const { return this->p_array;         }
           const value_t* end  () const { return this->p_array + count; }

            size_t count;
        };

        template<typename value_t, typename size_t> [[nodiscard]] inline
        bool smart<value_t, size_t>::init(size_t size_)
        {
            count = 0;
            return resizable<value_t, size_t>::init(size_);
        }

        template<typename value_t, typename size_t> template<typename callback_data_t> inline
        void smart<value_t, size_t>::destroy
        (
                void(*pf_destroy_callback)(value_t, callback_data_t),
                callback_data_t callback_data
        )
        {
            clean(pf_destroy_callback, callback_data);
            destroy();
        }

        template<typename value_t, typename size_t>
        [[nodiscard]] inline value_t& smart<value_t, size_t>::operator[](size_t i)
        {
#ifndef NDEBUG
            if (i >= count)
                exception("dynamic array array subscript aborted:", "i >= m_count");
#endif
            return resizable<value_t, size_t>::operator[](i);
        }

        template<typename value_t, typename size_t>
        [[nodiscard]] inline const value_t& smart<value_t, size_t>::operator[](size_t i) const
        {
#ifndef NDEBUG
            if (i >= count)
                exception("dynamic array array subscript aborted:", "i >= m_count");
#endif
            return resizable<value_t, size_t>::operator[](i);
        }

        template<typename value_t, typename size_t>
        [[nodiscard]] inline bool smart<value_t, size_t>::prepare_size(size_t new_count)
        {
            if (this->size <= new_count)
            {
                if (not this->resize(new_count + count))
                    return false;
            }
            return true;
        }


        template<typename value_t, typename size_t>
        [[nodiscard]] inline bool smart<value_t, size_t>::insert(/**/)
        {
            if (this->size == count)
            {
                if (not this->resize(this->size * 2))
                    return false;
            }
            /*this->p_array[count] = dummy*/;
            ++count;
            return true;
        }
        template<typename value_t, typename size_t> [[nodiscard]] inline
        bool smart<value_t, size_t>::insert(const value_t &value)
        {
            if (this->size == count)
            {
                if (not this->resize(this->size * 2))
                    return false;
            }

            insert_nocheck(value);

            return true;
        }
        template<typename value_t, typename size_t> [[nodiscard]] inline
        bool smart<value_t, size_t>::insert(const value_t &value, size_t index)
        {
            if (this->size == count)
            {
                if (not this->resize(this->size * 2 + 1))
                    return false;
            }

            insert_nocheck(value, index);

            return true;
        }

        template<typename value_t, typename size_t> inline
        void smart<value_t, size_t>::insert_nocheck(const value_t &value)
        {
#ifndef NDEBUG
            if (count == this->size)
                exception("count == size in dynamic array insertion");
#endif
            this->p_array[count] = value;
            ++count;
        }

        template<typename value_t, typename size_t> inline
        void smart<value_t, size_t>::insert_nocheck(const value_t &value, size_t index)
        {
#ifndef NDEBUG
            if (index >= this->size)
                exception("index >= size in dynamic array insertion");
            if (index > count)
                exception("index >= count in dynamic array insertion");
#endif
            for (size_t i = count; i > index; --i)
                this->p_array[i] = this->p_array[i - 1];

            this->p_array[index] = value;
            ++count;
        }

        template<typename value_t, typename size_t>
        inline void smart<value_t, size_t>::remove(size_t i)
        {
            --count;
            for (; i < count; ++i)
                this->p_array[i] = this->p_array[i+1];
        }

        template<typename value_t, typename size_t>
        inline void smart<value_t, size_t>::remove_by_iterator(value_t* iterator)
        {
            --count;
            for(;iterator < end(); ++iterator)
                *iterator = *(iterator + 1);
        }

        template<typename value_t, typename size_t>
        inline void smart<value_t, size_t>::remove_by_value(value_t value)
        {
            for (size_t i = 0; i < count; ++i)
            {
                if (this->p_array[i] == value)
                {
                    --count;
                    for (; i < count; ++i)
                        this->p_array[i] = this->p_array[i+1];
                }
            }
        }

        template<typename value_t, typename size_t> inline
        void smart<value_t, size_t>::cut()
        {
            if (count < this->size)
                this->resize(count);
        }

        template<typename value_t, typename size_t>
        [[nodiscard]] inline bool smart<value_t, size_t>::cut(size_t new_count)
        {
            if (new_count < this->size)
                return this->resize(new_count);
            return true;
        }


        template<typename value_t, typename size_t>
        template<typename callback_data_t>
        inline void smart<value_t, size_t>::clean
        (
                void(*pf_destroy)(value_t, callback_data_t),
                callback_data_t destroy_data
        )
        {
            for (size_t i = 0; i < count; ++i)
                pf_destroy(this->p_array[i], destroy_data);
            count = 0;
        }

        template<typename value_t, typename size_t>
        template<typename callback_data_t>
        inline void smart<value_t, size_t>::clean_if
        (
                bool(*pf_condition)(value_t),
                void(*pf_destroy)(value_t, callback_data_t),
                callback_data_t destroy_data
        )
        {
            size_t new_count = 0;
            for (size_t i = 0; i < count; ++i)
            {
                if (pf_condition(this->p_array[i]))
                    pf_destroy(this->p_array[i], destroy_data);
                else this->p_array[new_count++] = this->p_array[i];
            }
            count = new_count;
        }
    }
}
