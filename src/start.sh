#!/bin/bash

cd Server/Main_Server

echo <password> | sudo -S pkill -9 python


pwd
ls
echo <password> |sudo gnome-terminal -- python2 Main.py

cd ../../Client

pwd
ls
gnome-terminal -- python2 client.py

