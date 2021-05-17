add_executable(iiwa_simulation iiwa_common.cpp iiwa_lcm.cpp kuka_torque_controller.cpp
                                iiwa_simulation.cpp)
target_link_libraries(iiwa_simulation PRIVATE drake::drake gflags)
