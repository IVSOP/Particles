## Recorder
- [ ] Put new sandbox dimensions etc into recorder
- [ ] Need to cache things in memory and not keep getting from file somehow. fix
- [ ] Recorder is forgetting either accel or old_pos in the transition. fix

## Simulation
- [ ] Add multithreading. Use threadpool or locks and colnditions and elbow grease?
- [ ] Add SIMD
- [ ] Add a system where an error in adding to grid draws the current simulation, making offender particles red
- [ ] Cleanup after closing window, and prevent setting up the renderer multiple times
- [ ] Make mipmaps(????????????????)
- [ ] Check if transposing the grid would be better for cache locality
- [ ] Redo collisions to get better cache locality
