# Итерация 12: Скоринг, combo, back-to-back и T-spin

> Общие ограничения, команды сборки и Review Focus — в индексе плана [`2026-10-21-terminal-tetris-sample.md`](../2026-10-21-terminal-tetris-sample.md); они обязательны и для этой итерации. Соседние итерации, на которые ссылается текст, — файлы `task-NN-*.md` рядом.

**Files:**

- Modify: `samples/tetris/tetris_core.hpp`
- Modify: `samples/tetris/tetris_core.cpp`
- Modify: `samples/tetris/tetris_view.hpp`
- Modify: `samples/tetris/tetris_view.cpp`
- Modify: `samples/tetris/tetris_sample_app.cpp`

**Interfaces:**

- Consumes: `tetris::GameState`, `tetris::ClearResult`, `tetris::PieceKind`, `tetris::Rotation`, `tetris::c_fieldColumns`, `tetris::c_fieldRows`, `tetris::c_hiddenRows`, `tetris::view::c_colorText`.
- Produces, в `tetris`:

```cpp
enum class TSpin : uint8_t { none, mini, full };

TSpin detectTSpin(const GameState & state) noexcept;   // вызывается до clearLines, по состоянию на момент закрепления
void applyScore(GameState & state, ClearResult cleared, TSpin spin) noexcept;
```

- Produces, в `tetris::view`:

```cpp
void writeNumber(std::span<char> buffer, uint64_t value) noexcept;   // std::to_chars, завершает нулём
void drawCounters(terminal::Screen & screen, const GameState & state) noexcept;
```

- [ ] **Step 1: Реализовать `detectTSpin`**

Только если `active.kind == PieceKind::t` и `lastActionWasRotation`. Центр T — `active.column + 1`, `active.row + 1`. Проверить четыре диагонали вокруг центра; клетка вне поля считается занятой. Занятых меньше трёх — `none`. Обе «передние» диагонали заняты — `full`; передние определяются состоянием поворота: у `spawn` это две верхние, у `clockwise` — две правые, у `doubleTurn` — две нижние, у `counterClockwise` — две левые. Передняя только одна — `mini`, который становится `full`, если `lastKickIndex == 4`.

- [ ] **Step 2: Реализовать `applyScore`**

База по таблице спеки, умножается на `state.level`. При `backToBack` и сложном очищении база умножается на 1.5 — целочисленно, `(base * 3) / 2`. Сложное очищение — `cleared.lines == 4` или `spin != TSpin::none && cleared.lines > 0`. Бонус perfect clear берётся из таблицы как есть и прибавляется после умножения; для `cleared.lines == 4 && state.backToBack` применяется значение 3200, а не 2000. Combo: при `cleared.lines > 0` — `combo += 1` и прибавка `50 * combo * level` когда `combo > 0`; при `cleared.lines == 0` — `combo = -1`. Флаг `backToBack` ставится сложным очищением и снимается простым; закрепление без очистки его не трогает.

- [ ] **Step 3: Подключить к `lockPiece`**

В `lockPiece` после вписывания фигуры и до `clearLines` вызвать `detectTSpin`, затем `clearLines`, затем `applyScore`. Порядок существенен: T-spin проверяется по клеткам вокруг центра, которые очистка рядов уничтожит.

- [ ] **Step 4: Закрепить скоринг `static_assert`**

`constexpr`-проверки на собранном вручную `GameState`: single на уровне 1 даёт 100, tetris на уровне 3 даёт 2400, тот же tetris при поднятом `backToBack` — 3600, T-spin double на уровне 2 — 2400, combo на втором очищении подряд на уровне 1 прибавляет ровно 50.

Run: собрать.
Expected: компиляция проходит.

- [ ] **Step 5: Вывести счёт на панель**

Реализовать `drawCounters` — `SCORE`, `LEVEL`, `LINES` в рядах 15, 17 и 19 правой панели, правым краем, цветом `c_colorText`. Число в строку — `writeNumber`: `std::to_chars` из `<charconv>` в переданный `std::span<char>`, затем нуль по адресу `result.ptr`; вызывающая сторона передаёт стек-буфер `std::array<char, 16>` и отдаёт его в `putText` как `toy::StringView` из указателя. `std::format` и `toy::String` не используются: первое тянет локаль, второе — динамическое выделение. Подключить вызов `drawCounters` в кадр в `tetris_sample_app.cpp`.

- [ ] **Step 6: Проверить вручную**

Run: собрать, сыграть партию до двух-трёх очищений подряд и до одного Tetris.
Expected: числа на панели совпадают с ручным подсчётом по таблице; combo растёт на подряд идущих очищениях и обнуляется после холостого закрепления; четыре ряда за раз дают заметно больше, чем четыре одиночных.

- [ ] **Step 7: Коммит**

```bash
git add samples/tetris
git commit -m "feat(samples): add guideline scoring with t-spins and combos"
```
