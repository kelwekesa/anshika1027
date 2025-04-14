# anshika1027 - simulating MESI cache coherence protocol

### folder structure
```
project
│   README.md
│   .gitignore
|	CMakeLists.txt
|	Makefile  
│
└───include
|      Bbus.h
│      Cache.h
│      InputParser.h
│      Memory.h
│      utility.h   
└───src
|     	Bus.cpp
|     	Cache.cpp
|   	InputParser.cpp
|   	main.cpp
|   	Memory.cpp
|   	utility.cpp
└───traces
   		...
   		...
```

### how to compile
make clean
make
### how to run
cd bin
./L1simulate -t app1 -s 4 -E 4 -b 8 -o logfile.txt    

:scream: please note that the traces **MUST** be placed in a folder called traces in the root folder

## Discussion




