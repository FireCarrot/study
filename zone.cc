#include "zone.h"

Zone::Zone(AccountingAllocator* allocator, const char* name)
    : allocation_size_(0),
      segment_bytes_allocated_(0),
      position_(0),
      limit_(0),
      allocator_(allocator),
      segment_head_(nullptr),
      name_(name) {
  allocator_->ZoneCreation(this);
}

Zone::~Zone() {
}

void* Zone::New(size_t size) {
  // Round up the requested size to fit the alignment.
  size = RoundUp(size, kAlignment);
}
