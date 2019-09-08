# conan\_cmake\__example
Follow these steps to build the project,
* Install conan
```
$ pip install --user conan
```

* If you are using GCC compiler >= 5.1, Conan will set the compiler.libcxx to the old ABI for backwards compatibility. You can change this with the following commands:
```
$ conan profile new default --detect  # Generates default profile detecting GCC and sets old ABI
$ conan profile update settings.compiler.libcxx=libstdc++11 default  # Sets libcxx to C++11 ABI

```
* Configure conan for this project
```
$ mkdir build && cd build
$ conan install ..
```

* Build project with cmake
```
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```
