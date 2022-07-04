#include <cassert>

#include "dependency.h"

namespace qsym {
/* @Information(alekum) : DependencyNode might not be used
  DependencyNode::DependencyNode()
    : dependencies_(NULL)
  {}

  DependencyNode::~DependencyNode() {
    delete dependencies_;
  }

  DependencySet* DependencyNode::getDependencies() {
    if (dependencies_ == NULL)
      dependencies_ = new DependencySet(computeDependencies());
    return dependencies_;
  }
*/
} // namespace qsym
