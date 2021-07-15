#ifndef HASHMAP_H
#define HASHMAP_H

#include <QDebug>
#include <QVector>

#include "lib/mpair.h"
#include "lib/hashcode.h"

template <typename key_t, typename value_t>
/**
 * @brief The HashMap class
 *        All the operations has amortized constant time complexity
 */
class HashMap {

public:
    /**
     * @brief The Cell struct
     *  container for key/value and next/previous pointer
     *  to reduce the overhead of the linkedlist
     */
    struct Cell {
        MPair<key_t, value_t> data;
        Cell* next;
        Cell* previous;

        /**
          * Default constructor
          */
        Cell() = default;

        /**
         * @brief Cell
         *        construct Cell from MPair
         */
        Cell(const MPair<key_t, value_t>& data)
            : data(data),
              next(nullptr),
              previous(nullptr)
        { /* do nothing */ }

        /**
         * @brief Cell
         *        construct Cell from key/value
         */
        Cell(const key_t& key, const value_t& value)
            : data(key, value),
              next(nullptr),
              previous(nullptr)
        { /* do nothing */ }
    };

public:
    /**
     * @brief The iterator class
     *        Internal iterator class to loop over the HashMap
     */
    class iterator:
            public std::iterator<std::input_iterator_tag,
                                 MPair<key_t, value_t>>
    {
    private:
        const HashMap* mp;
        int bucket;
        Cell* cp;

    public:
        /**
          *  Default constructor
          *  it's deleted as it needs a HashMap to iterate on
          */
        iterator() = default;

        /**
         * @brief iterator
         *        construct iterator with prespecified postion
         *        usefull for HashMap internal functions
         */
        iterator(const HashMap* mp, int bucket, Cell* cp)
            : mp(mp), bucket(bucket), cp(cp)
        { /* do nothing */ }

        /**
         * @brief iterator
         *        construct iterator in the begining or the end
         *        of the HashMap
         */
        iterator(const HashMap* mp, bool end)
        {
            this->mp = mp;
            if(end || mp->buckets.size() == 0) {
                bucket = mp->buckets.size();
                cp = nullptr;
            } else {
                bucket = 0;
                cp = mp->buckets[0];
                while (!cp && ++bucket < mp->buckets.size()) {
                    cp = mp->buckets[bucket];
                }
            }
        }

        /**
         * @brief prefix operator ++
         */
        iterator& operator++() {
            cp = cp->next;
            while (!cp && ++bucket < mp->buckets.size()) {
                cp = mp->buckets[bucket];
            }
            return *this;
        }

        /**
         * @brief postfix operator ++
         */
        iterator operator++(int) {
            iterator copy(*this);
            operator++();
            return copy;
        }

        /**
         * @brief operator *
         * @return refernce to the key/value
         */
        MPair<key_t, value_t>& operator*() {
            return cp->data;
        }

        /**
         * @brief operator ->
         * @return pointer to the key/value
         */
        MPair<key_t, value_t>* operator->() {
            return &(cp->data);
        }

        /**
         * @brief operator ==
         * TODO: check for version updates
         */
        bool operator ==(const iterator& it) {
            return mp == it.mp && bucket == it.bucket && cp == it.cp;
        }

        /**
         * @brief operator !=
         * TODO: check for version updates
         */
        bool operator !=(const iterator& it) {
            return mp != it.mp || bucket != it.bucket || cp != it.cp;
        }
    };

    /**
     * @brief begin
     * @return iterator to the first element on HashMap
     */
    iterator begin() const {
        return iterator(this, false);
    }

    /**
     * @brief end
     * @return iterator to the element behind the last element
     *         on the HashMap
     */
    iterator end() const {
        return iterator(this, true);
    }

public:
    /**
     * @brief HashMap
     *        Default constructor
     */
    HashMap();

    /**
     * @brief HashMap
     *        copy constructor
     */
    HashMap(const HashMap& src);

    /**
     * @brief HashMap
     *        move constructor
     */
    HashMap(HashMap&& src);

    /**
      * Destructor
      */
    ~HashMap();

    /**
     * @brief size
     * @return number of elements in the map
     */
    int size() const { return num_cells; }

    /**
     * @brief operator =
     *        copy assigment operator
     * @return reference to allow chaining
     */
    HashMap& operator=(const HashMap& src);

    /**
     * @brief operator =
     *        move assigment operator
     * @return reference to allow chaining
     */
    HashMap& operator=(HashMap&& src);

    /**
     * @brief insert
     *        insert new item in the map
     *        if the key is present in the map
     *        it returns and update nothing
     */
    void insert(const MPair<key_t, value_t>& item);

    /**
     * @brief insert
     *        overload insert function for seperated
     *        key and value
     */
    void insert(const key_t& key, const value_t& vaule);

