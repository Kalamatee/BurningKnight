# Burning Knight AROS (C/SDL2)

This directory contains a C-based port of Burning Knight that targets SDL2.
It establishes the core loop, subsystem boundaries, and a playable sandbox
scene used as the foundation for a full rewrite.

## Goals
- Maintain SDL2 usage for rendering, input, audio, and timing.
- Provide a modular C architecture that mirrors the original game's systems.
- Serve as the foundation for a complete feature port.

## Controls
- Move: Arrow keys or WASD
- Shoot: Space
- Pause: P
- Reset run: R

## Settings
Settings are loaded from `aros_settings.cfg` at startup and saved on exit when
`autosave` is enabled.

## Structure
- `src/main.c`: SDL2 bootstrapping and game loop.
- `src/game.c`: High-level orchestration of game systems.
- `src/entity/`: Entity system (player, mobs, projectiles, pickups).
- `src/level/`: Level management (checkerboard floor).
- `src/state/`: State machine (pause, game over, run timer).
- `src/ui/`: UI system (HUD bar, pause/game over indicators).
- `src/core/`: Core runtime (settings, tags, events, save store).
- `src/util/`: Logging utilities.

## Build
```sh
cmake -S . -B build
cmake --build build
```

## Port Plan
See `PORT_PLAN.md` for staged port milestones.
