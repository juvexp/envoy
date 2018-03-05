*   Consistent naming, where reasonable:

    *   `QuicHttpDecodeBuffer* db` (rather than `b` or `buf` or `decode_buffer`)
    *   `Base` as prefix for any class that is only used as a base class

*   Add support for counting interesting and/or detailed events. For example, an
    HpackEntryDecoderListener implementation that counts and then forwards the
    calls a wrapped listener. This avoids the cost of dealing with the counters
    all the time.
