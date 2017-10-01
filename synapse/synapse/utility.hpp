#ifndef SYNAPSE_UTILITY
#define SYNAPSE_UTILITY
#include <memory>

namespace synapse {
  template<typename T>
  struct view {
    std::shared_ptr<T> data;
  }
}

#endif // SYNAPSE_UTILITY