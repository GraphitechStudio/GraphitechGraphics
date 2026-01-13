# ECS RFC Documentation

This document follows the RFC (Request for Comments) format as described in the [RFC Documentation Model](https://fr.wikipedia.org/wiki/Mod%C3%A8le:RFC/Documentation). It provides a complete specification for the Entity Component System (ECS) used in the GraphitechEngine project. The ECS is designed for efficient game development, focusing on performance, modularity, and scalability.

## Table of Contents

- [Status](#status)
- [Abstract](#abstract)
- [Introduction](#introduction)
- [Terminology](#terminology)
- [Architecture](#architecture)
- [API Specification](#api-specification)
- [Implementation Details](#implementation-details)
- [Examples](#examples)
- [Security Considerations](#security-considerations)
- [References](#references)
- [Authors](#authors)

## Status

This RFC is in **Draft** status. It describes the current implementation of the ECS in GraphitechEngine v0.1. Feedback and contributions are welcome via the project's contribution guidelines.

## Abstract

The Entity Component System (ECS) in GraphitechEngine is a data-oriented architecture for managing game entities, components, and systems. It separates data (components) from logic (systems), allowing for efficient processing, caching, and scalability. This RFC outlines the design, API, and usage of the ECS to ensure consistent implementation and extension.

## Introduction

### Background

Traditional object-oriented programming can lead to performance issues in games due to deep inheritance hierarchies and cache misses. ECS addresses this by using composition over inheritance, where entities are simple IDs, components are pure data, and systems operate on groups of components.

### Motivation

- **Performance**: Data-oriented design improves CPU cache utilization.
- **Modularity**: Systems can be added/removed without affecting entities.
- **Scalability**: Easy to handle thousands of entities.
- **Flexibility**: Components can be mixed and matched dynamically.

### Scope

This RFC covers the core ECS classes: `Scene`, `Entity`, `Component`, and `System`. It does not cover rendering-specific systems like `SpriteRenderSystem`, which are documented separately.

## Terminology

- **Entity**: A unique identifier representing an object in the game world.
- **Component**: A struct containing data for a specific aspect (e.g., position, velocity).
- **System**: A class that processes entities with specific components.
- **Scene**: A container managing entities, components, and systems.
- **Archetype**: A grouping of entities with the same set of components (for optimization).

## Architecture

### High-Level Overview

```
┌─────────────────┐
│     Scene       │
│  ┌────────────┐ │
│  │  Entities  │ │
│  └────────────┘ │
│  ┌────────────┐ │
│  │ Components │ │
│  └────────────┘ │
│  ┌────────────┐ │
│  │  Systems   │ │
│  └────────────┘ │
└─────────────────┘
```

### Core Classes

- **Scene**: Manages the lifecycle of entities and components. Provides methods to create entities, add/remove components, and query entities.
- **Entity**: A lightweight ID (e.g., uint32_t). Entities are created via `Scene::createEntity()`.
- **Component**: User-defined structs inheriting from a base `Component` class. Stored in pools for efficient access.
- **System**: Classes that iterate over entities with specific components. Registered with the scene for updates.

### Data Flow

1. Create a scene.
2. Create entities.
3. Add components to entities.
4. Register systems.
5. Update systems each frame.

## API Specification

### Scene Class

#### Methods

- `Entity createEntity()`: Creates a new entity and returns its ID.
- `void destroyEntity(Entity entity)`: Destroys an entity and removes its components.
- `template<typename T> void addComponent(Entity entity, T&& component)`: Adds a component to an entity.
- `template<typename T> T* getComponent(Entity entity)`: Retrieves a component from an entity (returns nullptr if not found).
- `template<typename T> void removeComponent(Entity entity)`: Removes a component from an entity.
- `template<typename... Ts> std::vector<Entity> getEntitiesWith<Ts...>()`: Returns entities that have all specified components.
- `void update(float deltaTime)`: Updates all registered systems.

#### Example Usage

```cpp
ECS::Scene scene;
ECS::Entity player = scene.createEntity();
scene.addComponent<PositionComponent>(player, {100.0f, 50.0f});
scene.addComponent<VelocityComponent>(player, {0.0f, 0.0f});
```

### Entity Class

- Simple ID wrapper. No methods; used as a handle.

### Component Base Class

- Virtual destructor for polymorphism.
- Components should be POD (Plain Old Data) for performance.

#### Example Component

```cpp
struct PositionComponent : public ECS::Component {
    float x, y;
};
```

### System Base Class

- `virtual void update(Scene& scene, float deltaTime) = 0`: Implement this to process entities.

#### Example System

```cpp
class MovementSystem : public ECS::System {
    void update(ECS::Scene& scene, float deltaTime) override {
        auto entities = scene.getEntitiesWith<PositionComponent, VelocityComponent>();
        for (auto entity : entities) {
            auto* pos = scene.getComponent<PositionComponent>(entity);
            auto* vel = scene.getComponent<VelocityComponent>(entity);
            pos->x += vel->vx * deltaTime;
            pos->y += vel->vy * deltaTime;
        }
    }
};
```

## Implementation Details

### Memory Management

- Components are stored in contiguous pools (e.g., std::vector) for cache efficiency.
- Entities are managed with a registry mapping IDs to component indices.

### Performance Optimizations

- Archetype-based storage: Entities with the same components are grouped.
- Query caching: `getEntitiesWith` results are cached where possible.

### Thread Safety

- Not thread-safe by default. Systems should be designed for single-threaded updates or use external synchronization.

## Examples

### Complete Game Loop

```cpp
ECS::Scene scene;

// Create player
ECS::Entity player = scene.createEntity();
scene.addComponent<PositionComponent>(player, {0.0f, 0.0f});
scene.addComponent<VelocityComponent>(player, {10.0f, 0.0f});

// Register systems
scene.registerSystem(std::make_unique<MovementSystem>());
scene.registerSystem(std::make_unique<RenderSystem>());

// Game loop
while (running) {
    float deltaTime = getDeltaTime();
    scene.update(deltaTime);
}
```

## Security Considerations

- No direct security risks, as ECS is data-focused.
- Ensure components do not contain sensitive data; validate inputs in systems.

## References

- [Entity Component System Wikipedia](https://en.wikipedia.org/wiki/Entity_component_system)
- GraphitechEngine Source Code (internal)

## Authors

- GraphitechEngine Development Team