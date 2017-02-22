//
// Single-file tokenizer
//
// Usage:
// #define TOKENIZER_IMPL
// #include "tokenizer.h"
//
#if !defined(GR_TOKENIZER_H)
#define GR_TOKENIZER_H

#include <assert.h>

// tokenize callback
typedef void(gr_tokenize_callback_t)(const char *token, size_t len, const char delim, void *user_data);

void gr_tokenize(const char *str, size_t len, const char *delims, gr_tokenize_callback_t callback, void *user_data);
bool gr_is_char_in_string(const char r, const char *str);

#endif

#if defined(TOKENIZER_IMPL)

void gr_tokenize(const char *str, size_t len, const char *delims, gr_tokenize_callback_t callback, void *user_data) {
    assert(callback && "gr_tokenize_callback_t is NULL");
    assert(str && "str is NULL");

    const char *l = str;
    const char *r = str;
    size_t token_len = 0;

    while (r < str + len) {
        if (gr_is_char_in_string(*r, delims)) {
            token_len = r - l;
            if (token_len > 0) {
                callback(l, token_len, *r, user_data);
            }
            l = r + 1;
        }
        ++r;
    }
}

bool gr_is_char_in_string(const char r, const char *str) {
    assert(str && "str is NULL");

    for (; *str != '\0'; ++str) {
        if (*str == r) {
            return true;
        }
    }
    return false;
}

#endif
