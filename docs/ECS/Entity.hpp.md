# Entity.hpp

Represents a unique entity ID.

## Navigation Menu

- [ECS.hpp](ECS.hpp.md)
- [Scene.hpp](Scene.hpp.md)
- [Entity.hpp](Entity.hpp.md)
- [Component.hpp](Component.hpp.md)
- [System.hpp](System.hpp.md)

## Description

Entity is a lightweight wrapper around an ID (e.g., uint32_t). It has no methods; used as a handle for components and systems.

**Example:**

```cpp
ECS::Entity e = scene.createEntity();
```

[Back to Navigation](#navigation-menu) | [ECS.hpp](ECS.hpp.md) | [Scene.hpp](Scene.hpp.md)