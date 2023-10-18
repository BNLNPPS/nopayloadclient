#!/bin/bash

echo "Attempting to prepare the github runner."
read -p "This will install additional software. Continue (y/n)?" CONT
if [ "$CONT" != "y" ]; then
    echo "user did not confirm. exiting...";
    exit 0
fi

echo "Proceeding with the setup..."

echo "check out nopayloaddb and launch container..."
git clone git@github.com:BNLNPPS/nopayloaddb.git
cd nopayloaddb
git pull origin master
docker-compose up --build -d
cd ..

echo "install libcurl..."
sudo apt-get update
sudo apt-get install -y libnghttp2-dev
sudo apt-get install curl
sudo apt-get install libcurl4-openssl-dev

echo "install nopayloadclient..."
cd nopayloadclient
cmake -DBUILD_TESTING=ON -S -DUSE_EXTERNAL_JSON=ON . -B build
cmake --build build/
sudo cmake --install build/
cd ..

echo "create remote payload dir"
mkdir /tmp/remote_pl_store/

echo "test curl..."
curl http://localhost:8000/api/cdb_rest/gt
