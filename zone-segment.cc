#include "zone-segment.h"

void Segment::ZapContents() {
  memset(start(), kZapDeadByte, capacity());
}

void Segment::ZapHeader() {
  memset(this, kZapDeadByte, sizeof(Segment));
}
