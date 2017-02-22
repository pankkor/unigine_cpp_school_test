//
// Unigine C++ School test task
// by griff
//
// Usage:
//      $./freq in.txt out.txt
//
// Algorythm:
//      1. find next token
//      2. get frequency for token from hashmap (hashmap[token])
//      3. put frequency + 1 back in hashmap for token (hashmap[token] += 1)
//      4. repeat steps 1-3 until eof
//      5. sort hashmap key-value pairs in output order
//
// Complexity:
//      Build frequency table - Big O(N), where N is nuber of characters in file
//      Sorted output - Big O(KlogK), where K is number of unique words in frequency table
//

#include <stdio.h>
#include <codecvt>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <algorithm>

#define TOKENIZER_IMPL
#include "tokenizer.h"

#define MMAP_IMPL
#include "mmap.h"

void freq_count(const char *token, size_t len, const char delim, void *user_data);
bool save_to_file(const char *file, const std::unordered_map<std::string, size_t> &map);

int main(int argc, char **argv) {

    if (argc != 3) {
        fprintf(stderr, "Usage : %s [in_file] [out_file]\n", argv[0]);
        return 1;
    }

    size_t len = 0;
    void *data = gr_mmap_file(argv[1], &len);
    if (!data) {
        fprintf(stderr, "mmap failed \"%s\"\n", argv[1]);
        return 1;
    }

    std::unordered_map<std::string, size_t> map;
    gr_tokenize((const char *)data, len, "!?., \n\t", freq_count, &map);
    if (!save_to_file(argv[2], map)) {
        fprintf(stderr, "save failed \"%s\"\n", argv[2]);
    }

    if (!gr_munmap_file(data, len)) {
        fprintf(stderr, "munmmap failed\n");
        return 1;
    }

    return 0;
}

// STL doesn't have good unicode support, this won't work in all cases
std::string tolower_utf8(const std::string &str) {
    std::locale::global(std::locale(""));
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;

    std::wstring wstr = conv.from_bytes(str);
    std::use_facet<std::ctype<wchar_t>>(std::locale()).tolower(&wstr[0], &wstr[0] + wstr.size());
    return conv.to_bytes(wstr);
}

void freq_count(const char *token, size_t len, const char delim, void *user_data) {
    std::unordered_map<std::string, size_t> *map = (std::unordered_map<std::string, size_t> *)user_data;

    std::string token_str = std::string(token, len);
    (*map)[tolower_utf8(token_str)] += 1;
}

bool sort_by_freq(const std::pair<std::string, size_t> &a, const std::pair<std::string, size_t> &b) {
    return ((a.second == b.second) && (a.first < b.first)) || (a.second > b.second);
}

bool save_to_file(const char *file, const std::unordered_map<std::string, size_t> &map) {
    FILE *f = fopen(file, "w");
    if (!f) {
        perror("fopen");
        return false;
    }

    std::vector<std::pair<std::string, size_t>> freqs;
    freqs.reserve(map.size());

    for (auto pair : map) {
        freqs.push_back(pair);
    }

    std::sort(freqs.begin(), freqs.end(), sort_by_freq);

    for (auto pair : freqs) {
        fprintf(f, "%ld %s\n", (long)pair.second, pair.first.c_str());
    }

    if (fclose(f) != 0) {
        perror("fclose");
        return false;
    }

    return true;
}

