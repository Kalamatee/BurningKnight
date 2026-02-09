# Burning Knight C Port Plan (AROS)

This plan outlines the staged translation of the C# Burning Knight implementation into a C/SDL2 codebase.
Each stage should result in a runnable build with incremental parity.

## Stage 1: Core Runtime & Configuration (completed)
- Settings/config persistence (`Settings.cs`, `GlobalSave`).
- Tag registry (`Tags.cs`, `BitTag`).
- Seasonal events (`Events.cs`).
- Core bootstrap wiring and log instrumentation.

## Stage 2: Entity & Component Framework (completed)
- Entity lifecycle management (create/destroy, tags, spatial tracking).
- Input, camera, and time-step policies.
- Core combat primitives (projectiles, health, damage, pickups).

## Stage 3: Gameplay Foundations (completed)
- Player movement, combat primitives, damage, and hit effects.
- Basic items, projectiles, and interactions.
- Level/room flow baseline (arena), spawn cadence.

## Stage 4: Systems Integration (completed)
- Simple AI behaviors and spawn logic.
- HUD and pause/game over flow.
- Runtime settings save/load integrated.

## Stage 5: Parity & Polish (ongoing)
- Expand content coverage (rooms, items, bosses).
- Integrate audio pipeline and richer UI.
- Validate parity against the original C# behaviors.
