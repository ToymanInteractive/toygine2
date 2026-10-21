# Терминальный тетрис — план реализации

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Играбельный тетрис по полному Tetris Guideline в терминале, живущий в `samples/tetris/` как второй семпл движка.

**Architecture:** Два слоя с односторонней зависимостью. Чистая игровая логика в `tetris_core.*` не знает ни про POSIX, ни про время, ни про вывод — она продвигается явными вызовами и принимает прошедшее время аргументом. Платформенная обвязка в `terminal_screen.*` и `terminal_input.*` владеет терминалом, вводом и кадрами. `tetris_view.*` строит кадр из состояния игры, `tetris_sample_app.cpp` склеивает всё в цикл и даёт точку входа через `ConsoleApplication`. Логика переносится на будущее API движка без переписывания, потому что не включает ничего платформенного.

**Tech Stack:** C++23, CMake с пресетами `macos-debug` / `linux-debug`, POSIX (`termios`, `poll`, `ioctl`, `sigaction`, `clock_gettime`), ANSI-эскейпы и палитра xterm-256, из движка — `toy::application::ConsoleApplication`, `toy::StringView`, `assert_message`, `toy::assertion`.

**Spec:** `docs/superpowers/specs/2026-10-21-terminal-tetris-sample-design.md`

План отступает от спеки в одном месте: к четырём парам файлов добавлена пятая, `tetris_view.{hpp,cpp}`. Построение кадра — раскладка панелей, рамки, предпросмотры, наложения — это около трёхсот строк, которые утопили бы `tetris_sample_app.cpp` и сделали бы точку входа нечитаемой. Если это отступление не нужно, `tetris_view.*` схлопывается обратно в `tetris_sample_app.cpp` без изменения интерфейсов.

## Global Constraints

Каждая итерация обязана им соответствовать, они не повторяются в задачах.

- C++23, исключения и RTTI выключены, предупреждения — ошибки. Никаких `throw`, `dynamic_cast`, `typeid`.
- Никакого динамического выделения памяти: только буферы фиксированной ёмкости, статические или на стеке.
- Каждый `.cpp` и `.hpp` начинается блоком из `tools/builder/license/header_c_like.txt`, каждый `CMakeLists.txt` — блоком из `tools/builder/license/header_script.txt`, год — 2026.
- Разметка — только `.clang-format`: 2 пробела, 120 колонок, прикреплённые скобки. Перед каждым коммитом `clang-format -i` по изменённым файлам.
- Имена: `PascalCase` для типов, `camelCase` для функций и переменных, `snake_case` для пространств имён и файлов, константы уровня файла — `c_` плюс `camelCase`, приватные члены — с `_` в начале.
- Doxygen-блоки не нужны: `samples/` входит в `EXCLUDE` в `docs/Doxyfile.in`. Комментарий — обычный `//` в одну строку и только там, где он объясняет почему.
- Тесты в `tests/` не добавляются. Проверка итерации — `static_assert` там, где правило вычислимо на этапе компиляции, и ручной прогон в терминале там, где нет.
- `tetris_core.*` включает только стандартную библиотеку и `core.hpp`. Ни `<termios.h>`, ни `<poll.h>`, ни `<unistd.h>`, ни `<sys/ioctl.h>`, ни `<csignal>`.
- Платформы — только macOS Desktop. С PR #157 движка `console_application_posix.cpp` компилируется лишь на macOS, а `IMPLEMENT_CONSOLE_APP` на Linux отсутствует (включается `static_assert(false)` из `application.hpp`), поэтому гейт в `samples/CMakeLists.txt` сужен до macOS Desktop. Исходники семпла остаются Linux-переносимыми; когда движок вернёт консольную точку входа на Linux, гейт снова расширяется одной строкой.
- Диагностика — `ConsoleApplication::writeErrorLine`, никогда `printf` и `std::cout`. Внутри alternate screen в stdout не пишется ничего мимо кадрового буфера.
- POSIX feature-макросы объявляются через `target_compile_definitions` и только для таргета семпла: `PRIVATE _DEFAULT_SOURCE _GNU_SOURCE _DARWIN_C_SOURCE`.
- Точные значения из спеки: поле 10 × 24 (4 скрытых ряда и 20 видимых), шаг симуляции 8333 мкс, обрезка дельты кадра 250 000 мкс, не более 8 шагов за кадр, lock delay 500 000 мкс, не более 15 сбросов, максимальный уровень 15, уровень каждые 10 рядов, soft drop — интервал гравитации делённый на 20 с полом 1000 мкс, DAS 100 000 мкс, ARR 0, ёмкость буфера кадра 160 × 60, буфер вывода 32 КиБ, очередь событий 64, минимальный терминал 48 × 24.
- Коммиты в стиле репозитория: `feat(samples): ...`, `build(samples): ...`, `fix(samples): ...`. Одна итерация — один коммит.

