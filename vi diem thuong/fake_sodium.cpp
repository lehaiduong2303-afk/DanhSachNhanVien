#define _CRT_SECURE_NO_WARNINGS
#include "fake_sodium.hpp"
#include <cstring>

// ------------------------------------------------
// fake init
// ------------------------------------------------
int fake_sodium_init(void) {
    return 0; // luôn thành công
}

// ------------------------------------------------
// fake password hash (plain text copy)
// ------------------------------------------------
int crypto_pwhash_str(
    char* out,
    const char* passwd,
    unsigned long long passwdlen,
    unsigned long long,
    size_t
) {
    if (!out || !passwd) return -1;

    // copy plain text (mock)
    std::strncpy(out, passwd, crypto_pwhash_STRBYTES - 1);
    out[crypto_pwhash_STRBYTES - 1] = '\0';

    return 0;
}

// ------------------------------------------------
// fake verify
// ------------------------------------------------
int crypto_pwhash_str_verify(
    const char* hash,
    const char* passwd,
    unsigned long long passwdlen
) {
    if (!hash || !passwd) return -1;
    if (std::strlen(passwd) != passwdlen) return -1;

    return std::strcmp(hash, passwd) == 0 ? 0 : -1;
}
