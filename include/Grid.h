#ifndef GRID_H
#define GRID_H

#include "common.h"

#define GRID_CELL_CAPACITY 4

struct GridCell {
	uint32_t particles[GRID_CELL_CAPACITY];
	uint32_t len; // could be smaller, but will get the shit alliased out of it anyway
	// could also just set last to -1 maybe? (not unsigned obviously, but anything close to 2**32 will be very unlikely anyway)
};

// WARNING
// this class does only very basic things
// concept of row and collumn is abstracted on top of a 1D array
class Grid {
public:
	const uint32_t width, height, size;
	std::unique_ptr<GridCell[]> cells;

	Grid() = delete;
	Grid(uint32_t width, uint32_t height) : width(width), height(height), size(width * height), cells(std::make_unique<GridCell[]>(width * height)) { }
	~Grid() = default;
};

#endif
