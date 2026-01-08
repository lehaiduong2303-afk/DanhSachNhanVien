#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    // Gi? l?p API c?a libsodium
    int crypto_pwhash_str_verify(
        const char* hash,
        const char* passwd,
        unsigned long long passwdlen
    );
#pragma once

#ifdef __cplusplus
    extern "C" {
#endif

        // -------- constants --------
#define crypto_pwhash_STRBYTES 128
#define crypto_pwhash_OPSLIMIT_INTERACTIVE 1
#define crypto_pwhash_MEMLIMIT_INTERACTIVE 1

// -------- init --------
        int fake_sodium_init(void);

        // -------- password hash --------
        int crypto_pwhash_str(
            char* out,
            const char* passwd,
            unsigned long long passwdlen,
            unsigned long long opslimit,
            size_t memlimit
        );

        int crypto_pwhash_str_verify(
            const char* hash,
            const char* passwd,
            unsigned long long passwdlen
        );

#ifdef __cplusplus
    }
#endif

#ifdef __cplusplus
}
#endif
