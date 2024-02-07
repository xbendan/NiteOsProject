#pragma once

#include <stdcxx/linked_list.h>
#include <stdcxx/math.h>
#include <stdcxx/types.h>

#define PAGE_SIZE_4K 4096ULL
#define PAGE_SIZE_2M 2097152ULL
#define PAGE_SIZE_1G 1073741824ULL
#define PAGE_FLAG_FREE (1 << 0)
#define PAGE_FLAG_KMEM (1 << 1)
#define PAGE_FLAG_SWAP (1 << 2)
#define PAGE_FLAG_LOCK (1 << 3)
#define PAGE_FLAG_DIRTY (1 << 4)
#define PAGE_FLAG_SHARE (1 << 5)
#define PAGE_PART_SIZE (PAGE_SIZE_1G / 4)
#define PAGE_PART_COUNT (PAGE_PART_SIZE / PAGE_SIZE_4K)

namespace Kern::Mem {
    struct Page4K
    {
        Page4K* _next;
        Page4K* _previous;
        uint8_t _order;
        uint8_t _flags;
        struct /* slub */
        {
            struct
            {
                uint32_t _inuse : 16;
                uint32_t _objects : 15;
                uint32_t _frozen : 1;
            } __attribute__((packed));
            void** _freelist;
        };

        union
        {
            uint64_t _priv;
            Page4K*  _pageHead;
            void*    _kmemPool;
        };
        uint64_t _address;

        inline bool isFree()
        {
            return _flags & (PAGE_FLAG_FREE   //
                             | PAGE_FLAG_KMEM //
                             | PAGE_FLAG_SWAP);
        }

        inline uint64_t size() { return (1 << _order) * PAGE_SIZE_4K; }

        Page4K* split();
        Page4K* merge(Page4K* page);

        Page4K* merge();
    };

    class Page4KFifoQueue
    {
    public:
        Page4KFifoQueue()
          : m_head(nullptr)
          , m_tail(nullptr)
          , m_size(0)
        {
        }

        Page4KFifoQueue(Page4KFifoQueue const& other)
        {
            if (!other.m_size) {
                return;
            }
            m_head = other.m_head;
            m_tail = other.m_tail;
            m_size = other.m_size;
        }

        Page4KFifoQueue(Page4KFifoQueue&& other)
        {
            if (!other.m_size) {
                return;
            }
            m_head = other.m_head;
            m_tail = other.m_tail;
            m_size = other.m_size;
        }

        void push(Page4K* page)
        {
            if (!m_size) {
                m_head = page;
                m_tail = page;
            } else {
                m_tail->_next   = page;
                page->_previous = m_tail;
                m_tail          = page;
            }
            m_size++;
        }

        Page4K* pop()
        {
            if (!m_size) {
                return nullptr;
            }
            Page4K* page = m_head;
            m_head       = m_head->_next;
            if (m_head) {
                m_head->_previous = nullptr;
            } else {
                m_tail = nullptr;
            }
            m_size--;
            return page;
        }

        Page4K* takeAway(Page4K* page)
        {
            if (page->_previous) {
                page->_previous->_next = page->_next;
            } else {
                m_head = page->_next;
            }
            if (page->_next) {
                page->_next->_previous = page->_previous;
            } else {
                m_tail = page->_previous;
            }
            m_size--;
            return page;
        }

        Page4K* head() { return m_head; }

        Page4K* tail() { return m_tail; }

        uint32_t size() { return m_size; }

    private:
        Page4K*  m_head;
        Page4K*  m_tail;
        uint32_t m_size;
    };

    struct PagePartitionModel
    {
        Page4K*  _pages;
        uint32_t _modelId;
    };

    Page4K* pageOf(uint64_t address);
}