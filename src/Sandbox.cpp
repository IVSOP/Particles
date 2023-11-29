#include "Sandbox.h"

Sandbox::Sandbox(uint32_t max_particles, uint32_t pixel_width, uint32_t pixel_height, uint32_t particle_radius)
: particles(max_particles), grid(0, 0), current_tick(0), index_tick(0), spawners()
{

}

Sandbox::~Sandbox() {

}