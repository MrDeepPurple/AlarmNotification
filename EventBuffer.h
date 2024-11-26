#include <cstdint>
#include <ctime>
#include "Configuration.h"

#ifndef EVENT_BUFFER_H
#define EVENT_BUFFER_H
namespace eventbuffer
{

enum event_t: uint8_t {
    CONN_REST = 0, /* connection restored */
    CONN_LOST, /* connection lost */
    STARTED, /* started */
    ALARM_ENABLED, /* alarm passed from disabled to enabled */
    ALARM_DISABLED, /* alarm passed from enabled to disabled */
    ALARM_TRIGGERED, /* alarm is triggered */
    ALARM_BACKTONORMAL /* alarm passed from triggered to normal */
};

struct eventlog_entry_t {
    event_t event;
    time_t when;
};

class EventBuffer
{
    private:
        eventlog_entry_t eventlog[EVENT_LOG_SIZE];
        uint8_t start_loc;
        uint8_t end_loc;
        uint8_t current_size;
        time_t start_time;

    public:
        friend class iterator;
        class iterator
        {
            private:
                EventBuffer* inst;
                int16_t iter_loc;
                iterator(EventBuffer *evt);
            public:
                friend class EventBuffer;
                bool empty();
                bool next();
                bool prev();
                event_t getEvent();
                time_t getDate();
        };
        EventBuffer();
        iterator* getIterator();
        void add(event_t);
        time_t getStartTime();
};

}

#endif
