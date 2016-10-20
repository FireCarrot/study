#ifndef ZONE_SEGMENT_H_
#define ZONE_SEGMENT_H_

#include "globals.h"
#include "zone.h"

class Segment {
  public:
    void Initialize(size_t size) { size_ = size; }

    Zone* zone() const { return zone_; }
    void set_zone(Zone* const zone) { zone_ = zone; }

    Segment* next() const { return next_; }
    void set_next(Segment* const next) { next_ = next; }

    size_t size() const { return size_; }
    size_t capacity() const { return size_ - sizeof(Segment); }

    Address start() const { return address(sizeof(Segment)); }
    Address end() const { return address(size_); }

    // Zap the contents of the segment (but not the header).
    void ZapContents();
    // Zaps the header and makes the segment unusable this way.
    void ZapHeader();

  private:
    // Constant byte value used for zapping dead memory in debug mode.
    static const unsigned char kZapDeadByTE = 0xcd;
    // Computes the address of the nth byte in this segment.
    Address address(size_t n) const { return Address(this) + n; }
    Zone* zone_;
    Segment* next_;
    size_t size_;
};

#endif // #ifndef ZONE_SEGMENT_H_
