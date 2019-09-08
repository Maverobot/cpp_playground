# conan\_catkin\_example

Reference link: https://git.rwth-aachen.de/tim.uebelhoer/catkin-conan/blob/master/README.md

* Install conan
```
$ pip install --user conan
```

* If you are using GCC compiler >= 5.1, Conan will set the compiler.libcxx to the old ABI for backwards compatibility. You can change this with the following commands:
```
$ conan profile new default --detect  # Generates default profile detecting GCC and sets old ABI
$ conan profile update settings.compiler.libcxx=libstdc++11 default  # Sets libcxx to C++11 ABI

```

* Build ROS workspace with conan
```
catkin_make_isolated --cmake-args -DCMAKE_BUILD_TYPE=Release
```
or
```
catkin build --cmake-args -DCMAKE_BUILD_TYPE=Release
```
