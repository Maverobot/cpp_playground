add_executable(acrobot_lqr acrobot_lqr.cpp)
target_link_libraries(acrobot_lqr PRIVATE drake::drake gflags)

add_executable(acrobot_passive acrobot_passive.cpp)
target_link_libraries(acrobot_passive PRIVATE drake::drake gflags)
