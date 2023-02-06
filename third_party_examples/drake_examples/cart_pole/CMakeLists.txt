add_executable(cart_pole_passive_simulation cart_pole_passive_simulation.cpp)
target_link_libraries(cart_pole_passive_simulation PRIVATE drake::drake gflags)

add_executable(cart_pole_lqr cart_pole_lqr.cpp)
target_link_libraries(cart_pole_lqr PRIVATE drake::drake gflags stdc++fs)
