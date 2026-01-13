# Contribution Documentation

This section provides guidelines for contributing to the ECS development. Contributions should follow these rules to maintain code quality and consistency.

## Getting Started

1. Fork the repository.
2. Create a feature branch: `git checkout -b feature/your-feature`.
3. Make changes.
4. Submit a pull request with a clear description.

## Code Style

- Use C++17 features.
- Follow Google C++ Style Guide.
- Use RAII for resource management.
- Document all public APIs with Doxygen-style comments.

## Testing

- Add unit tests for new components/systems.
- Use the provided test framework (e.g., for SpriteBatch_test.cpp).

## Documentation

- Update this RFC for API changes.
- Add .md files for new .hpp files, with navigation links.