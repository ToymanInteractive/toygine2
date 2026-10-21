# Итерация 10: Ввод и привязки

> Общие ограничения, команды сборки и Review Focus — в индексе плана [`2026-10-21-terminal-tetris-sample.md`](../2026-10-21-terminal-tetris-sample.md); они обязательны и для этой итерации. Соседние итерации, на которые ссылается текст, — файлы `task-NN-*.md` рядом.

**Files:**

- Modify: `samples/tetris/terminal_input.hpp`
- Modify: `samples/tetris/terminal_input.cpp`
- Modify: `samples/tetris/tetris_sample_app.cpp`

**Interfaces:**

- Consumes: `tetris::GameState`, `tetris::tryShift`, `tetris::tryRotate`, `tetris::swapHold`, `tetris::startGame`, `tetris::Phase`, `terminal::waitForInput`.
- Produces, в `terminal`:

```cpp
enum class Key : uint8_t {
  none, left, right, up, down, space, rotateCounterClockwise, hold, pause, restart, quit
};

inline constexpr size_t c_maxEventsPerFrame = 64;

size_t readKeys(std::span<Key> out) noexcept;   // сколько клавиш разобрано, не больше out.size()
```

- Produces, в `tetris_sample_app.cpp`: `void dispatchKey(tetris::GameState & state, terminal::Key key) noexcept;`

- [ ] **Step 1: Реализовать `readKeys`**

Один `read(STDIN_FILENO, ...)` в локальный буфер на 256 байт, затем разбор. Распознаются `ESC [ A/B/C/D` и `ESC O A/B/C/D` — вверх/вниз/вправо/влево, `0x20` — `space`, `z` и `Z` — `rotateCounterClockwise`, `c` и `C` — `hold`, `p` и `P` — `pause`, `r` и `R` — `restart`, `q` и `Q` — `quit`, `0x03` — `quit`. Всё прочее отбрасывается.

Незавершённая последовательность в конце буфера — префикс `ESC`, `ESC [`, `ESC O` или `ESC [ 1..9` — сохраняется в файловый остаточный буфер на 8 байт и подставляется в начало следующего чтения. Если накопленное уже не может стать началом распознаваемой последовательности, остаток отбрасывается. Голый `ESC` ни на что не назначен, поэтому таймер различения не нужен.

- [ ] **Step 2: Реализовать `dispatchKey`**

`left` → `tryShift(state, -1)`, `right` → `tryShift(state, 1)`, `up` → `tryRotate(state, Rotation::clockwise)`, `rotateCounterClockwise` → `tryRotate(state, Rotation::counterClockwise)`, `hold` → `swapHold(state)`, `quit` → признак выхода из цикла, `restart` → `startGame(state, makeSeed())` и только когда `phase == gameOver`, `pause` → `togglePause`, которого пока нет: до итерации 14 клавиша игнорируется. `down` и `space` обрабатываются в итерации 13.

- [ ] **Step 3: Подключить к циклу и убрать временные привязки**

В цикле после `waitForInput`: `readKeys` в `std::array<Key, c_maxEventsPerFrame>`, затем `dispatchKey` на каждое. Временные `a`/`d`/`w`/`s` из итерации 9 удалить. Выход по `quit` вместо выхода по любому байту.

- [ ] **Step 4: Проверить привязки вручную**

Run: собрать, запустить, поиграть полминуты.
Expected: каждая клавиша из таблицы спеки делает своё; залипаний, двойных срабатываний и фантомных поворотов нет; `Q` и Ctrl-C выходят и оставляют шелл рабочим.

- [ ] **Step 5: Проверить мусор во вводе вручную**

Run: во время партии вставить в терминал строку текста из буфера обмена и зажать стрелку влево на несколько секунд.
Expected: игра не виснет, фигура уезжает влево и остаётся у стены, вставка не порождает посторонних действий; после отпускания управление возвращается немедленно.

- [ ] **Step 6: Коммит**

```bash
git add samples/tetris
git commit -m "feat(samples): read and dispatch terminal input"
```
