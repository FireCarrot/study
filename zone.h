#ifndef ZONE_H_
#define ZONE_H_

#include "globals.h"

class Zone final {
  public:
    Zone(AccountingAllocator* allocator, const char* name);
    ~Zone();

    // Allocate 'size' bytes of memory in the Zone; expands the Zone by
    // allocating new segments of memory on demand using malloc().
    void* New(size_t size);
  private:
    static const size_t kAlignment = kPointerSize;
}

#endif // #ifndef ZONE_H_

