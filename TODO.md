## Bugfix
- [x] Particles going out of bounds when window size is not 1000x1000. lost like 2h in this, it was the shader, the last place I expected to see this
- [x] Particles are all white when using the current settings in main

## Recorder
- [ ] Put new sandbox dimensions etc into recorder
- [ ] Need to cache things in memory and not keep getting from file somehow. fix
- [ ] Recorder is forgetting either accel or old_pos in the transition. fix

## Rendering
- [ ] Change shader to support height != width

## Simulation
- [ ] Finish multithreading
- [ ] Add SIMD
- [ ] Add a system where an error in adding to grid draws the current simulation, making offender particles red
- [ ] Cleanup after closing window, and prevent setting up the renderer multiple times
- [ ] Make mipmaps(????????????????)
- [ ] Check if transposing the grid would be better for cache locality
- [ ] Redo collisions to get better cache locality
- [ ] Test ridiculous overkill compile-timing everything, like the grid size
- [ ] Use different threadpool, it would be better for cache if thread N always worked on slice N
Think about collision optimizations:
- [ ] Threads only need to wait for the fisrst row of the thread above them
- [ ] Collisions can maybe propagate up and never look below them, meaning update() could be done right after the collisions of each slice