# anshika1027 - simulating MESI cache coherence protocol

### folder structure
```
project
│	README.md
│	.gitignore
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
`$ make clean`  
`$ make`    
### how to run
`$ cd bin`    
`$ ./L1simulate -t app1 -s 6 -E 2 -b 5 -o logfile.txt`

:scream: please note that the traces **MUST** be placed in a folder called traces in the root folder as shown above   

### Discussion

The MESI protocol is an invalidate-based cache coherence protocol    

Write-back caching is a technique where data is first written to a cache,      
and the actual write to the backing store (like main memory or disk) is delayed until a later time

### Default parameters for experiment

block bits 5 (32 byte block)    
4KB 2-way set associative cache per core     
a single set is therefore 2KB / 32 = 6 bits for the set     


