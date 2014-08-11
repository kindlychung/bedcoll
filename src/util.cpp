#include "util.h"

off_t fileSize(std::string fn)
{
    struct stat fsStatBuf;
    stat(fn.c_str(), &fsStatBuf);
    off_t fileLen = fsStatBuf.st_size;
    return fileLen;
}
void dump_buffer(unsigned char *buffer, off_t buffer_size)
{
    off_t i;
    for(i = 0;i < buffer_size; ++i) { printf("%02x \n", (int)buffer[i]); }
    printf("\n");
}




off_t countlines(std::string fn)
{
    std::ifstream aFile(fn);
    off_t lines_count =0;
    std::string line;
    while (std::getline(aFile , line))
        ++lines_count;
    return lines_count;
}

