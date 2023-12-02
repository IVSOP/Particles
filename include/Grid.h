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
	const GLuint width, height, size;
	std::unique_ptr<GridCell[]> cells;

	Grid() = delete;
	Grid(GLuint width, GLuint height) : width(width), height(height), size(width * height), cells(std::make_unique<GridCell[]>(width * height)) { }
	~Grid() = default;

	void clear();
	void insert(GLuint particle_id, GLfloat x, GLfloat y);
};

#endif