### Команды сборки и проверки

```bash
cmake --preset macos-debug                                                  # linux-debug на Linux
cmake --build --preset macos-debug --target tetris-sample-app
./out/build/macos-debug/samples/tetris/tetris-sample-app
./tools/builder/check_code_format.sh                                        # из корня репозитория
./tools/builder/check_source_headers.sh
```

Сборка обязана проходить без единого предупреждения: таргет линкуется с `toygine::compile::warnings`, а CI трактует предупреждения как ошибки.

## Review Focus

Автоматических тестов у семпла нет, поэтому вот пять входных воздействий и состояний, которые спека подразумевает, но ни один `static_assert` не ловит. Каждое проверено вручную в итерации, которая владеет кодом.

1. **Ctrl-C и `SIGTERM` посреди партии** — шелл после выхода пригоден к работе: raw-режим снят, alternate screen закрыт, курсор виден. Проверяется в итерации 2.
2. **Запуск не в терминал и запуск в слишком маленьком терминале** — внятный отказ в stderr и ненулевой код возврата, а не поток эскейпов. Проверяется в итерации 2.
3. **Обрывок CSI-последовательности и переполнение очереди событий** — вставка из буфера обмена и залипание стрелок не вешают игру и не порождают фантомных нажатий. Проверяется в итерации 10.
4. **Уровень 15, интервал гравитации меньше шага симуляции** — фигура проходит несколько рядов за один вызов `advance`, ряды очищаются, цикл не зависает. Проверяется в итерации 8 и повторно в 11.
5. **`SIGWINCH` посреди партии** — при уменьшении терминала ниже 48 × 24 партия не теряет время и не рисуется кашей, при возврате размера продолжается с того же места. Проверяется в итерации 14.

## Итерации

Каждая итерация — отдельный файл в `2026-10-21-terminal-tetris-sample/`. Исполнитель читает свой файл и раздел **Global Constraints** выше: ограничения не повторяются в задачах.

1. [Каркас семпла](2026-10-21-terminal-tetris-sample/task-01-scaffold.md)
2. [Захват и восстановление терминала](2026-10-21-terminal-tetris-sample/task-02-terminal-takeover.md)
3. [Буфер кадра и дифференциальный вывод](2026-10-21-terminal-tetris-sample/task-03-frame-buffer.md)
4. [Раскладка экрана](2026-10-21-terminal-tetris-sample/task-04-screen-layout.md)
5. [Фигуры, повороты и столкновения](2026-10-21-terminal-tetris-sample/task-05-pieces.md)
6. [Состояние игры, поле и спавн](2026-10-21-terminal-tetris-sample/task-06-game-state.md)
7. [Мешок, очередь NEXT и hold](2026-10-21-terminal-tetris-sample/task-07-bag-queue-hold.md)
8. [Игровой цикл, гравитация и закрепление при приземлении](2026-10-21-terminal-tetris-sample/task-08-game-loop.md)
9. [Движение и SRS wall kicks](2026-10-21-terminal-tetris-sample/task-09-wall-kicks.md)
10. [Ввод и привязки](2026-10-21-terminal-tetris-sample/task-10-input.md)
11. [Lock delay, очистка рядов и завершение партии](2026-10-21-terminal-tetris-sample/task-11-lock-delay.md)
12. [Скоринг, combo, back-to-back и T-spin](2026-10-21-terminal-tetris-sample/task-12-scoring.md)
13. [Ghost, hard drop, soft drop и DAS/ARR](2026-10-21-terminal-tetris-sample/task-13-ghost-drops.md)
14. [Пауза, game over, рестарт и изменение размера терминала](2026-10-21-terminal-tetris-sample/task-14-pause-gameover.md)
