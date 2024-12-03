#include <cstddef>
#include <cstdint>
#include <new>
#include <cassert>
#include <mutex>
#include <algorithm>

#include <iostream>

#define LOG(msg) std::cout << msg << std::endl;

class MemoryManager
{
public:
    // Инициализирует менеджер памяти непрерывным блоком size байт,
    // начиная с адреса start.
    // Возвращает true в случае успеха и false в случае ошибки
    // Методы Allocate и Free должны работать с этим блоком памяти для хранения данных.
    // Указатель start должен быть выровнен по адресу, кратному sizeof(std::max_align_t)
    MemoryManager(void* start, size_t size) noexcept
        : m_start(static_cast<uint8_t*>(start)), m_size(size)
    {
        assert(reinterpret_cast<uintptr_t>(start) % alignof(std::max_align_t) == 0);
        assert(size >= sizeof(BlockHeader));

        auto initialBlock = reinterpret_cast<BlockHeader*>(m_start);
        initialBlock->size = size - sizeof(BlockHeader);
        initialBlock->next = nullptr;
        m_freeStart = initialBlock;
    }

    // Выделяет блок памяти внутри размером size байт и возвращает адрес выделенного
    // блока памяти. Возвращённый указатель должен быть выровнен по адресу, кратному align.
    // Параметр align должен быть степенью числа 2.
    // В случае ошибки (нехватка памяти, невалидные параметры) возвращает nullptr.
    // Полученный таким образом блок памяти должен быть позднее освобождён методом Free
    void* Allocate(size_t size, size_t align = alignof(std::max_align_t)) noexcept
    {
        if (size == 0 || (align & (align - 1)) != 0) 
        {
            return nullptr;
        }

        std::lock_guard<std::mutex> lock(m_mutex);

        BlockHeader* prev = nullptr;
        BlockHeader* current = m_freeStart;

        while (current)
        {
            uintptr_t blockStart = reinterpret_cast<uintptr_t>(current + 1);
            uintptr_t alignedStart = (blockStart + (align - 1)) & ~(align - 1);
            size_t padding = alignedStart - blockStart;

            if (current->size >= size + padding)
            {
                size_t remainingSize = current->size - (size + padding);
                if (remainingSize >= sizeof(BlockHeader))
                {
                    auto newBlock = reinterpret_cast<BlockHeader*>(alignedStart + size);
                    newBlock->size = remainingSize;
                    newBlock->next = current->next;
                    current->next = newBlock;
                }

                if (prev)
                {
                    prev->next = current->next;
                }
                else
                {
                    m_freeStart = current->next;
                }

                current->size = size + padding;
                return reinterpret_cast<void*>(alignedStart);
            }

            prev = current;
            current = current->next;
        }

        return nullptr;
    }

    void Free(void* addr) noexcept
    {
        if (!addr)
        {
            return;
        }

        std::lock_guard<std::mutex> lock(m_mutex);

        auto block = reinterpret_cast<BlockHeader*>(
            static_cast<uint8_t*>(addr) - sizeof(BlockHeader));

        assert(reinterpret_cast<uint8_t*>(block) >= m_start &&
            reinterpret_cast<uint8_t*>(block) < m_start + m_size);

        BlockHeader* prev = nullptr;
        BlockHeader* current = m_freeStart;

        while (current && current < block)
        {
            prev = current;
            current = current->next;
        }

        block->next = current;
        if (prev)
        {
            prev->next = block;
        }
        else
        {
            m_freeStart = block;
        }

        CoalesceBlocks(prev, block);
        CoalesceBlocks(block, current);
    }

private:
    struct BlockHeader
    {
        size_t size;          
        BlockHeader* next;    
    };

    uint8_t* m_start;          
    size_t m_size;             
    BlockHeader* m_freeStart;  
    std::mutex m_mutex;       

    void CoalesceBlocks(BlockHeader* prev, BlockHeader* current) noexcept
    {
        if (prev && reinterpret_cast<uint8_t*>(prev) + prev->size + sizeof(BlockHeader) == reinterpret_cast<uint8_t*>(current))
        {
            prev->size += current->size + sizeof(BlockHeader);
            prev->next = current->next;
        }
    }
};
