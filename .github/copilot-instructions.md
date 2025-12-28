<!-- Copilot / AI guidance for contributors working in this repository -->
# Copilot instructions — QMK Firmware (repo-specific)

Purpose
- Help AI assistants quickly understand and work productively in this repo.

Big picture
- Monorepo for keyboard firmware. Major directories: `keyboards/` (per-board sources and keymaps), `quantum/` (core firmware code), `layouts/`, `lib/`, and `drivers/`.
- Build is driven by the top-level `Makefile` plus `builddefs/` make fragments. The top-level make delegates to `build_keyboard.mk`, `build_test.mk`, etc.
- The `qmk` CLI (Python package) is an integration point: the Makefile uses `qmk` (variable `QMK_BIN`) for tasks like listing keyboards and resolving aliases.

Essential workflows
- Install the CLI: `python3 -m pip install qmk` (repo requires Python 3.9+ and the packages in `requirements.txt`). See `docs/cli.md` for options and `QMK_HOME`.
- Build a keymap (Makefile style): `make <keyboard>:<keymap>` (example: `make dz60:default`). This is equivalent to `qmk compile -kb <keyboard> -km <keymap>`.
- Flash firmware: use `make <keyboard>:<keymap>:flash` or `qmk flash -kb <keyboard> -km <keymap>`.
- List keyboards/layouts: `qmk list-keyboards` and `qmk list-layouts --keyboard <keyboard>` (Makefile also calls these).
- Troubleshooting builds: re-run with `VERBOSE=true` or `SILENT=false` (example: `make dz60:default VERBOSE=true`). Top-level make is marked `.NOTPARALLEL:` so avoid using `-j` at the top level.

Project-specific conventions
- Keymaps live under `keyboards/<keyboard>/keymaps/<keymap>/` — each keymap may include a `keymap.c`, `config.h`, and README.
- Keyboard-level configuration (pins, MCU, layout) lives in `keyboards/<keyboard>/` and its included header files (e.g. `config.h`).
- User overlays/uspace: the Makefile supports `QMK_USERSPACE` and `QMK_HOME` (see `paths.mk` and environment handling in `Makefile`).
- Build fragments: inspect `builddefs/` for `build_keyboard.mk`, `build_test.mk`, and `support.mk` to understand build targets and messages.

Tests & CI
- Unit/integration tests live in `tests/` and are invoked by the Makefile via `build_test.mk`. CI workflows are in `.github/workflows/`.
- Repo contains `nose2.cfg` and other test helpers — check the Makefile `PARSE_TEST` / `BUILD_TEST` logic for how tests are discovered and run.

Where to look when editing or debugging
- Start with: `readme.md`, `Makefile`, `paths.mk`, and `builddefs/` (they define build flow and variables).
- For keyboard-specific behavior, inspect `keyboards/<keyboard>/` and `quantum/` for core subsystems.
- Docs: `docs/` (VitePress) contains detailed guides, especially `docs/cli.md`, `getting_started_make_guide.md`, and hardware/driver docs.

Quick examples to reference in PRs or edits
- Build: `make planck/rev6:default`  (or `qmk compile -kb planck/rev6 -km default`).
- Flash: `make planck/rev6:default:flash` (or `qmk flash -kb planck/rev6 -km default`).
- Debug build: `make <kb>:<km> VERBOSE=true` or `make <kb>:<km> SILENT=false`.

Notes for AI agents
- Prefer making minimal, targeted changes. Respect existing makefile variables and conventions (use `QMK_BIN`, `QMK_USERSPACE`, `QMK_HOME`).
- When adding or editing a keyboard/keymap, include a short README under `keymaps/<name>/` following local examples.
- If suggesting CLI or toolchain changes, reference `docs/cli.md` and the top-level `Makefile` to keep behavior consistent.

If anything here is unclear or you want coverage of additional areas (toolchains, CI, flashing via specific programmers), tell me which area to expand.
