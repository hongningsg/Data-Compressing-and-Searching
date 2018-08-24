
output:main.o Static_Huffman_Coding.o
	g++ main.o Static_Huffman_Coding.o -o huffman

main.o: main.cpp
	g++ -c main.cpp

Static_Huffman_Coding.o: Static_Huffman_Coding.cpp Static_Huffman_Coding.h
	g++ -c Static_Huffman_Coding.cpp

clean:
	rm *.o output
