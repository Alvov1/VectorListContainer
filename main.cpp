#include <vector>
#include <list>

template<class T>
class VectorList {
private:
    using VectT = std::vector<T>;
    using ListT = std::list<VectT>;
    using ListIter = typename ListT::const_iterator;
    using VectIter = typename VectT::const_iterator;

public:

    VectorList() = default;

    VectorList(VectorList const &) = default;

    VectorList(VectorList &&) noexcept = default;

    VectorList &operator=(VectorList &&) noexcept = default;

    VectorList &operator=(VectorList const &) = default;

    template<class It>
    void append(It p, It q) {
        if (p != q)
            data_.push_back(VectT(p, q));
    }

    bool empty() const { return size() == 0; }

    size_t size() const {
        size_t count = 0;
        for (auto it = data_.begin(); it != data_.end(); ++it)
            count += it->size();
        return count;
    }

    struct const_iterator : std::iterator<std::bidirectional_iterator_tag, const T> {
    public:
        const_iterator() = default;
        const_iterator(const_iterator const &move) = default;
        const_iterator(ListT const *const ptr, VectIter const &vectIter, ListIter const &listIter)
                : ptr(ptr), listIter(listIter), vectIter(vectIter) {}

        const_iterator &operator++() {
            ++vectIter;
            if (vectIter == listIter->end() && std::next(listIter) != ptr->end()) {
                listIter++;
                vectIter = listIter->begin();
            }
            return *this;
        };
        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++*this;
            return temp;
        };
        const_iterator &operator--() {
            if (vectIter != listIter->begin())
                --vectIter;
            else if (listIter != ptr->begin()) {
                listIter--;
                vectIter = std::prev(listIter->end());
            }
            return *this;
        };
        const_iterator operator--(int) {
            const_iterator temp = *this;
            --*this;
            return temp;
        };

        bool operator==(const_iterator const &move) const {
            return move.listIter == listIter && move.vectIter == vectIter;
        };

        bool operator!=(const_iterator const &move) const {
            return !(this->operator==(move));
        };

        const T *operator->() const {
            return &(*vectIter);
        }

        const T &operator*() const {
            return *vectIter;
        }

    private:
        ListIter listIter;
        VectIter vectIter;
        ListT const *ptr;
    };

    const_iterator begin() const {
        ListIter listIter = data_.begin();
        VectIter vectIter = listIter->begin();
        return const_iterator(&data_, vectIter, listIter);
    }
    const_iterator end() const {
        if (!empty()) {
            ListIter listIter = std::prev(data_.end());
            VectIter vectIter = listIter->end();
            return const_iterator(&data_, vectIter, listIter);
        } else {
            ListIter listIter = data_.begin();
            VectIter vectIter = listIter->begin();
            return const_iterator(&data_, vectIter, listIter);
        }
    }

    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

private:
    ListT data_;
};
