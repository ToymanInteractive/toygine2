# Итерация 1: Каркас семпла

> Общие ограничения, команды сборки и Review Focus — в индексе плана [`2026-10-21-terminal-tetris-sample.md`](../2026-10-21-terminal-tetris-sample.md); они обязательны и для этой итерации. Соседние итерации, на которые ссылается текст, — файлы `task-NN-*.md` рядом.

**Files:**

- Create: `samples/tetris/CMakeLists.txt`
- Create: `samples/tetris/tetris_sample_app.cpp`
- Create: `samples/tetris/tetris_core.hpp`
- Create: `samples/tetris/tetris_core.cpp`
- Create: `samples/tetris/terminal_screen.hpp`
- Create: `samples/tetris/terminal_screen.cpp`
- Create: `samples/tetris/terminal_input.hpp`
- Create: `samples/tetris/terminal_input.cpp`
- Create: `samples/tetris/tetris_view.hpp`
- Create: `samples/tetris/tetris_view.cpp`
- Modify: `samples/CMakeLists.txt`

**Interfaces:**

- Consumes: `toy::application::ConsoleApplication`, `toy::application::Version`, `IMPLEMENT_CONSOLE_APP` из `<application.hpp>`.
- Produces: класс `TetrisSampleApp final : public toy::application::ConsoleApplication` с `constexpr explicit TetrisSampleApp(const toy::application::Version & version) noexcept` и `bool runInternal() noexcept override`; таргет CMake `tetris-sample-app`. Все остальные файлы создаются пустыми заглушками — только лицензионный заголовок, `\file`-комментарий не нужен, пространство имён и ни одного символа.

- [ ] **Step 1: Создать `samples/tetris/CMakeLists.txt`**

Блок из `header_script.txt`, затем ровно то, что в спеке в разделе **Сборка**: `add_executable(tetris-sample-app ...)` со всеми пятью `.cpp`, `target_link_libraries(... PRIVATE toygine toygine::compile::warnings)`, `FOLDER "samples"`, `target_compile_definitions(... PRIVATE _DEFAULT_SOURCE _GNU_SOURCE _DARWIN_C_SOURCE)` и `install(TARGETS ...)`.

- [ ] **Step 2: Подключить каталог в `samples/CMakeLists.txt`**

После существующего `add_subdirectory(console)`:

```cmake
if (TOYGINE_TARGET_PLATFORM STREQUAL "Linux Desktop" OR TOYGINE_TARGET_PLATFORM STREQUAL "macOS Desktop")
  add_subdirectory(tetris)
endif()
```

- [ ] **Step 3: Написать `tetris_sample_app.cpp`**

Заголовок из `header_c_like.txt`, `#include <application.hpp>`, класс `TetrisSampleApp` по образцу `samples/console/console_sample_app.cpp:297-324`, весь файл целиком под `#if defined(__linux__) || defined(__APPLE__)`. `runInternal` печатает `writeOutputLine(toy::StringView{"Tetris sample app"})` и версию через `version()`, возвращает `true`. В конце `IMPLEMENT_CONSOLE_APP(TetrisSampleApp)`.

- [ ] **Step 4: Создать четыре пары заглушек**

Каждый файл — лицензионный заголовок и include guard в стиле репозитория (`SAMPLES_TETRIS_TETRIS_CORE_HPP_`); `.cpp` включает свой `.hpp`. `tetris_core.*` — пространство имён `tetris`, `terminal_screen.*` и `terminal_input.*` — пространство имён `terminal`, `tetris_view.*` — пространство имён `tetris::view`. Пустые, но компилируемые.

- [ ] **Step 5: Собрать и запустить**

Run: команды сборки из раздела выше.
Expected: сборка без предупреждений, бинар печатает две строки и завершается с кодом 0.

- [ ] **Step 6: Проверить линтеры и закоммитить**

Run: `./tools/builder/check_code_format.sh` и `./tools/builder/check_source_headers.sh`
Expected: оба завершаются с кодом 0.

```bash
git add samples/tetris samples/CMakeLists.txt
git commit -m "feat(samples): scaffold terminal tetris sample target"
```
