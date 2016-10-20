#include "accounting-allocator.h"

#include <algorithm>

AccountingAllocator::AccountingAllocator() : unused_segments_mutex_() {
  memory_pressure_level_.SetValue(MemoryPressureLevel::kNone);
  std::fill(unused_segments_heads_,
            unused_segments_heads_ +
                (1 + kMaxSegmentSizePower - kMinSegmentSizePower),
            nullptr);

  std::fill(unused_segments_sizes,
            unused_segments_sizes + ( 1 + kMaxSegmentSizePower - kMinSegmentSizePower),
            0);
}

AccountingAllocator::~AccountingAllocator() { ClearPool(); }

void AccountingAllocator::ClearPool() {
  LockGuard<Mutex> lock_guard(&unused_segments_mutex_);

  for (uint8_t power = 0; power <= kMaxSegmentSizePower - kMinSegmentSizePower; power++) {
    Segment* current = unused_segments_heads_[power];
    while (current) {
      Segment* next = current->next();
      FreeSegment(current);
      current = next;
    }
    unused_segments_heads_[power] = nullptr;
  }
}

void AccountingAllocator::FreeSegment(Segment* memory) {
  NoBarrier_AtomicIncrement(&current_memory_usage_, -static_cast<AtomicWorld>(memory->size()));
  memory->ZapHeader();
  free(memory);
}
