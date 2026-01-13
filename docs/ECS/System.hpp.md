# System.hpp

Base class for systems.

## Navigation Menu

- [ECS.hpp](ECS.hpp.md)
- [Scene.hpp](Scene.hpp.md)
- [Entity.hpp](Entity.hpp.md)
- [Component.hpp](Component.hpp.md)
- [System.hpp](System.hpp.md)

## Functions

### update

Processes entities.

**Signature:** `virtual void update(Scene& scene, float deltaTime) = 0`

**Parameters:** scene - The scene, deltaTime - Time delta

**Returns:** None

**Example:**

```cpp
class MovementSystem : public ECS::System {
    void update(ECS::Scene& scene, float deltaTime) override {
        // Logic here
    }
};
```

[Back to Navigation](#navigation-menu) | [ECS.hpp](ECS.hpp.md) | [Scene.hpp](Scene.hpp.md)