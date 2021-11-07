#! /bin/bash
# 11/01/2021

# config gcc version
module load gcc/10.1.0

# build server and client
g++ tcpServer.cpp -o server -L./SFML-2.5.1/lib -lsfml-network -lsfml-system -I./SFML-2.5.1/include
g++ tcpClient.cpp -o client -L./SFML-2.5.1/lib -lsfml-network -lsfml-system -I./SFML-2.5.1/include

# add executed
chmod +x server.sh client.sh
