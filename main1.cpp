#include "misc.h"

[[nodiscard]] static uint32_t read_table_line(char* data, char delimiter, char *p_dst, uint32_t limit,
                                              uint32_t *p_readed_count)
{
    uint32_t count = 0;
    uint32_t i = 0;
    for (; i < limit && (data[i] != '\n' && data[i] != 0); ++i)
        if (data[i] != delimiter)
            p_dst[count++] = data[i];
    *p_readed_count = i + 1;
    return count;
}


int main()
{
    String file_data;
    if (not readFileToStr("grammar", &file_data))
        return -1;

    char header[file_data.data.size];
    uint32_t offset;
    uint32_t header_size = read_table_line(file_data.data.begin(), ' ', header, file_data.data.size, &offset);

    char *p_data = &header[header_size];
    char *p_file_data = file_data.data.begin() + offset;
    uint32_t limit = file_data.data.size - offset;


    struct Vertex { char src; char dst; char value; };
    struct State  { char name; bool is_end; };
    mtl::array::smart<State>  states;
    mtl::array::smart<Vertex> vertices;
    if (not states.init(8) || not vertices.init(8))
        return -1;

    while (p_file_data < file_data.data.end())
	{
        uint32_t row_size = read_table_line(p_file_data, ' ', p_data, limit, &offset);
        p_file_data  += offset;
        if (row_size == 0)
            break;

        if (not states.insert({p_data[0], p_data[row_size-1] == '1'}))
            return -1;

        for (uint32_t i = 1; i < row_size - 1; ++i)
            if (not vertices.insert({p_data[0], p_data[i], header[i-1]}))
                return -1;
	}

    file_data.destroy();

    puts("Right-handed grammar");
    for (const auto &state : states)
    {
        uint32_t length = 0;
        header[length++] = state.name;
        for (auto &c : "::=")
            header[length++] = c;
        --length;
        for (uint32_t i = 0; i < vertices.size; ++i)
        {
            const auto &ver = vertices[i];
            if (state.name == ver.src)
            {
                header[length++] = ver.value;
                header[length++] = ver.dst;
                header[length++] = '|';
            }
        }
        header[length-1] = 0;
        puts(header);
	}

    puts("Left-handed grammar");
    for (auto &state : states)
    {
        if (state.is_end)
        {
            uint32_t length = 0;
            header[length++] = state.name;
            for (auto &c : "::=")
                header[length++] = c;
            --length;
            for (uint32_t i = 0; i < vertices.size; ++i)
            {
                const auto &ver = vertices[i];
                if (state.name == ver.dst)
                {
                    header[length++] = ver.src;
                    header[length++] = ver.value;
                    header[length++] = '|';
                }
            }
            header[length-1] = 0;
            puts(header);
		}
    }

    states.destroy();
    vertices.destroy();
    return 0;
}

