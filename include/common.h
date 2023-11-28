#ifndef COMMON_H
#define COMMON_H

#include <GL/glew.h>
#include "ParticleStructs.h"
#include <iostream>

#define print_error(msg) printf("%s, %s, line %d:\n", __FILE__, __func__, __LINE__); perror(msg);

const GLchar *readFromFile(char *filepath);

#endif
