# ECS.hpp

Main ECS header file.

## Navigation Menu

- [ECS.hpp](ECS.hpp.md)
- [Scene.hpp](Scene.hpp.md)
- [Entity.hpp](Entity.hpp.md)
- [Component.hpp](Component.hpp.md)
- [System.hpp](System.hpp.md)

## Functions

### createEntity

Creates a new entity.

**Signature:** `Entity createEntity()`

**Parameters:** None

**Returns:** New Entity ID

**Example:**

```cpp
ECS::Scene scene;
ECS::Entity e = scene.createEntity();
```

[Back to Navigation](#navigation-menu) | [Scene.hpp](Scene.hpp.md) | [Entity.hpp](Entity.hpp.md)