    /**
     * @brief get
     * @return the value for the specfied key
     *         the default value if the key is not present
     */
    const value_t& get(const key_t& key) const;

    /**
     * @brief contains
     * @return true if the key is present in the map
     *         false otherwise
     */
    bool contains(const key_t& key) const;

    /**
     * @brief find
     * @return iterator points to the location for the specfied key
     *         end() if the key is not present
     */
    iterator find(const key_t& key) const;

    /**
     * @brief operator []
     *        same as get() but return reference to the value
     */
    const value_t& operator[](const key_t& key) const;

    /**
     * @brief operator []
     *        if the key is present in the map
     *        it returns a reference to the value
     *        otherwise inserts a new item with the defaut value
     */
    value_t& operator[](const key_t& key);

    /**
     * @brief clear
     *        remove all the elements in the map
     */
    void clear();

    /**
     * @brief remove
     * @param key
     */
    void remove(const key_t& key);

    template <typename T, typename U>
    /**
     * @brief operator <<
     * @param dbg
     * @param mp
     * @return
     */
    friend QDebug operator<<(QDebug dbg, const HashMap<T, U> &mp);

private:
    /**
     * @brief insert_cell
     *        insert_cell in the right buckets
     */
    void insert_cell(Cell* cell);

    /**
     * @brief find_cell
     * @return pointer to the cell with the specfied key
     *         null if the key is not present is the map
     */
    Cell* find_cell(const key_t& key, int index) const;

    /**
     * @brief delete_buckets
     *        delete the elements in the given map
     */
    void delete_buckets(QVector<Cell*>& buckets) const;

    /**
     * @brief deep_copy
     *        deep copy for all the elements in the map
     */
    void deep_copy(const HashMap<key_t, value_t>& src);

    /**
     * @brief rehash
     *        if the load factor is bigger than MAX_LOAD_FACTOR
     *        it doubles the internal vector size and rehash all the elements
     *        to achive the amortized constant complexity
     */
    void rehash();

private:
    QVector<Cell*> buckets;
    int num_cells;

    static constexpr float MAX_LOAD_FACTOR = 0.75;
    static constexpr int   INITIAL_CAPCITY = 16;
};


template <typename key_t, typename value_t>
HashMap<key_t, value_t>::HashMap()
    : buckets(INITIAL_CAPCITY), num_cells(0)
{
    // do nothing
}


template<typename key_t, typename value_t>
HashMap<key_t, value_t>::HashMap(const HashMap &src)
{
    deep_copy(src);
}


template<typename key_t, typename value_t>
HashMap<key_t, value_t>::HashMap(HashMap &&src)
    : buckets(std::move(src.buckets)), num_cells(std::move(src.num_cells))
{
    src.num_cells = 0;
}


template <typename key_t, typename value_t>
void
HashMap<key_t, value_t>::insert(const key_t& key, const value_t& vaule)
{
    rehash();

    int index = hash_code(key) % buckets.size();

    if(buckets[index]) {
        Cell* cp = buckets[index];
        while(cp) {
            if(cp->data.key == key) {
                return;
            }
            cp = cp->next;
        }
        Cell* new_cell = new Cell(key, vaule);
        new_cell->next = buckets[index];
        buckets[index]->previous = new_cell;
        buckets[index] = new_cell;
    }
    else {
        Cell* new_cell = new Cell(key, vaule);
        buckets[index] = new_cell;
    }

    num_cells++;
}


template <typename key_t, typename value_t>
const value_t&
HashMap<key_t, value_t>::get(const key_t& key) const
{
    int index = hash_code(key) % buckets.size();
    Cell * cell = find_cell(key, index);
    if(cell)
        return cell->data.value;
    return std::move(value_t());
}


template<typename key_t, typename value_t>
bool HashMap<key_t, value_t>::contains(const key_t &key) const
{
    int index = hash_code(key) % buckets.size();
    return find_cell(key, index);
}

template<typename key_t, typename value_t>
typename HashMap<key_t, value_t>::iterator
HashMap<key_t, value_t>::find(const key_t &key) const
{
    int index = hash_code(key) % buckets.size();
    Cell * cell = find_cell(key, index);
    if(cell)
        return HashMap<key_t, value_t>::iterator(this, index, cell);
    return end();
}

template<typename key_t, typename value_t>
const value_t&
HashMap<key_t, value_t>::operator[](const key_t &key) const
{
    return get(key);
}

template<typename key_t, typename value_t>
void
HashMap<key_t, value_t>::clear()
{
    delete_buckets(buckets);
    num_cells = 0;
}

template<typename key_t, typename value_t>
void
HashMap<key_t, value_t>::remove(const key_t &key)
{
    int index = hash_code(key) % buckets.size();
    Cell *cp = find_cell(key, index);
    if(!cp)
        return;

    if(cp->previous)
        cp->previous->next = cp->next;
    else
        buckets[index] = cp->next;

    if(cp->next)
        cp->next->previous = cp->previous;

    delete cp;
    num_cells--;
}

