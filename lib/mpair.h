#ifndef MPAIR_H
#define MPAIR_H

#include <utility>
#include <QPair>
#include <QDebug>

template <typename key_t, typename value_t>
/**
 * @brief The MPair struct
 *        Map pair is a key/value pair used in HashMap
 *  All the operations has complexity of O(sizeof(key) + sizeof(value))
 */
struct MPair {
    const key_t key;
    value_t value;

    /**
     * @brief MPair
     *        default constructor
     */
    MPair() = default;

    /**
     * @brief MPair
     *        construct MPair from key and the defaut value
     */
    MPair(const key_t& key)
        : key(key), value(value_t()) {}

    /**
     * @brief MPair
     *        construct MPair from key and value
     */
    MPair(const key_t& key, const value_t& value)
        : key(key), value(value) {}

    /**
     * @brief MPair
     *        copy constructor
     */
    MPair(const MPair& mp)
        : key(mp.key), value(mp.value) {}

    /**
     * @brief MPair
     *        move constructor
     */
    MPair(MPair&& mp)
        : key(std::move(mp.key)),
          value(std::move(mp.value)) {}

    /**
     * @brief MPair
     *        copy constructor from std::pair
     */
    MPair(const std::pair<key_t, value_t>& sp)
        : key(sp.first), value(sp.second) {}

    /**
     * @brief MPair
     *        move constructor from std::pair
     */
    MPair(std::pair<key_t, value_t>&& sp)
        : key(std::move(sp.first)),
          value(std::move(sp.second)) {}

    /**
     * @brief MPair
     *        copy constructor from QPair
     */
    MPair(const QPair<key_t, value_t>& qp)
        : key(qp.first), value(qp.second) {}

    /**
     * @brief MPair
     *        move constructor from QPair
     */
    MPair(QPair<key_t, value_t>&& qp)
        : key(std::move(qp.first)),
          value(std::move(qp.second)) {}

    /**
     * @brief operator =
     *        assignment operator
     * @return reference to allow chaning
     */
    MPair& operator=(const MPair& mp)
    {
        key = mp.key;
        value = mp.value;
        return *this;
    }

    /**
     * @brief operator =
     *        move assignment operator
     * @return reference to allow chaning
     */
    MPair& operator=(MPair&& mp)
    {
        key = std::move(mp.key);
        value = std::move(mp.value);
        return *this;
    }

    /**
     * @brief operator ==
     */
    bool operator==(const MPair& mp)
    {
        return key == mp.key && value == mp.value;
    }

    /**
     * @brief operator !=
     */
    bool operator!=(const MPair& mp)
    {
        return key != mp.key || value != mp.value;
    }

    /**
     * @brief operator <
     */
    bool operator<(const MPair& mp)
    {
        if(key == mp.key)
            return value < mp.value;
        return key < mp.key;
    }

    /**
     * @brief operator <=
     */
    bool operator<=(const MPair& mp)
    {
        return key <= mp.key;
    }

    /**
     * @brief operator >
     */
    bool operator>(const MPair& mp)
    {
        if(key == mp.key)
            return value > mp.value;
        return key > mp.key;;
    }

    /**
     * @brief operator >=
     */
    bool operator>=(const MPair& mp)
    {
        return key >= mp.key;
    }

    /**
     * @brief swap
     */
    void swap(const MPair& mp)
    {
        std::swap(key, mp.key);
        std::swap(value, mp.value);
    }
};

template <typename key_t, typename value_t>
/**
 * @brief make_mpair
 * @param key
 * @param value
 * @return
 */
MPair<key_t, value_t>
make_mpair(const key_t& key, const value_t& value)
{
    return MPair<key_t, value_t>(key, value);
}

template <typename key_t, typename value_t>
/**
 * @brief operator <<
 *        overload QDebug to enable debuging of MPair
 * @return
 */
QDebug
operator<<(QDebug dbg, const MPair<key_t, value_t> &mp)
{
    dbg.nospace() << "{ " << mp.key
                  << " , " << mp.value
                  << " }";
    return dbg.nospace();
}

#endif // MPAIR_H
