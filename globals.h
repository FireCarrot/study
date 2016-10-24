#ifndef GLOBALS_H_
#define GLOBALS_H_

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete

// The USE(x) template is used to silence C++ compiler warnings
// issued for (yet) unused variables (typically parameters).
template <typename T>
inline void USE(T) { }

const int kPointerSize = sizeof(void*);

using uint8_t = unsigned char;
using byte = uint8_t;
using Address = byte*;
//typedef unsigned char uint8_t;
//typedef uint8_t byte;
//typedef byte* Address;

/**
 * Memory pressure level for the MemoryPressureNotification.
 * kNone hints that there is no memory pressure.
 * kModerate hints to speed up incremental garbage collection at the cost of
 * higher latency due to garbage collection pauses.
 * kCritical hints to free memory as soon as possible. Garbage collection
 * pauses at this level will be large.
 */
enum class MemoryPressureLevel: std::uint8_t { kNone, kModerate, kCritical };

// Use AtomicWord for a machine-sized pointer. It will use the Atomic32 or
// Atomic64 routines below, depending on you architecture.
using AtomicWorld = intptr_t;
using Atomic64 = intptr_t;

// --------------------------------------------------------------
// Constants

const int KB = 1024;
const int MB = KB * KB;
const int GB = KB * KB * KB;
const int kMaxInt = 0x7FFFFFFF;
const int kMinInt = -kMaxInt - 1;
// end of Contants ---------------------------------------------

inline Atomic64 NoBarrier_AtomicIncrement(volatile Atomic64* ptr,
                                          Atomic64 increment) {
  Atomic64 temp = increment;
  __asm__ __volatile__("lock; xaddq %0,%1"
                       : "+r" (temp), "+m" (*ptr)
                       : : "memory");
  //temp now contains the previous value of *ptr
  return temp + increment;
}

inline Atomic64 NoBarrier_Load(volatile const Atomic64* ptr) {
  return __atomic_load_n(ptr, __ATOMIC_RELAXED);
}

// Compute the 0-relative offset of some absolute value x of type T.
// This allows conversion of Addresses and integral types into
// 0-relative int offsets.
template <typename T>
inline intptr_t OffsetFrom(T x) {
  return x - static_cast<T>(0);
}

// Compute the absolute value of type T for some 0-relative offset x.
// This allows conversion of 0-relative int offsets into Addresses and
// integral types.
template <typename T>
inline T AddressFrom(intptr_t x) {
  return static_cast<T>(static_cast<T>(0) + x);
}

// Return the largest multiple of m which is <= x.
template <typename T>
inline T RoundDown(T x, intptr_t m) {
  return AddressFrom<T>(OffsetFrom(x) & -m);
}

// Return the smallest multiple of m which is >= x.
template <typename T>
inline T RoundUp(T x, intptr_t m) {
  return RoundDown<T>(static_cast<T>(x + m - 1), m);
}

void FatalProcessOutOfMemory(const char* location) {
  fprintf(stderr, "API fatal error handler returned after process out of memory \n");
}

template <typename T, typename U>
inline bool IsAligned(T value, U alignment) {
  return (value & (alignment - 1)) == 0;
}

// Returns true if (addr + offset) is aligned.
inline bool IsAddressAligned(Address addr,
                             intptr_t alignment,
                             int offset = 0) {
  intptr_t offs = OffsetFrom(addr + offset);
  return IsAligned(offs, alignment);
}
#endif // #ifndef GLOBALS_H_
