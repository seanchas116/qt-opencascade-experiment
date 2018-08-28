#include "Component.hpp"

namespace ShapeEditor {

Component::Component(const std::shared_ptr<Entity> &entity) : _entity(entity) {
}

Component::~Component() {
}

} // namespace ShapeEditor
