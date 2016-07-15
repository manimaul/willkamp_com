##An HTTP micro-service / REST API with NoSQL data store in CPP14

####Library Dependencies


* CMake

https://cmake.org/

OSX:

`brew install cmake`

Ubuntu / Linux:

`sudo apt-get install cmake build-essential`


* Clang (optional)

http://clang.llvm.org/

Ubuntu / Linux:

`$ sudo apt-get install clang`

`$ sudo update-alternatives --config c++` <choose clang>


* MongoDb C Driver

https://github.com/mongodb/mongo-c-driver
http://api.mongodb.com/c/current/

OSX:

`$ brew install mongo-c`

Ubuntu / Linux:

`$ sudo apt-get install libmongoc-dev libbson-dev`


* Libmicrohttpd

https://www.gnu.org/software/libmicrohttpd/

OSX:

`$ brew install microhttpd`

Ubuntu / Linux:

`$ sudo apt-get install libmicrohttpd-dev`

Ubuntu / Linux:

`$ sudo apt-get install cmake build-essential clang-3.8 clang libmongoc-dev libbson-dev libmicrohttpd-dev `


####Build

`$ cmake .`

`$ make`