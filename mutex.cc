#include "mutex.h"

#include <cerrno>

static inline void InitializeNativeHandle(pthread_mutex_t* mutex) {
  int result;
  // Use an error checking mutex in debug mode.
#if defined(DEBUG)
  pthread_mutexattr_t attr;
  result = pthread_mutexattr_init(&attr);
  result = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
  result = pthread_mutex_init(mutex, &attr);
  result = pthread_mutexattr_destroy(&attr);
#else
  // Use a fast mutex (default attributes).
  result = pthread_mutex_init(mutex, nullptr);
#endif // defined(DEBUG)
  USE(result);
}

static inline void DestroyNativeHandle(pthread_mutex_t* mutex) {
  int result = pthread_mutex_destroy(mutex);
  USE(result);
}

static inline void LockNativeHandle(pthread_mutex_t* mutex) {
  int result = pthread_mutex_lock(mutex);
  USE(result);
}

static inline void UnlockNativeHandle(pthread_mutex_t* mutex) {
  int result = pthread_mutex_unlock(mutex);
  USE(result);
}

static inline bool TryLockNativeHandle(pthread_mutex_t* mutex) {
  int result = pthread_mutex_trylock(mutex);
  if (result == EBUSY) {
    return false;
  }

  return true;
}

Mutex::Mutex() {
  InitializeNativeHandle(&native_handle_);
}

Mutex::~Mutex() {
  DestroyNativeHandle(&native_handle_);
}

void Mutex::Lock() {
  LockNativeHandle(&native_handle_);
}

void Mutex::Unlock() {
  UnlockNativeHandle(&native_handle_);
}

bool Mutex::TryLock() {
  return TryLockNativeHandle(&native_handle_);
}
