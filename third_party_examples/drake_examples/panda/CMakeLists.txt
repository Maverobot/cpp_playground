add_executable(panda_simulation iiwa_common.cpp iiwa_lcm.cpp kuka_torque_controller.cpp
                                iiwa_simulation.cpp)
target_link_libraries(panda_simulation PRIVATE drake::drake gflags stdc++fs)
