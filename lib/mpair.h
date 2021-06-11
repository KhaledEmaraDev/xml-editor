#ifndef MPAIR_H
#define MPAIR_H

#include <utility>
#include <QPair>
#include <QDebug>

template <typename key_t, typename value_t>
struct MPair {
    const key_t key;
    value_t value;

    MPair() = default;

    MPair(const key_t& key)
        : key(key), value(value_t()) {}

    MPair(const key_t& key, const value_t& value)
        : key(key), value(value) {}

    MPair(const MPair& mp)
        : key(mp.key), value(mp.value) {}

    MPair(MPair&& mp)
        : key(std::move(mp.key)),
          value(std::move(mp.value)) {}

    MPair(std::pair<key_t, value_t>&& sp)
        : key(std::move(sp.first)),
          value(std::move(sp.second)) {}

    MPair(const std::pair<key_t, value_t>& sp)
        : key(std::move(sp.first)),
          value(std::move(sp.second)) {}

    MPair(QPair<key_t, value_t>&& qp)
        : key(std::move(qp.first)),
          value(std::move(qp.second)) {}

    MPair(const QPair<key_t, value_t>& qp)
        : key(std::move(qp.first)),
          value(std::move(qp.second)) {}

    MPair& operator=(const MPair& mp)
    {
        key = mp.key;
        value = mp.value;
    }

    MPair& operator=(MPair&& mp)
    {
        key = std::move(mp.key);
        value = std::move(mp.value);
    }

    bool operator==(const MPair& mp)
    {
        return key == mp.key && value == mp.value;
    }

    bool operator!=(const MPair& mp)
    {
        return key != mp.key || value != mp.value;
    }

    bool operator<(const MPair& mp)
    {
        if(key == mp.key)
            return value < mp.value;
        return key < mp.key;
    }

    bool operator<=(const MPair& mp)
    {
        return key <= mp.key;
    }

    bool operator>(const MPair& mp)
    {
        if(key == mp.key)
            return value > mp.value;
        return key > mp.key;;
    }

    bool operator>=(const MPair& mp)
    {
        return key >= mp.key;
    }

    void swap(const MPair& mp)
    {
        std::swap(key, mp.key);
        std::swap(value, mp.value);
    }
};

template <typename key_t, typename value_t>
MPair<key_t, value_t>
make_mpair(const key_t& key, const value_t& value)
{
    return MPair<key_t, value_t>(key, value);
}

template <typename key_t, typename value_t>
QDebug
operator<<(QDebug dbg, const MPair<key_t, value_t> &mp)
{
    dbg.nospace() << "{ " << mp.key
                  << " , " << mp.value
                  << " }";
    return dbg.nospace();
}

#endif // MPAIR_H
