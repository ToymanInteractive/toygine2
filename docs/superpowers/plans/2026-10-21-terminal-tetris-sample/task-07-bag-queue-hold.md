# Итерация 7: Мешок, очередь NEXT и hold

> Общие ограничения, команды сборки и Review Focus — в индексе плана [`2026-10-21-terminal-tetris-sample.md`](../2026-10-21-terminal-tetris-sample.md); они обязательны и для этой итерации. Соседние итерации, на которые ссылается текст, — файлы `task-NN-*.md` рядом.

**Files:**

- Modify: `samples/tetris/tetris_core.hpp`
- Modify: `samples/tetris/tetris_core.cpp`
- Modify: `samples/tetris/tetris_view.hpp`
- Modify: `samples/tetris/tetris_view.cpp`
- Modify: `samples/tetris/tetris_sample_app.cpp`

**Interfaces:**

- Consumes: `tetris::GameState`, `tetris::PieceKind`, `tetris::pieceCells`, `tetris::collides`, `tetris::splitmix64`, `tetris::c_queueSize`, `tetris::view::colorOf`, `tetris::view::c_holdColumn`, `tetris::view::c_sideColumn`.
- Produces, в `tetris`:

```cpp
PieceKind nextFromBag(GameState & state) noexcept;   // тянет из мешка, пополняет мешок по исчерпании
void refillBag(GameState & state) noexcept;          // Fisher-Yates на splitmix64 от state.seed
PieceKind popQueue(GameState & state) noexcept;      // голова очереди в state.queue, хвост дозаполняется
void swapHold(GameState & state) noexcept;
```

- Produces, в `tetris::view`: `void drawHold(terminal::Screen & screen, const GameState & state) noexcept;` и `void drawQueue(terminal::Screen & screen, const GameState & state) noexcept;`

- [ ] **Step 1: Реализовать `refillBag`**

`refillBag` кладёт в `state.bag` все семь `PieceKind` по порядку и перемешивает Fisher-Yates: для `i` от 6 до 1 обмен `bag[i]` с `bag[next % (i + 1)]`, где `next` — очередной вывод `tetris::splitmix64` от `state.seed`. `bagIndex = 0`. `<random>` не используется.

- [ ] **Step 2: Реализовать `nextFromBag` и `popQueue`**

`nextFromBag`: при `bagIndex == 7` вызывает `refillBag`, возвращает `bag[bagIndex++]`. `popQueue`: возвращает `queue[0]`, сдвигает оставшиеся четыре влево, в хвост кладёт `nextFromBag`.

- [ ] **Step 3: Перевести спавн на очередь и подключить hold**

Спавн из итерации 6 берёт фигуру через `popQueue`, а не из заглушки. `startGame` заполняет очередь пятью вызовами `nextFromBag` после `refillBag`. `swapHold`: если `holdUsed` или `phase != playing` — ничего не делает; иначе меняет `active.kind` с `holdPiece` (при `holdFilled == false` берёт из очереди через `popQueue`), ставит `holdFilled = true`, `holdUsed = true`, возвращает фигуру в состояние `spawn` и позицию спавна и проверяет столкновение — при `blockOut` завершает игру.

- [ ] **Step 4: Закрепить свойство мешка `static_assert`**

`constexpr`-проверка на `GameState`, собранном в постоянном выражении: за семь подряд вызовов `nextFromBag` каждый из семи `PieceKind` встречается ровно один раз, и на следующих семи — снова ровно один раз.

Run: собрать.
Expected: компиляция проходит.

- [ ] **Step 5: Реализовать `drawHold` и `drawQueue`**

Обе рисуют фигуру в состоянии `spawn` глифом `█` в цвете `colorOf`, центрируя по горизонтали и вертикали внутри области 8 колонок на 2 ряда. Hold — область `(c_holdColumn + 1, 3)`; если `holdFilled == false`, область остаётся пустой. Очередь — пять областей с началом в `(c_sideColumn + 1, 3)` и шагом 2 ряда.

- [ ] **Step 6: Проверить вручную**

Временно вызвать `swapHold` и несколько `popQueue` из `runInternal` и показать кадр.

Run: собрать, запустить.
Expected: в NEXT пять различных фигур, после `swapHold` в HOLD появилась прежняя активная, повторный `swapHold` до закрепления ничего не меняет.

- [ ] **Step 7: Коммит**

```bash
git add samples/tetris
git commit -m "feat(samples): add 7-bag queue and hold piece"
```
