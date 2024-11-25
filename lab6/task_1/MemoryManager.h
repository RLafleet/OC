#include <cassert>
#include <cstddef>
#include <cstring>
#include <new>
#include <mutex>

class MemoryManager
{
public:
    MemoryManager(void* start, size_t size) noexcept
        : m_start(static_cast<char*>(start))
        , m_size(size)
        , m_offset(0)
    {
        // Проверка выравнивания
        assert(reinterpret_cast<uintptr_t>(start) % alignof(std::max_align_t) == 0);
        // Блок памяти должен быть достаточно большим для работы
        assert(size >= sizeof(std::max_align_t));
    }

    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;

    void* Allocate(size_t size, size_t align = alignof(std::max_align_t)) noexcept
    {
        if (size == 0 || !IsPowerOfTwo(align))
            return nullptr;

        std::lock_guard<std::mutex> lock(m_mutex);

        uintptr_t currentAddress = reinterpret_cast<uintptr_t>(m_start + m_offset);
        uintptr_t alignedAddress = AlignUp(currentAddress, align);

        size_t padding = alignedAddress - currentAddress;
        if (m_offset + padding + size > m_size)
            return nullptr;

        m_offset += padding + size;
        return reinterpret_cast<void*>(alignedAddress);
    }

    void Free(void* addr) noexcept
    {
        // `Free` делает указатель недействительным, но ничего не освобождает.
        // Оставляем метод как заглушку, так как простой менеджер не ведёт учёт блоков.
    }

private:
    static bool IsPowerOfTwo(size_t value) noexcept
    {
        return value && (value & (value - 1)) == 0;
    }

    static uintptr_t AlignUp(uintptr_t address, size_t align) noexcept
    {
        return (address + (align - 1)) & ~(align - 1);
    }

    char* m_start;
    size_t m_size;
    size_t m_offset;
    std::mutex m_mutex;
};
