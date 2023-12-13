#include "Grid.h"

#define GRID_DEBUG

void Grid::clear() {
	GLuint i;
	for (i = 0; i < size; i++) {
		cells[i].len = 0;
	}
}

// given x and y, inserts particle in the respective position
// however, this position is in a 1D array
void Grid::insert(GLuint particle_id, GLfloat x, GLfloat y) {
	// equal to <coord> / size_of_square
	const GLuint row = static_cast<GLuint>(y * inverse_square_size);
	const GLuint col = static_cast<GLuint>(x * inverse_square_size);

	// printf("asked to insert %f %f, converted to row %u col %u\n", x, y, row, col);
	
	const GLuint index = (row * cols) + col;

	GridCell &cell = cells[index];

#ifdef GRID_DEBUG
	if (cell.len == GRID_CELL_CAPACITY) {
		float avg_len = 0;
		fprintf(stderr, "Too many particles in (%f, %f) -> %u. Printing grid\n", x, y, index);
		for (GLuint row2 = 0; row2 < rows; row2 ++) {
			fprintf(stderr, "\nrow %u----------------------------------\n", row2);
			const GLuint index2 = (row * cols) + col;
			for (GLuint col2 = 0; col2 < cols; col2++) {
				if (col2 == col && row2 == row) {
					fprintf(stderr, KRED "[%u]:%u|" KNRM, col2, cells[index2].len);
				} else {
					fprintf(stderr, "[%u]:%u|", col2, cells[index2].len);
				}
				avg_len += cells[index2].len;
			}	
		}
		fprintf(stderr, "\nAvg len is %f\nTotal len is %d\n", avg_len / size, static_cast<int>(avg_len));
		exit(5);
	}
#endif

	cell.particles[cell.len] = particle_id;
	cell.len ++;
}

GridCell *Grid::get(GLuint row, GLuint col) {
	if (row >= rows || col >= cols) return nullptr;
	// else if (row < 0 || col < 0) return nullptr; not needed, would overflow anyway and get a huge value

	return &cells[(row * cols) + col];
}
