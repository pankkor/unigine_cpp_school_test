# Unigine C++ School test task

##### Build:
```make.sh``` or ```make.bat```
Tested with MSVC 19.00.24210 on Win 7, clang-800.0.42.1 on macOS Sierra

##### Usage:
```$./freq in.txt out.txt```

##### Algorythm:
1. find next token
2. get frequency for token from hashmap (```hashmap[token]```)
3. put frequency + 1 back in hashmap for token (```hashmap[token] += 1```)
4. repeat steps 1-3 until eof
5. sort hashmap key-value pairs in output order

##### Complexity:
Build frequency table - Big O(N), where N is nuber of characters in file
Sorted output - Big O(KlogK), where K is number of unique words in frequency table

