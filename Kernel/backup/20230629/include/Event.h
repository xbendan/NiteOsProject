#include <macros>

class Event
{
private:
    uint64_t m_EventTypeId;

public:
    Event(uint64_t typeId);
    ~Event();

    static void CallEvent(Event *event);
    static void RegisterListener(uint64_t typeId, void (*listener)(Event &event));
};