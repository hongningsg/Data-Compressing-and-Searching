NOTE: there are two implements of header, dynamic and static. Default is static, using 3 bits for record numbers which means maximum frequency of each charactor is 16777216, and can store all 256 deferent charactors. Dynamic way is using dynamic lenth of bits to store numbers, but with one "NULL" indicator to separate charactors, dynamic way can store maximum to 4294967296, but will exceed when the extreme case of both numbers of charactors and numbers of frequencies of all charactors are large.

To change static way to dynamic, change first line in main to:
Static_Huffman_Coding *hf = new Static_Huffman_Coding(1000000, true);

Encode: -e [inputfile] [outputfile]
Using map to count frequencies of different charactors, then place charactors into Huffman tree. Huffman tree is implemented by priority queue. Pop nodes of priority queue with dynamic/static way to write header. Static way is transform frequency number into binary, then divide into high, med and low part then transfer to 3 charactors. Dynamic way is transfer number regarding its magnitude, use a NULL charactor to seperate charactors. Content of output file is by pop all nodes of Huffman tree, place binary numbers together, encode every 8 bits to a charactors, put 0s when last bit not enough for 8 bits then put a number to indicate how many 0s added into last charactor.

Decode: -d [inputfile] [outputfile]
Read header then use nodes and frequencies to rebuild the Huffman tree. Decode by traversal the tree with '0' to left child and '1' to right child. Last charactor indicates how many bits of second last charactor should be ignored.

Search: -s query [encodedfile]
Search encoded file by decode first, then parttern match finding if query string is included in file. If a match found, shift one charactor then search for the rest until end of file.

Memory control:
By default, 1000000(1MB) memory is used to buffer files. A temporary buffer will be used when empty current memory and read next stage to memory.