#pragma once
#include <memory>
#include <QVector>

namespace ShapeEditor {

class Component;

class Entity final : public std::enable_shared_from_this<Entity> {
public:
    Entity();
    ~Entity();
    template <typename TComponent> void addComponent() {
        auto component = std::make_shared<TComponent>(shared_from_this());
        _components.append(component);
    }
    template <typename TComponent> const std::shared_ptr<TComponent>& component() const {
        for (const auto& component : _components) {
            auto cast = std::dynamic_pointer_cast<TComponent>(component);
            if (cast) {
                return cast;
            }
        }
        return {};
    }
private:
    QVector<std::shared_ptr<Component>> _components;
};

} // namespace ShapeEditor
