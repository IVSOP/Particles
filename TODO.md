## Recorder
- [ ] Put new sandbox dimensions etc into recorder
- [ ] Need to cache things in memory and not keep getting from file somehow. fix
- [ ] Recorder is forgetting either accel or old_pos in the transition. fix

## Simulation
- [x] Add multithreading. Use threadpool or locks and conditions and elbow grease?
- [ ] Add SIMD
- [ ] Add a system where an error in adding to grid draws the current simulation, making offender particles red
- [ ] Cleanup after closing window, and prevent setting up the renderer multiple times
- [ ] Make mipmaps(????????????????)
- [ ] Check if transposing the grid would be better for cache locality
- [ ] Redo collisions to get better cache locality
- [ ] Test ridiculous overkill compile-timing everything, like the grid size
- [ ] Use different threadpool, I hate futures and only need a simple join. Also it would be better for cache if thread N always worked on slice N
Think about collision optimizations:
- [ ] Threads only need to wait for the fisrst row of the thread above them
- [ ] Collisions can maybe propagate up and never look below them, meaning update() could be done right after the collisions of each slice