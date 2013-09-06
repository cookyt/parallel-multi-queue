+ Get rid of allowed large types in queues. Just assume that if the type is
  non-trivial, the user will use a pointer (same assumption
  boost::lockfree::queue makes).
