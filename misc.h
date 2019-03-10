
#include "tools/string.h"
#include "tools/file.h"

[[nodiscard]] inline bool readFileToStr(const char *filename, String *p_dst)
{
    File file; unsigned int length;
    if (not file.open(filename, File::READ_BIT | File::TEXT_BIT))
        goto CANCEL;

    if (not file.position_set_end())
        goto CANCEL_F;

    length = static_cast<unsigned int>(file.position());
    if (not file.position_set(0))
        goto CANCEL_F;

    if (not p_dst->data.init(length))
        return file.close(), false;

    if (not file.read_bytes(p_dst->data.p_array, length))
        goto CANCEL_S;

    return file.close(), true;

    CANCEL_S: p_dst->destroy();
    CANCEL_F: file.close();
    CANCEL:   return printf("%s - file error\n", filename), false;
}
