# Итерация 13: Ghost, hard drop, soft drop и DAS/ARR

> Общие ограничения, команды сборки и Review Focus — в индексе плана [`2026-10-21-terminal-tetris-sample.md`](../2026-10-21-terminal-tetris-sample.md); они обязательны и для этой итерации. Соседние итерации, на которые ссылается текст, — файлы `task-NN-*.md` рядом.

**Files:**

- Modify: `samples/tetris/tetris_core.hpp`
- Modify: `samples/tetris/tetris_core.cpp`
- Modify: `samples/tetris/terminal_input.hpp`
- Modify: `samples/tetris/terminal_input.cpp`
- Modify: `samples/tetris/tetris_view.cpp`
- Modify: `samples/tetris/tetris_sample_app.cpp`

**Interfaces:**

- Consumes: `tetris::GameState`, `tetris::collides`, `tetris::gravityInterval`, `tetris::isGrounded`, `tetris::tryShift`, `tetris::tryRotate`, `tetris::lockPiece`, `tetris::advance`, `terminal::Key`, `terminal::readKeys`, `terminal::waitForInput`.
- Produces, в `tetris`:

```cpp
inline constexpr uint32_t c_softDropFloorMicroseconds = 1000;

uint32_t softDropInterval(const GameState & state) noexcept;   // gravityInterval / 20, пол 1000 мкс
void refreshGhost(GameState & state) noexcept;
void softDrop(GameState & state) noexcept;                     // один ряд вниз, 1 очко
void hardDrop(GameState & state) noexcept;                     // до упора, 2 очка за ряд, lockPiece
```

- Produces, в `terminal`:

```cpp
enum class Action : uint8_t {
  none, shiftLeft, shiftRight, slideLeft, slideRight, softDropOne,
  rotateClockwise, rotateCounterClockwise, hardDrop, hold, pause, restart, quit
};

enum class Direction : uint8_t { none, left, right };

struct ActionBatch { std::array<Action, 8> items; uint32_t count; };

struct RepeatState {
  Direction direction;
  uint32_t dasElapsedMicroseconds;
  bool dasFired;
  uint32_t softDropCooldownMicroseconds;
};

inline constexpr uint32_t c_dasMicroseconds = 100000;

void resetRepeat(RepeatState & state) noexcept;
ActionBatch actionsForKey(RepeatState & state, terminal::Key key, uint32_t softDropIntervalMicroseconds) noexcept;
ActionBatch advanceRepeat(RepeatState & state, uint32_t elapsedMicroseconds, uint32_t softDropIntervalMicroseconds) noexcept;
```

- [ ] **Step 1: Реализовать `refreshGhost` и `softDropInterval`**

`refreshGhost` копирует `active` и опускает `row`, пока `collides` не станет положительным; последнее свободное положение — в `state.ghost`. Вызывается из `tryShift`, `tryRotate`, `softDrop` и из шага гравитации в `advance`. `softDropInterval` — `gravityInterval(state) / 20` с полом `c_softDropFloorMicroseconds`.

- [ ] **Step 2: Реализовать `softDrop` и `hardDrop`**

`softDrop`: если фигура не на опоре — сдвинуть на ряд вниз, `score += 1`, `refreshGhost`; если на опоре — ничего не делать, закрепление делает lock delay. `hardDrop`: опустить до упора, считая пройденные ряды, `score += 2 * ряды`, `refreshGhost`, затем `lockPiece`. Оба — только при `phase == playing`.

- [ ] **Step 3: Реализовать `resetRepeat`, `actionsForKey`, `advanceRepeat`**

`actionsForKey` на `left`/`right`: записывает `shiftLeft` или `shiftRight`, ставит `direction`, `dasElapsedMicroseconds = 0`, `dasFired = false`. На `up` — `rotateClockwise`, на `rotateCounterClockwise` — `rotateCounterClockwise`, на `space` — `hardDrop`, на `hold` — `hold`, на `pause` — `pause`, на `restart` — `restart`, на `quit` — `quit`; направление при этом не сбрасывается. На `down`: если `softDropCooldownMicroseconds > 0` — пустой батч, иначе `softDropOne` и `softDropCooldownMicroseconds = softDropIntervalMicroseconds`.

`advanceRepeat`: вычитает `elapsedMicroseconds` из `softDropCooldownMicroseconds` с насыщением в нуль. Если `direction != none` и `dasFired == false`, накапливает `dasElapsedMicroseconds`; по достижении `c_dasMicroseconds` ставит `dasFired = true` и записывает `slideLeft` или `slideRight`. После `dasFired` повтор направления не продолжается: при ARR = 0 фигура уже у стены.

- [ ] **Step 4: Перевести цикл на действия**

В `tetris_sample_app.cpp`: `RepeatState`, на каждое прочитанное нажатие — `actionsForKey`, на каждый кадр — `advanceRepeat` с той же дельтой, что уходит в `advance`. Батч применяется через файловую функцию `dispatchAction(tetris::GameState &, terminal::Action)`, где `slideLeft` и `slideRight` вызывают `tryShift` в цикле до первого `false`, а `pause` по-прежнему игнорируется — `togglePause` появляется только в итерации 14. `dispatchKey` из итерации 10 удаляется.

- [ ] **Step 5: Нарисовать ghost**

В `drawField` после отрисовки закреплённых клеток и до активной фигуры: клетки `state.ghost` глифом `░` цветом `c_colorGhost`, только в видимых рядах и только там, где клетка поля пуста.

- [ ] **Step 6: Проверить DAS и ARR вручную**

Run: собрать, зажать стрелку влево на пустом поле.
Expected: один сдвиг сразу, пауза около 100 мс, затем фигура одним прыжком уезжает к стене; короткое нажатие сдвигает ровно на одну клетку; нажатие вправо перехватывает повтор.

- [ ] **Step 7: Проверить soft drop и ghost вручную**

Run: нажать ↓ один раз, затем зажать ↓, затем нажать пробел.
Expected: одиночное нажатие опускает ровно на один ряд; удержание опускает со скоростью примерно в 20 раз быстрее гравитации; призрак всегда стоит там, куда упадёт фигура, и совпадает с местом приземления после пробела.

- [ ] **Step 8: Коммит**

```bash
git add samples/tetris
git commit -m "feat(samples): add ghost piece, drops and DAS repeat"
```
