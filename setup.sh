# !bin/bash

sudo apt-get update -y

sudo apt-get install python2 python-dev mysql-server default-libmysqlclient-dev gcc curl -y

curl https://bootstrap.pypa.io/pip/2.7/get-pip.py --output get-pip.py

sudo python2 get-pip.py

sudo rm get-pip.py

sudo pip2 install mysqlclient