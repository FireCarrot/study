#ifndef MUTEX_H_
#define MUTEX_H_

// ----------------------------------------------------------------------------
// Mutex
//
// This class is a synchronization primitive that can be used to protect shared
// data from being simultaneously accessed by multiple threads. A mutex offers
// exclusive, non-recursive ownership semantics:
// - A calling thread owns a mutex from the time that it successfully calls
//   either |Lock()| or |TryLock()| until it calls |Unlock()|.
// - When a thread owns a mutex, all other threads will block (for calls to
//   |Lock()|) or receive a |false| return value (for |TryLock()|) if they
//   attempt to claim ownership of the mutex.
// A calling thread must not own the mutex prior to calling |Lock()| or
// |TryLock()|. The behavior of a program is undefined if a mutex is destroyed
// while still owned by some thread. The mutex class is non-copyable.

#include <pthread.h>

#include "globals.h"

class Mutex final {
  public:
    Mutex();
    ~Mutex();

    // Locks the given mutex. If the mutex is currently unlocked, it becomes
    // locked and owned by the calling thread, and immediatly. If the mutex
    // is already locked by another thread, suspends the calling thread until
    // the mutex is unlocked.
    void Lock();

    // Unlocks the given mutex. The mutex is assumed to be locked and owned by
    // the calling thread on entrance.
    void Unlock();

    // Tries to lock the given mutex. Returns whether the mutex was
    // successfully locked.
    bool TryLock();

    typedef pthread_mutex_t NativeHandle;

    NativeHandle& native_handle() {
      return native_handle_;
    }

    const NativeHandle& native_handle() const {
      return native_handle_;
    }

  private:
    NativeHandle native_handle_;

    DISALLOW_COPY_AND_ASSIGN(Mutex);
};

// -----------------------------------------------------------------------------
// LockGuard
//
// This class is a mutex wrapper that provides a convenient RAII-style mechanism
// for owning a mutex for the duration of a scoped block.
// When a LockGuard object is created, it attempts to take ownership of the
// mutex it is given. When control leaves the scope in which the LockGuard
// object was created, the LockGuard is destructed and the mutex is released.
// The LockGuard class in non-copyable.

template <typename Mutex>
class LockGuard final {
  public:
    explicit LockGuard(Mutext* mutex) : mutex_(mutex) { mutex_->Lock(); }
    ~LockGuard() { mutex_->Unlock(); }

  private:
    Mutex* mutex_;

    DISALLOW_COPY_AND_ASSIGN(LockGuard);
};

#endif //#ifndef MUTEX_H_
