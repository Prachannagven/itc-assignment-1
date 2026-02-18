# Project Summary
This project implements huffman, shanon-type, shanon and shanon-fano encoding using C. Text may be entered, or a .txt file may be processed through in order to create the encrypted bitstream. The input text is seperated into characters, and then the frequency and relative probability of each is calculated as well as the entropy of the message itself.

From there, the data is pushed into nodes that contain all the data of each symbol, for easy cross-encryption method handling. 

# Running The Project
To run the project, you must have gcc and make installed on your system. These exist by default in linux. You can run the following commands to make the project.
```bash
cd src/
make
./itc
```
You will then be prompted to enter a string. Enter a string (of max 10,000 characters) and then the program will perform all the encoding types for you. The bitstreams will be spit out as well. 

To run it on a .txt file, just add the file as an argument to the main executable, like follows
```
./itc hello.txt
```
Processing will then be done on the .txt file and the outputs returned in stdout.

