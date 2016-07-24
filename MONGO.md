##[MongoDb](https://www.mongodb.com)

The mongo-c driver needs a service to connect to.

* Install on OSX:  
```bash
brew install mongodb
```

* Install on Ubuntu / Linux:  
```bash
sudo apt-get install mongodb-org
```

##Startup MongoDb

```bash
mkdir ./mongodb_data/
mongod --dbpath ./mongodb_data/
```