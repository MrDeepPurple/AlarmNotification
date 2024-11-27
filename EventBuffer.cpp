#include "EventBuffer.h"

namespace eventbuffer
{
    EventBuffer::EventBuffer()
    {
        //store start time
        start_time = time(nullptr);
    }

    /*
     * @brief add a event to the event log moving the indices of
     * the circular buffer
     * @param
     * @return pointer to an iterator object
     */
    void EventBuffer::add(event_t evt)
    {
        // increment current size
        if(current_size < EVENT_LOG_SIZE)
            current_size++;

        // move end location to the next element
        end_loc = (end_loc + 1) % current_size;

        // move start buffer if they coincide
        if(end_loc == start_loc)
            start_loc = (start_loc + 1) % current_size;

        // store current event in the event buffer 
        eventlog[end_loc].event = evt;
        eventlog[end_loc].when = time(nullptr);
    }

    /*
     * @brief return date at which this class has been instantiated
     * @param
     * @return value of the start date
     */
    time_t EventBuffer::getStartTime()
    {
        return start_time;
    }

    EventBuffer::iterator * EventBuffer::getIterator()
    {
      return new EventBuffer::iterator(this);
    }

    EventBuffer::iterator::iterator(EventBuffer* evtb)
    {
        inst = evtb;
        iter_loc = inst->start_loc;
    }

    /*
     * @brief return date of the event pointed by the current position of the iterator
     * @param
     * @return value of the date of the event
     */
    time_t EventBuffer::iterator::getDate()
    {
        return inst->eventlog[iter_loc].when;
    }

    /*
     * @brief return event pointed by the current position of the iterator
     * @param
     * @return value of the event enum
     */
    event_t EventBuffer::iterator::getEvent()
    {
        return inst->eventlog[iter_loc].event;
    }

    /*
     * @brief move iterator to the next object.
     * @param
     * @return FALSE if iterator is at the end of the array
     */
    bool EventBuffer::iterator::next()
    {
        if(iter_loc == inst->end_loc)
        {
            return false;
        }
        
        iter_loc = (iter_loc + 1) % inst->current_size;
        return true;
    }

    /*
     * @brief move iterator to the previous object.
     * @param
     * @return FALSE if iterator is at the begin of the array
     */
    bool EventBuffer::iterator::prev()
    {
        if(iter_loc == inst->start_loc)
        {
            return false;
        }

        iter_loc = (iter_loc - 1) % inst->current_size;
        return true;
    }

    /*
     * @brief return true if empty.
     * @param
     * @return TRUE if buffer is empty
     */
    bool EventBuffer::iterator::empty()
    {
        return (inst->current_size > 0);
    }
}