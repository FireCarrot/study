#ifndef ZONE_H_
#define ZONE_H_

#include "globals.h"

// AddressSanitizer (aka ASan) detects use-after-free and buffer overflows
// Finds : buffer overflows (stack, heap, globals)
//         heap-use-after-free, stack-use-after-return
// Compiler module (clang, gcc) : instruments all loads/stores
//                                inserts redzones around stack and global variables
// Run-time libary
//
const size_t kASanRedzoneBytes = 0;

class Zone final {
  public:
    Zone(AccountingAllocator* allocator, const char* name);
    ~Zone();

    // Allocate 'size' bytes of memory in the Zone; expands the Zone by
    // allocating new segments of memory on demand using malloc().
    void* New(size_t size);
  private:
    // Expand the Zone to hold at least 'size' more bytes and allocate
    // the bytes. Returns the address of the newly allocated chunk of
    // memory in the Zone. Should only be called if there isn't enough
    // room in the Zone already.
    Address NewExpand(size_t size);

    // Creates a new segment, sets it size, and pushes it to the front
    // of the segment chain. Returns the new segment.
    inline Segment* NewSegment(size_t requested_size);

    static const size_t kAlignment = kPointerSize;
    // Never allocate segments smaller than this size in bytes.
    static const size_t kMinimumSegmentSize = 8 * KB;
    // Never allocate segments larger than this size in bytes.
    static const size_t kMaximumSegmentSize = 1 * MB;

    // Report zone excess when allocation exceeds this limit.
    static const size_t kExcessLimit = 256 * MB;

    // Deletes all objects and free all memory allocated in the Zone.
    void DeleteAll();

    // The number of bytes allocated in this zone so far.
    size_t allocation_size_;

    // The number of bytes allocated in segments. Note that this number
    // includes memory allocated from the OS but not yet allocated from
    // the zone.
    size_t segment_bytes_allocated;

    // The free region in the current (front) segment is represented as
    // the half-open interval [position, limit]. The 'position' variable
    // is guaranteed to be aligned as dictated by kAlignment.
    Address position_;
    Address limit_;

    AccountingAllocator* allocator_;

    Segment* segment_head_;
    const char* name_;
}

#endif // #ifndef ZONE_H_