template<typename key_t, typename value_t>
typename HashMap<key_t, value_t>::Cell *
HashMap<key_t, value_t>::find_cell(const key_t &key, int index) const
{
    for (Cell* cp = buckets[index]; cp; cp = cp->next) {
        if(cp->data.key == key) {
            return cp;
        }
    }
    return NULL;
}

template<typename key_t, typename value_t>
value_t &
HashMap<key_t, value_t>::operator[](const key_t &key)
{
    int index = hash_code(key) % buckets.size();
    Cell *cp = find_cell(key, index);

    if(!cp) {
        rehash();
        index = hash_code(key) % buckets.size();
        cp = new Cell(key, value_t());
        cp->next = buckets[index];
        if(buckets[index])
            buckets[index]->previous = cp;
        buckets[index] = cp;
        num_cells++;
    }

    return cp->data.value;
}


template<typename key_t, typename value_t>
HashMap<key_t, value_t>::~HashMap()
{
    delete_buckets(buckets);
    num_cells = 0;
}

template<typename key_t, typename value_t>
HashMap<key_t, value_t>&
HashMap<key_t, value_t>::operator=(const HashMap &src)
{
    if (this != &src) {
        clear();
        deep_copy(src);
    }
    return *this;
}

template<typename key_t, typename value_t>
void
HashMap<key_t, value_t>::insert(const MPair<key_t, value_t> &item)
{
    insert(item.key, item.value);
}

template<typename key_t, typename value_t>
HashMap<key_t, value_t>&
HashMap<key_t, value_t>::operator=(HashMap &&src)
{
    num_cells = src.num_cells;
    src.num_cells = 0;
    buckets = std::move(src.buckets);
    return *this;
}

template<typename key_t, typename value_t>
void
HashMap<key_t, value_t>::delete_buckets(QVector<Cell*>& buckets) const
{
    for (int i = 0; i < buckets.size(); i++) {
        Cell* cp = buckets[i];
        while (cp) {
            Cell* temp = cp->next;
            delete cp;
            cp = temp;
        }
        buckets[i] = nullptr;
    }
}

template<typename key_t, typename value_t>
void
HashMap<key_t, value_t>::deep_copy(const HashMap<key_t, value_t>& src)
{
    buckets = QVector<Cell *>(src.buckets.size());
    num_cells = src.num_cells;
    for (int i = 0; i < src.buckets.size(); i++) {
        Cell* last_item = nullptr;
        for (Cell* cp = src.buckets[i]; cp ; cp = cp->next) {
            Cell* new_cell = new Cell(cp->data.key, cp->data.value);

            if (last_item) {
                last_item->next = new_cell;
            } else {
                buckets[i] = new_cell;
            }
            last_item = new_cell;
        }
    }
}


template<typename key_t, typename value_t>
void
HashMap<key_t, value_t>::rehash()
{
    if(num_cells <= MAX_LOAD_FACTOR * buckets.size())
        return;

    Cell *cp, *temp;

    int new_size = buckets.size() * 2;
    QVector<Cell*> old_buckets = std::move(buckets);
    buckets = QVector<Cell *>(new_size);

    for(int i = 0; i < old_buckets.size(); i++) {
        cp = old_buckets[i];
        while(cp) {
            temp = cp->next;
            insert_cell(cp);
            cp = temp;
        }
    }
}

template<typename key_t, typename value_t>
void
HashMap<key_t, value_t>::insert_cell(HashMap::Cell *cell)
{
    int index = hash_code(cell->data.key) % buckets.size();
    if(buckets[index]) {
        cell->next = buckets[index];
        buckets[index]->previous = cell;
        buckets[index] = cell;
    }
    else {
        cell->next = nullptr;
        buckets[index] = cell;
    }
    cell->previous = nullptr;
}


template <typename key_t, typename value_t>
QDebug
operator<<(QDebug dbg, const HashMap<key_t, value_t> &mp)
{
    dbg.nospace() << "HashMap at address:" << &mp  << "\n";
    dbg.nospace() << "No. of Cells: " << mp.num_cells << "\n";
    dbg.nospace() << "No. of Buckets: " << mp.buckets.size() << "\n";
    dbg.nospace() << "Load factor: " << mp.num_cells / (float) mp.buckets.size() << "\n";

    for(int i = 0; i < mp.buckets.size(); i++) {
        dbg.nospace() << i << ":";
        auto cp = mp.buckets[i];
        while(cp != nullptr) {
            dbg.nospace() << " { "
                          << cp->data.key
                          << " : "
                          << cp->data.value
                          << " }";
            cp = cp->next;
        }
        dbg.nospace() << "\n";
    }
    dbg.nospace() << "\n";
    return dbg.nospace();
}


#endif // HASHMAP_H
