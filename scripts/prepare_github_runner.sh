#!/bin/bash

echo "Attempting to prepare the github runner."
read -p "This will install additional software. Continue (y/n)?" CONT
if [ "$CONT" != "y" ]; then
    echo "user did not confirm. exiting...";
    exit 0
fi

echo "Proceeding with the setup..."

echo "install libcurl..."
sudo apt-get update
sudo apt-get install -y libnghttp2-dev
sudo apt-get install curl
sudo apt-get install libcurl4-openssl-dev

echo "install nopayloadclient..."
cd nopayloadclient
cmake -DBUILD_TESTING=ON -S . -B build
cmake --build build/
cmake --install build/
cd ..

echo "check out nopayloaddb and launch container..."
git clone git@github.com:BNLNPPS/nopayloaddb.git
cd nopayloaddb
git pull origin master
sed -i 's/FROM python:3/FROM python:3.7.5/g' Dockerfile
docker-compose up --build -d
sleep 30
cd ..

echo "test curl..."
curl http://localhost:8000/api/cdb_rest/gt

echo "create remote payload dir"
mkdir /tmp/remote_pl_store/
