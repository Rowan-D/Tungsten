// 0 is null, 1 is first element

#ifndef TUNGSTEN_UTILS_SLOT_LIST_HPP
#define TUNGSTEN_UTILS_SLOT_LIST_HPP

#include <cstdint>
#include <vector>
#include <concepts>

namespace wUtils
{
    template<typename T>
    concept HasExistenceMethods = requires(T t)
    {
        { t.Exists() } -> std::convertible_to<bool>;
        { t.SetExists(true) };
    };

    template<typename T, bool UseExistenceMethods = HasExistenceMethods<T>>
    class SlotList;

    template<typename T>
    class SlotList<T, false>
    {
    public:
        static constexpr uint32_t Null = 0;

        SlotList()
            : m_data(), m_exists(), m_freeList()
        {
        }

        void Reserve(uint32_t count)
        {
            m_data.reserve(count);
            m_exists.reserve(count);
        }

        inline void ReserveFreeList(uint32_t count) { m_freeList.reserve(count); }

        uint32_t Add(const T& value)
        {
            if (m_freeList.empty())
            {
                m_data.push_back(value);
                m_exists.push_back(true);
                return m_data.size();
            }
            const uint32_t index = m_freeList.back();
            m_freeList.pop_back();
            m_data[index - 1] = value;
            m_exists[index - 1] = true;
            return index;
        }

        uint32_t Add(T&& value)
        {
            if (m_freeList.empty())
            {
                m_data.push_back(std::move(value));
                m_exists.push_back(true);
                return m_data.size();
            }
            const uint32_t index = m_freeList.back();
            m_freeList.pop_back();
            m_data[index - 1] = std::move(value);
            m_exists[index - 1] = true;
            return index;
        }

        void Remove(uint32_t index)
        {
            if (m_exists[index - 1])
            {
                m_exists[index - 1] = false;
                m_freeList.push_back(index);
            }
        }

        void Clear()
        {
            m_data.clear();
            m_exists.clear();
            m_freeList.clear();
        }

        inline const T& At(uint32_t index) const { return m_data[index - 1]; }
        inline const T& operator[](uint32_t index) const { return m_data[index - 1]; }
        inline T& operator[](uint32_t index) { return m_data[index - 1]; }

        inline uint32_t SlotCount() const { return m_data.size(); }
        inline uint32_t SlotCapasity() const { return m_data.capacity(); }
        inline uint32_t Count() const { return m_data.size() - m_freeList.size(); }
        inline uint32_t FreeListCount() const { return m_freeList.size(); }
        inline uint32_t FreeListCapacity() const { return m_freeList.capacity(); }


        inline bool Exists(uint32_t index) const { return m_exists[index - 1]; }

    private:
        std::vector<T> m_data;
        std::vector<bool> m_exists;
        std::vector<uint32_t> m_freeList;
    };

    template<typename T>
    class SlotList<T, true>
    {
    public:
        static constexpr uint32_t Null = 0;

        SlotList()
            : m_data(), m_freeList()
        {
        }

        void Reserve(uint32_t count)
        {
            m_data.reserve(count);
        }

        inline void ReserveFreeList(uint32_t count) { m_freeList.reserve(count); }

        uint32_t Add(const T& value)
        {
            if (m_freeList.empty())
            {
                m_data.push_back(value);
                return m_data.size();
            }
            else
            {
                const uint32_t index = m_freeList.back();
                m_freeList.pop_back();
                m_data[index - 1] = value;
                m_data[index - 1].setExists();
                return index;
            }
        }

        uint32_t Add(T&& value)
        {
            if (m_freeList.empty())
            {
                m_data.push_back(std::move(value));
                return m_data.size();
            }
            else
            {
                const uint32_t index = m_freeList.back();
                m_freeList.pop_back();
                m_data[index - 1] = std::move(value);
                return index;
            }
        }

        void Remove(uint32_t index)
        {
            if (m_data[index - 1].Exists())
            {
                m_data[index - 1].SetExists(false);
                m_freeList.push_back(index);
            }
        }

        void Clear()
        {
            m_data.clear();
            m_freeList.clear();
        }

        inline const T& At(uint32_t index) const { return m_data[index - 1]; }
        inline const T& operator[](uint32_t index) const { return m_data[index - 1]; }
        inline T& operator[](uint32_t index) { return m_data[index - 1]; }

        inline uint32_t SlotCount() const { return m_data.size(); }
        inline uint32_t SlotCapasity() const { return m_data.capacity(); }
        inline uint32_t Count() const { return m_data.size() - m_freeList.size(); }
        inline uint32_t FreeListCount() const { return m_freeList.size(); }
        inline uint32_t FreeListCapacity() const { return m_freeList.capacity(); }

        inline bool Exists(uint32_t index) const { return m_data[index - 1].Exists(); }

    private:
        std::vector<T> m_data;
        std::vector<uint32_t> m_freeList;
    };
}

#endif