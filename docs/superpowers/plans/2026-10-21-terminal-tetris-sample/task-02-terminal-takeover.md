# Итерация 2: Захват и восстановление терминала

> Общие ограничения, команды сборки и Review Focus — в индексе плана [`2026-10-21-terminal-tetris-sample.md`](../2026-10-21-terminal-tetris-sample.md); они обязательны и для этой итерации. Соседние итерации, на которые ссылается текст, — файлы `task-NN-*.md` рядом.

**Files:**

- Modify: `samples/tetris/terminal_screen.hpp`
- Modify: `samples/tetris/terminal_screen.cpp`
- Modify: `samples/tetris/tetris_sample_app.cpp`

**Interfaces:**

- Consumes: `toy::assertion::initialize()`, `toy::assertion::setCallbacks()`, `toy::assertion::deInitialize()`, `toy::assertion::Action`, `ConsoleApplication::writeErrorLine`.
- Produces, в пространстве имён `terminal`:

```cpp
struct Size { int32_t columns; int32_t rows; };
enum class EnterResult : uint8_t { ok, notATerminal, tooSmall, setupFailed };

inline constexpr int32_t c_maxColumns = 160;
inline constexpr int32_t c_maxRows = 60;
inline constexpr int32_t c_minColumns = 48;
inline constexpr int32_t c_minRows = 24;

Size querySize() noexcept;          // ioctl(TIOCGWINSZ), при неудаче {0, 0}
EnterResult enter() noexcept;       // isatty, размер, termios raw, alternate screen, скрытый курсор, sigaction
void leave() noexcept;              // идемпотентно; обратный порядок
```

- Produces, в `tetris_sample_app.cpp`: `class TerminalGuard` без копирования и перемещения, чей деструктор вызывает `terminal::leave()`.

- [ ] **Step 1: Реализовать `querySize` и `enter`/`leave`**

`enter`: `isatty(STDIN_FILENO)` и `isatty(STDOUT_FILENO)` → `notATerminal`; `querySize` меньше `c_minColumns × c_minRows` или больше `c_maxColumns × c_maxRows` → `tooSmall`; `tcgetattr` не удался → `setupFailed`. Затем `cfmakeraw` на копии сохранённых настроек, `tcsetattr(TCSANOW)`, запись `ESC[?1049h` и `ESC[?25l` через `write(STDOUT_FILENO, ...)`, полная перерисовка. Сохранённый `termios`, исходный размер и признак «внутри alternate screen» — файловые переменные, потому что до них должен доставать и обработчик сигнала, и обработчик ассерта.

`leave`: идемпотентен; если признак не поднят, не делает ничего. Иначе `ESC[0m`, `ESC[?25h`, `ESC[?1049l`, `tcsetattr` сохранённых настроек, сброс признака. Флаги `fcntl` не менялись вовсе, восстанавливать нечего.

- [ ] **Step 2: Перехватить сигналы завершения**

`sigaction` на `SIGINT`, `SIGTERM`, `SIGHUP`. Обработчик вызывает `terminal::leave()`, затем повторно поднимает свой сигнал с `SIG_DFL`, чтобы код возврата остался честным. Внутри обработчика только `write` и `tcsetattr` — оба async-signal-safe. `sa_flags` без `SA_RESTART`. Объявить как `struct sigaction`, не `sigaction`: в C++ имя типа и функции совпадает.

- [ ] **Step 3: Зарегистрировать обработчик ассерта**

В `runInternal` до `terminal::enter()`: `toy::assertion::initialize()` и `setCallbacks(reportAssertion, nullptr)`, где `reportAssertion` — файловая функция с сигнатурой `toy::assertion::Action(const char *) noexcept`: вызывает `terminal::leave()`, печатает полученную строку в stderr через `write(STDERR_FILENO, ...)` и возвращает `toy::assertion::Action::Break`. В конце `runInternal` — `deInitialize()`.

- [ ] **Step 4: Собрать `runInternal` на `TerminalGuard`**

`TerminalGuard` создаётся сразу после успешного `enter()`, чтобы деструктор сработал на любом выходе. При `EnterResult` отличном от `ok` — соответствующее сообщение через `writeErrorLine` и `return false`. Сообщения: `notATerminal` — «Tetris sample needs an interactive terminal», `tooSmall` — «Terminal must be at least 48 columns by 24 rows», `setupFailed` — «Failed to switch the terminal to raw mode». После успешного входа — блокирующий `read(STDIN_FILENO, &byte, 1)` одного байта и `return true`; `terminal::waitForInput` появится только в итерации 8.

- [ ] **Step 5: Проверить восстановление терминала вручную**

Run: собрать, запустить, во время работы нажать Ctrl-C; затем запустить повторно и отправить `SIGTERM` из другого терминала.
Expected: в обоих случаях шелл после выхода пригоден к работе — ввод отображается, курсор виден, содержимое экрана до запуска на месте.

- [ ] **Step 6: Проверить отказы вручную**

Run: `./out/build/macos-debug/samples/tetris/tetris-sample-app | cat` и запуск в терминале размером 40 × 20.
Expected: внятное сообщение в stderr, ненулевой код возврата, никакого мусора из эскейпов.

- [ ] **Step 7: Коммит**

```bash
git add samples/tetris
git commit -m "feat(samples): take over and restore the terminal in tetris sample"
```
