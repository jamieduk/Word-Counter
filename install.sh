#!/bin/bash
# (c) J~Net 2024
#
# ./install.sh
#
function x(){
sudo chmod +x $1
}

echo "Installing..."

sudo cp ./word-counter /usr/local/bin/
x /usr/local/bin/word-counter
