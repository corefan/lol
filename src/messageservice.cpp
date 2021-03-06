//
// Lol Engine
//
// Copyright: (c) 2013 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//                2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cstring>
#include <cstdlib>
#include <time.h>

namespace lol
{

// This is needed to map MessageService::Send() to the JS when building the HTML.
#if EMSCRIPTEN
extern "C"
{
    int C_Send(const char* message) { return (int)MessageService::Send(MessageBucket::AppIn, message); }
    //NOT IMPLEMENTED
    //bool C_FetchFirst(String& message);
}
#endif //EMSCRIPTEN

/*
 * The global g_renderer object, initialised by Video::Init
 */
MessageService *g_messageservice = nullptr;

//-----------------------------------------------------------------------------
// Ctor/Dtor
//----
MessageService::MessageService()
{
}

MessageService::~MessageService()
{
    m_bucket.empty();
}

//Setup/Destroy
void MessageService::Setup()
{
    g_messageservice = new MessageService();
    g_messageservice->m_bucket.resize(MessageBucket::MAX);
}

void MessageService::Destroy()
{
    delete g_messageservice;
    g_messageservice = nullptr;
}

//-----------------------------------------------------------------------------
bool MessageService::Send(MessageBucket id, const String& message)
{
    if (g_messageservice)
    {
        ASSERT(0 <= id.ToScalar() && id.ToScalar() < g_messageservice->m_bucket.count());
        return g_messageservice->Send(id, message.C());
    }
    return false;
}

bool MessageService::Send(MessageBucket id, const char* message)
{
    if (g_messageservice)
    {
        ASSERT(0 <= id.ToScalar() && id.ToScalar() < g_messageservice->m_bucket.count());
        MessageService& g = *g_messageservice;
        array<MessageList>& bucket = g.m_bucket[id.ToScalar()];
        bucket << MessageList(time(nullptr), String(message));
        return true;
    }
    return false;
}

//----
bool MessageService::FetchFirst(MessageBucket id, String& message)
{
    if (g_messageservice)
    {
        ASSERT(0 <= id.ToScalar() && id.ToScalar() < g_messageservice->m_bucket.count());
        time_t timestamp;
        return g_messageservice->FetchFirst(id, message, timestamp);
    }
    return false;
}

bool MessageService::FetchFirst(MessageBucket id, String& message, time_t& timestamp)
{
    if (g_messageservice)
    {
        ASSERT(0 <= id.ToScalar() && id.ToScalar() < g_messageservice->m_bucket.count());
        MessageService& g = *g_messageservice;
        array<MessageList>& bucket = g.m_bucket[id.ToScalar()];

        if (bucket.count())
        {
            message = bucket[0].m_message;
            timestamp = bucket[0].m_timestamp;
            bucket.remove(0);
            return true;
        }
    }
    return false;
}

//----
bool MessageService::FetchAll(MessageBucket id, String& message)
{
    if (g_messageservice)
    {
        ASSERT(0 <= id.ToScalar() && id.ToScalar() < g_messageservice->m_bucket.count());
        time_t timestamp;
        return g_messageservice->FetchAll(id, message, timestamp);
    }
    return false;
}

bool MessageService::FetchAll(MessageBucket id, String& message, time_t& first_timestamp)
{
    if (g_messageservice)
    {
        ASSERT(0 <= id.ToScalar() && id.ToScalar() < g_messageservice->m_bucket.count());
        MessageService& g = *g_messageservice;
        array<MessageList>& bucket = g.m_bucket[id.ToScalar()];
        message = String("");

        if (bucket.count())
        {
            first_timestamp = bucket[0].m_timestamp;
            for (int i = 0; i < bucket.count(); ++i)
                message += bucket[i].m_message;
            bucket.empty();
            return true;
        }
    }
    return false;
}

} /* namespace lol */

