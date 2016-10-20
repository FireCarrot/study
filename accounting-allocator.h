#ifndef ZONE_ACCOUNTING_ALLOCATOR_H_
#define ZONE_ACCOUNTING_ALLOCATOR_H_

#include "globals.h"
#include "mutex.h"
#include "zone-segment.h"

class AccountingAllocator {
  public:
    AccountingAllocator();
    virtual ~AccountingAllocator();

    // Gets an empty segment from the pool or creates a new one.
    virtual Segment* GetSegment(size_t bytes);

  private:
    static const uint8_t kMinSegmentSizePower = 13;
    static const uint8_t kMaxSegmentSizePower = 18;
    static const uint8_t kMaxSegmentsPerBucket = 5;

    AtomicValue<MemoryPressureLvel> memory_pressure_level_;
    Mutex unused_segments_mutex_;

    AtomicWorld current_memory_usage_ = 0;
    AtomicWorld max_memory_usage_ = 0;
    AtomicWorld current_pool_size_ = 0;

    void FreeSegment(Segment* memory);

    // Empties the pool and puts all its contents onto the garbage stack.
    void ClearPool();

    Segment* unused_segments_heads[1 + kMaxSegmentSizePower - kMinSegmentSizePower];
    size_t unused_segments_sizes[1 + kMaxSegmentSizePower - kMinSegmentSizePower];

    DISALLOW_COPY_AND_ASSIGN(AccountingAllocator);
};
