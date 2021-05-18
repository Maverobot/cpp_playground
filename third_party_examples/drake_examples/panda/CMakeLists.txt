add_executable(panda_simulation panda_simulation.cpp)
target_link_libraries(panda_simulation PRIVATE drake::drake gflags stdc++fs)
