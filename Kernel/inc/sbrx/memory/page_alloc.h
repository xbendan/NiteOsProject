#include <mixins/meta/res.h>
#include <sbrx/memory/page.h>

namespace sbrx::mem {
    class PageRecQueue
    {
    public:
        PageRecQueue()
          : m_head(nullptr)
          , m_tail(nullptr)
          , m_size(0)
        {
        }

        PageRecQueue(PageRecQueue const& other)
        {
            if (!other.m_size) {
                return;
            }
            m_head = other.m_head;
            m_tail = other.m_tail;
            m_size = other.m_size;
        }

        PageRecQueue(PageRecQueue&& other)
        {
            if (!other.m_size) {
                return;
            }
            m_head = other.m_head;
            m_tail = other.m_tail;
            m_size = other.m_size;
        }

        void push(PageRec* page)
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

        PageRec* pop()
        {
            if (!m_size) {
                return nullptr;
            }
            PageRec* page = m_head;
            m_head        = m_head->_next;
            if (m_head) {
                m_head->_previous = nullptr;
            } else {
                m_tail = nullptr;
            }
            m_size--;
            return page;
        }

        Res<PageRec*, PageErr> take(PageRec* page)
        {
            if (page == nullptr) {
                return Res<PageRec*, PageErr>::Err(PageErr::PageNotFound);
            }

            if (page->_previous)
                page->_previous->_next = page->_next;
            else
                m_head = page->_next;

            if (page->_next)
                page->_next->_previous = page->_previous;
            else
                m_tail = page->_previous;

            m_size--;
            return Res<PageRec*, PageErr>::Ok(page);
        }

        Res<PageRec*, uint8_t> findLast() {}

    private:
        PageRec *m_head, *m_tail;
        uint32_t m_size;
    };
}