# Cluster
I created a cluster for the Network Laboratory course at the Instituto Balseiro
To run this cluster:

## First, compile:
g++ -g -Wall \
	Master.cpp \
	myconnect.cpp \
	protocolo.cpp \
	Class_Client2M.cpp \
	Class_Server2M.cpp \
	-o Master

g++ -g -Wall \
	Server.cpp \
	Server_DoJob.cpp \
	Server_RunJob.cpp \
	Server_Interactive.cpp \
	myconnect.cpp \
	protocolo.cpp \
	-o Server
	
g++ -g -Wall \
	Cliente.cpp \
	Cliente_RunInteractive.cpp \
	myconnect.cpp \
	protocolo.cpp \
	-o Cliente

 g++ -g -Wall \
	Cliente_Interactivo.cpp \
	Cliente_RunInteractive.cpp \
	myconnect.cpp \
	protocolo.cpp \
	-o Cliente_Interactivo

## Then, run:
./Master

./Server -p 9090 -i 127.0.0.1

./Cliente -p 8080 -i 127.0.0.1 -c SUBMIT -a 5 -e ./ejecutar.sh -d /home/jortz/Documents/IB/Labo6/TrabajoFinal

./Cliente -p 8080 -i 127.0.0.1 -c STATE -j 1

./Cliente -p 8080 -i 127.0.0.1 -c DELETE -j 1

./Cliente_Interactivo -p 8080 -i 127.0.0.1 
