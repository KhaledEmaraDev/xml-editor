#ifndef HASHCODE_H
#define HASHCODE_H

#include <type_traits>
#include <string>
#include <QDebug>
#include <QString>

static const size_t multiplier = 263;
static const size_t prime = 1000000007;
static const size_t a = 34;
static const size_t b = 2;

template <class key_t,
          std::enable_if_t<std::is_integral<key_t>::value, int> = 0>
size_t hash_code(key_t key) {
    size_t hash = (a * key + b) % prime;
    return hash;
}

template <class key_t,
          std::enable_if_t<std::is_same<key_t, QString>::value ||
                           std::is_same<key_t, std::string>::value ||
                           std::is_same<key_t, const char *>::value, int> = 0>
size_t hash_code(key_t key) {
    int length = 0;
    if constexpr(std::is_same<key_t, const char *>::value)
        length = sizeof(key);
    else
        length = key.size();

    size_t hash = 0;
    for (int i =length - 1; i >= 0; --i)
        if constexpr(std::is_same<key_t, QString>::value)
            hash = (hash * multiplier + key[i].unicode()) % prime;
        else
            hash = (hash * multiplier + key[i]) % prime;
    return hash;
}

#endif // HASHCODE_H
