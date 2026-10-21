# Итерация 8: Игровой цикл, гравитация и закрепление при приземлении

> Общие ограничения, команды сборки и Review Focus — в индексе плана [`2026-10-21-terminal-tetris-sample.md`](../2026-10-21-terminal-tetris-sample.md); они обязательны и для этой итерации. Соседние итерации, на которые ссылается текст, — файлы `task-NN-*.md` рядом.

**Files:**

- Modify: `samples/tetris/tetris_core.hpp`
- Modify: `samples/tetris/tetris_core.cpp`
- Modify: `samples/tetris/terminal_input.hpp`
- Modify: `samples/tetris/terminal_input.cpp`
- Modify: `samples/tetris/tetris_sample_app.cpp`

**Interfaces:**

- Consumes: `tetris::GameState`, `tetris::activeCells`, `tetris::collides`, `tetris::popQueue`, `tetris::startGame`, `tetris::Phase`, `tetris::c_fieldRows`, `tetris::c_hiddenRows`, `tetris::view::drawLayout`, `tetris::view::drawField`, `tetris::view::drawHold`, `tetris::view::drawQueue`, `terminal::Screen`, `terminal::querySize`.
- Produces, в `tetris`:

```cpp
inline constexpr uint32_t c_simulationStepMicroseconds = 8333;
inline constexpr uint32_t c_maxFrameMicroseconds = 250000;
inline constexpr uint32_t c_maxStepsPerFrame = 8;
inline constexpr uint32_t c_linesPerLevel = 10;
inline constexpr int32_t c_maxLevel = 15;

uint32_t gravityInterval(const GameState & state) noexcept;   // микросекунды на ряд при текущем уровне
void advance(GameState & state, uint32_t elapsedMicroseconds) noexcept;
void lockPiece(GameState & state) noexcept;                   // вписывает active в field и спавнит следующую
uint64_t nowMicroseconds() noexcept;                          // в tetris_sample_app.cpp, не в ядре
```

- Produces, в `terminal`: `bool waitForInput(uint32_t timeoutMicroseconds) noexcept;` — `poll` на `STDIN_FILENO` с `POLLIN`, `true` если есть байты, `false` по таймауту. Флаги `fcntl` не меняются.

- [ ] **Step 1: Реализовать `gravityInterval`**

Формула Guideline: секунд на ряд — `(0.8 - (level - 1) * 0.007) ^ (level - 1)`. Считать в `double`, затем в микросекунды, с округлением вниз и полом в 1 мкс. `static_assert`, что на уровне 1 результат равен 1 000 000 мкс, и что на уровне 15 он меньше `c_simulationStepMicroseconds`.

- [ ] **Step 2: Реализовать `advance` и `lockPiece`**

`advance`: если `phase != playing`, возвращается сразу. Накапливает `elapsedMicroseconds` в `gravityElapsedMicroseconds`; пока накопленное не меньше `gravityInterval`, вычитает интервал и пробует сдвинуть фигуру на ряд вниз — при неудаче вызывает `lockPiece` и прекращает цикл. За один вызов может пройти несколько рядов: на уровне 15 интервал меньше шага симуляции.

`lockPiece`: вписывает четыре клетки из `activeCells(state)` в `field` их `PieceKind`; если все четыре оказались в рядах `[0, c_hiddenRows)` — `phase = gameOver`, `overReason = lockOut`. Затем `holdUsed = false`, спавн следующей фигуры через `popQueue`, сброс `gravityElapsedMicroseconds`, `lockElapsedMicroseconds`, `lockResets`, `lastActionWasRotation`, `lastKickIndex`. Очистки рядов здесь пока нет — она приходит в итерации 11.

- [ ] **Step 3: Реализовать `waitForInput`**

`struct pollfd` на `STDIN_FILENO` с `POLLIN`, таймаут в миллисекундах округлением вверх от микросекунд, минимум 0. `EINTR` повторяет опрос с остатком таймаута. Возвращает `true`, если `revents` содержит `POLLIN`.

- [ ] **Step 4: Собрать цикл в `runInternal`**

`startGame(state, makeSeed())`, затем до выхода: `waitForInput` с таймаутом, равным остатку кадра до 16 667 мкс; `nowMicroseconds()` и дельта, обрезанная `c_maxFrameMicroseconds`; аккумулятор, не более `c_maxStepsPerFrame` шагов по `c_simulationStepMicroseconds`, на каждый — `advance(state, c_simulationStepMicroseconds)`; кадр — `clear`, `drawLayout`, `drawField`, `drawHold`, `drawQueue`, `present`. Временный выход по любому прочитанному байту.

- [ ] **Step 5: Проверить гравитацию и загрузку процессора вручную**

Run: собрать, запустить, наблюдать минуту; отдельно `top -pid $(pgrep tetris-sample-app)` на macOS или `top -p` на Linux.
Expected: фигура опускается на ряд примерно раз в секунду, при приземлении закрепляется и сверху появляется следующая; столбец CPU у процесса в пределах нескольких процентов.

- [ ] **Step 6: Проверить поведение на высокой скорости**

Временно поднять `state.level` до 15 сразу после `startGame`.

Run: собрать, запустить.
Expected: фигура падает до дна за один-два кадра, закрепляется, следующая появляется сразу; цикл не зависает и не пропускает закрепления. После проверки вернуть уровень.

- [ ] **Step 7: Коммит**

```bash
git add samples/tetris
git commit -m "feat(samples): add fixed timestep loop and gravity"
```
