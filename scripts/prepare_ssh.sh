#!/bin/bash

echo "Attempting to prepare ssh."
read -p "This will fiddle with the ssh config. Continue (y/n)?" CONT
if [ "$CONT" != "y" ]; then
    echo "user did not confirm. exiting...";
    exit 0
fi

DEPLOY_KEY=$1

mkdir -p ~/.ssh
ssh-keyscan github.com >> ~/.ssh/known_hosts
ssh-agent -a $SSH_AUTH_SOCK > /dev/null
ssh-add - <<< $DEPLOY_KEY
