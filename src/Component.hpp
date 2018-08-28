#pragma once
#include <memory>

namespace ShapeEditor {

class Entity;

class Component {
public:
    Component(const std::shared_ptr<Entity>& entity);
    virtual ~Component();
    std::shared_ptr<Entity> entity() const { return _entity.lock(); }
private:
    std::weak_ptr<Entity> _entity;
};

} // namespace ShapeEditor
