# Итерация 11: Lock delay, очистка рядов и завершение партии

> Общие ограничения, команды сборки и Review Focus — в индексе плана [`2026-10-21-terminal-tetris-sample.md`](../2026-10-21-terminal-tetris-sample.md); они обязательны и для этой итерации. Соседние итерации, на которые ссылается текст, — файлы `task-NN-*.md` рядом.

**Files:**

- Modify: `samples/tetris/tetris_core.hpp`
- Modify: `samples/tetris/tetris_core.cpp`
- Modify: `samples/tetris/tetris_sample_app.cpp`

**Interfaces:**

- Consumes: `tetris::GameState`, `tetris::collides`, `tetris::advance`, `tetris::lockPiece`, `tetris::tryShift`, `tetris::tryRotate`, `tetris::c_hiddenRows`, `tetris::c_fieldColumns`, `tetris::c_fieldRows`, `tetris::c_linesPerLevel`, `tetris::c_maxLevel`.
- Produces, в `tetris`:

```cpp
inline constexpr uint32_t c_lockDelayMicroseconds = 500000;
inline constexpr uint32_t c_maxLockResets = 15;

struct ClearResult { uint32_t lines; bool perfectClear; };

bool isGrounded(const GameState & state) noexcept;             // фигура не может сдвинуться на ряд вниз
void onSuccessfulMove(GameState & state) noexcept;             // сброс lock delay с учётом лимита
ClearResult clearLines(GameState & state) noexcept;            // вызывается из lockPiece после вписывания
```

`lockPiece` сохраняет `ClearResult` в новые поля `lastClearLines` и `lastClearWasPerfect` типа `uint32_t` и `bool` в `GameState` — скоринг из итерации 12 прочтёт их.

- [ ] **Step 1: Реализовать `isGrounded`**

Копия `active` с `row + 1`; `collides` по ней — значит фигура на опоре.

- [ ] **Step 2: Реализовать `onSuccessfulMove` и подключить к сдвигам и поворотам**

Если `isGrounded` и `lockResets < c_maxLockResets`: `lockElapsedMicroseconds = 0`, `lockResets += 1`. Вызывается из `tryShift` и `tryRotate` после успешного применения. Если фигура оказалась на новом для себя нижнем ряду — `lowestRow` обновляется и `lockResets` сбрасывается в 0; именно это не даёт бесконечному вращению на краю уступа.

- [ ] **Step 3: Довести lock delay в `advance`**

Когда `isGrounded`, `advance` накапливает время в `lockElapsedMicroseconds` и по достижении `c_lockDelayMicroseconds` вызывает `lockPiece`. Когда фигура не на опоре, `lockElapsedMicroseconds` остаётся нулём. Гравитационный счётчик при этом продолжает идти.

- [ ] **Step 4: Реализовать `clearLines`**

Проход снизу вверх: полностью заполненный ряд — все `c_fieldColumns` клеток `filled` — удаляется, все ряды выше сдвигаются на один вниз, самый верхний ряд обнуляется. Возвращает число удалённых рядов и `perfectClear` — поле целиком пусто после удаления. Сохраняет оба значения в `lastClearLines` и `lastClearWasPerfect`, увеличивает `state.lines` и пересчитывает `state.level` как `min(c_maxLevel, 1 + lines / c_linesPerLevel)`.

- [ ] **Step 5: Довести block out**

Спавн следующей фигуры в `lockPiece` уже ставит `blockOut`. Проверить, что `phase == gameOver` останавливает и `advance`, и `dispatchKey` кроме `restart` и `quit`.

- [ ] **Step 6: Проверить вручную**

Run: собрать, запустить, заполнить ряд до конца; отдельно положить фигуру на пол и не трогать её.
Expected: заполненный ряд исчезает, клетки сверху опускаются на его место; фигура на полу закрепляется примерно через полсекунды; сдвиг или поворот фигуры на полу откладывает закрепление, но после 15 таких откладываний она всё равно закрепляется; стакан высотой в 21 ряд завершает партию с `blockOut`.

- [ ] **Step 7: Проверить очистку рядов на 20G вручную**

Временно поднять `state.level` до 15 сразу после `startGame` и заранее заполнить нижний ряд поля, оставив одну пустую клетку под столбцом спавна.

Run: собрать, запустить.
Expected: за один вызов `advance` фигура проходит несколько рядов, вписывается в оставленную клетку, полный ряд удаляется сразу, а не на следующем кадре, и цикл не зависает. После проверки вернуть уровень.

- [ ] **Step 8: Коммит**

```bash
git add samples/tetris
git commit -m "feat(samples): add lock delay, line clearing and game over"
```
