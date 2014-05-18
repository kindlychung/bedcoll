#pragma once

#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include<algorithm>
#include <string>

off_t countlines(std::string fn);
void dump_buffer(unsigned char *buffer, off_t buffer_size);
off_t fileSize(std::string fn);
