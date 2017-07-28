#ifndef LOCKS_H
#define LOCKS_H

#include <thread>
#include <mutex>
#include <condition_variable>

static std::condition_variable gVar;
static std::mutex gLock;

#endif // LOCKS_H
