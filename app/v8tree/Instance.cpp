#include <v8tree/Instance.hpp>

namespace RNR {
  Instance::Instance() {
    parent = nullptr;
  }

  void Instance::setParent(Instance* new_parent) {
    if(parent) {
      new_parent->addChild(parent->removeChild(this));
    } else {
      new_parent->addChild(std::unique_ptr<Instance>(this));
    }
  }

  std::unique_ptr<Instance> Instance::removeChild(Instance* child) {
    int i;
    for(i = 0; i < children.size(); i++) {
      if(child == children[i].get()) {
        break;
      }
    }
    std::unique_ptr<Instance> old_ref = std::move(children[i]);
    children.erase(children.begin() + i);
    return old_ref;
  }

  void Instance::addChild(std::unique_ptr<Instance> new_child) {
    children.push_back(std::move(new_child));
    new_child->parent = this;
  }
}