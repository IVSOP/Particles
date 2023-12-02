#include "Grid.h"

void Grid::clear() {
	GLuint i;
	for (i = 0; i < size; i++) {
		cells[i].len = 0;
	}
}


void Grid::insert(GLuint particle_id, GLfloat x, GLfloat y) {
	print_error("not implemented");
}
