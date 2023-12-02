#ifndef GRID_H
#define GRID_H

#include "common.h"

#define GRID_CELL_CAPACITY 4

struct GridCell {
	GLuint particles[GRID_CELL_CAPACITY];
	GLuint len; // could be smaller, but will get the shit alliased out of it anyway
	// could also just set last to -1 maybe? (not unsigned obviously, but anything close to 2**32 will be very unlikely anyway)
};

// WARNING
// this class does only very basic things
// concept of row and collumn is abstracted on top of a 1D array
class Grid {
public:
	const GLuint rows, cols, size;
	const GLfloat particle_radius, square_size, inverse_square_size; // weird constant to speed up math
	std::unique_ptr<GridCell[]> cells;

	Grid() = delete;
	Grid(GLuint width, GLuint height, GLfloat particle_radius)
	: rows(height / (2 * particle_radius)), cols(width / (2 * particle_radius)), size(rows * cols), particle_radius(particle_radius),
	  square_size(2 * particle_radius), inverse_square_size(1.0f / (2 * particle_radius)), cells(std::make_unique<GridCell[]>(width * height))
	{
		clear();
	}

	~Grid() = default;

	void clear();
	void insert(GLuint particle_id, GLfloat x, GLfloat y);
	GridCell *get(GLuint row, GLuint col);
};

#endif
