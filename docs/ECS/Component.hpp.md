# Component.hpp

Base class for all components.

## Navigation Menu

- [ECS.hpp](ECS.hpp.md)
- [Scene.hpp](Scene.hpp.md)
- [Entity.hpp](Entity.hpp.md)
- [Component.hpp](Component.hpp.md)
- [System.hpp](System.hpp.md)

## Description

Components are pure data structs inheriting from Component. They should be POD for performance.

**Example:**

```cpp
struct PositionComponent : public ECS::Component {
    float x, y;
};
```

[Back to Navigation](#navigation-menu) | [ECS.hpp](ECS.hpp.md) | [Scene.hpp](Scene.hpp.md)