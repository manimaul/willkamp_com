#HTTP micro-service / REST API with NoSQL data store in CPP14

This is an http micro-service in CPP14 using [Libmicrohttpd](https://www.gnu.org/software/libmicrohttpd/) and
[Mongo-C-Driver](https://github.com/mongodb/mongo-c-driver).

See [build dependencies](BUILD_DEP.md) and [mongo-db service depencency](MONGO.md) for basic setup instructions / dependency list.

See [api](API.md)

##Build and Run

Since I'm new to cmake this is a reminder on how to build. From the project root directory, issue the following commands:
```bash
cmake .  
make
./willkamp
```