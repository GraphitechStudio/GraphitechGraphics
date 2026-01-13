# Scene.hpp

Manages entities, components, and systems.

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

### destroyEntity

Destroys an entity.

**Signature:** `void destroyEntity(Entity entity)`

**Parameters:** entity - The entity to destroy

**Returns:** None

**Example:**

```cpp
scene.destroyEntity(e);
```

### addComponent

Adds a component to an entity.

**Signature:** `template<typename T> void addComponent(Entity entity, T&& component)`

**Parameters:** entity - The entity, component - The component data

**Returns:** None

**Example:**

```cpp
scene.addComponent<PositionComponent>(e, {0.0f, 0.0f});
```

### getComponent

Retrieves a component.

**Signature:** `template<typename T> T* getComponent(Entity entity)`

**Parameters:** entity - The entity

**Returns:** Pointer to component or nullptr

**Example:**

```cpp
auto* pos = scene.getComponent<PositionComponent>(e);
```

### removeComponent

Removes a component.

**Signature:** `template<typename T> void removeComponent(Entity entity)`

**Parameters:** entity - The entity

**Returns:** None

**Example:**

```cpp
scene.removeComponent<PositionComponent>(e);
```

### getEntitiesWith

Gets entities with specific components.

**Signature:** `template<typename... Ts> std::vector<Entity> getEntitiesWith<Ts...>()`

**Parameters:** None

**Returns:** Vector of entities

**Example:**

```cpp
auto entities = scene.getEntitiesWith<PositionComponent>();
```

### update

Updates all systems.

**Signature:** `void update(float deltaTime)`

**Parameters:** deltaTime - Time since last update

**Returns:** None

**Example:**

```cpp
scene.update(0.016f);
```

[Back to Navigation](#navigation-menu) | [ECS.hpp](ECS.hpp.md) | [Entity.hpp](Entity.hpp.md)