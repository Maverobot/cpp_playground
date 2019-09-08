# conan\_example
Follow these steps to build the project,
* Install conan
```
pip install --user conan
```

* Configure conan for this project
```
mkdir build && cd build
conan install ..
```

* Build project with cmake
```
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```
