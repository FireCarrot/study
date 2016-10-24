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
    static const size_t kNumberBuckets =
        1 + kMaxSegmentSizePower - kMinSegmentSizePower;

    // Allocates a new segment. Returns nullptr on failed allocation.
    Segment* AllocateSegment(size_t bytes);
    void FreeSegment(Segment* memory);

    size_t GetCurrentMemoryUsage() const;
    size_t GetMaxMemoryUsage() const;

    size_t GetCurrentPoolSize() const;

    AtomicValue<MemoryPressureLvel> memory_pressure_level_;
    Mutex unused_segments_mutex_;

    AtomicWorld current_memory_usage_ = 0;
    AtomicWorld max_memory_usage_ = 0;
    AtomicWorld current_pool_size_ = 0;

    // Empties the pool and puts all its contents onto the garbage stack.
    void ClearPool();

    Segment* unused_segments_heads_[kNumberBuckets];

    size_t unused_segments_sizes_[kNumberBuckets];
    size_t unused_segments_max_sizes_[kNumberBuckets];

    DISALLOW_COPY_AND_ASSIGN(AccountingAllocator);
};
