.PHONY: clean

CILENTTARGET='./build/client.out'
MASTERTARGET='./build/master.out'
all: 
	make master
	make client   
master:
	g++ -g -std=c++11  ./Master/*.cpp ./src/*.cpp -o $(MASTERTARGET) -I ./include
client:
	g++ -g -std=c++11  ./Client/*.cpp ./src/*.cpp -o $(CILENTTARGET) -I ./include 
clean:
	rm -rf $(CLIENTTARGET)
	rm -rf $(MASTERTARGET)
