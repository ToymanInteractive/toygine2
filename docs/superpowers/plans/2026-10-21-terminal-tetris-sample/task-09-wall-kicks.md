# Итерация 9: Движение и SRS wall kicks

> Общие ограничения, команды сборки и Review Focus — в индексе плана [`2026-10-21-terminal-tetris-sample.md`](../2026-10-21-terminal-tetris-sample.md); они обязательны и для этой итерации. Соседние итерации, на которые ссылается текст, — файлы `task-NN-*.md` рядом.

**Files:**

- Modify: `samples/tetris/tetris_core.hpp`
- Modify: `samples/tetris/tetris_core.cpp`
- Modify: `samples/tetris/tetris_sample_app.cpp`

**Interfaces:**

- Consumes: `tetris::GameState`, `tetris::PlacedPiece`, `tetris::Rotation`, `tetris::collides`, `tetris::pieceCells`, `tetris::lockPiece`.
- Produces, в `tetris`:

```cpp
struct Kick { int8_t dx; int8_t dy; };   // dy положителен вверх, как в таблицах спеки

std::span<const Kick, 5> kickTable(PieceKind kind, Rotation from, Rotation to) noexcept;
bool tryShift(GameState & state, int32_t dx) noexcept;
bool tryRotate(GameState & state, Rotation direction) noexcept;
```

- [ ] **Step 1: Задать таблицы wall kicks**

Две `constexpr`-таблицы по восемь переходов на пять `Kick` каждая — JLSTZ и I, значения побайтово из таблиц спеки. Для O `kickTable` возвращает таблицу из пяти `(0, 0)`, а `tryRotate` меняет только `active.rotation` и возвращает `true` без проверки столкновений: квадрат 2 × 2 поворотом инвариантен. `static_assert` на размер обеих таблиц — восемь переходов, и на то, что первая проба каждого перехода равна `(0, 0)`.

- [ ] **Step 2: Реализовать `tryShift`**

Копия `active` со сдвинутым `column`; если `collides` — `false`. Иначе применить, `lastActionWasRotation = false`, `lastKickIndex = 0`, вернуть `true`. Счётчик сбросов lock delay обновляется по правилу из итерации 11, здесь пока не трогается.

- [ ] **Step 3: Реализовать `tryRotate`**

Вычислить целевое `Rotation`: из `spawn` по часовой — `clockwise`, против — `counterClockwise`; из `clockwise` по часовой — `doubleTurn`, против — `spawn`; и так далее, полный цикл из восьми переходов. Взять `kickTable`, для каждой из пяти проб построить кандидата со сдвинутыми `column` и `row` — знак `dy` инвертируется, потому что ряды поля идут сверху вниз, — и взять первую, где `collides` отрицателен. Применить, запомнить `lastActionWasRotation = true` и `lastKickIndex` в номере сработавшей пробы. Если ни одна проба не прошла — `false`, состояние не меняется.

- [ ] **Step 4: Закрепить переходы `static_assert`**

Для каждого из восьми переходов JLSTZ и каждого из восьми I проверить, что `kickTable` возвращает ровно те пять значений, что в спеке. Отдельно — что поворот на четыре шага по часовой из любого состояния возвращает то же состояние.

Run: собрать.
Expected: компиляция проходит.

- [ ] **Step 5: Проверить kick вручную через временные привязки**

Временно в цикле из итерации 8: байт `a` вызывает `tryShift(state, -1)`, `d` — `tryShift(state, 1)`, `w` — `tryRotate` по часовой, `s` — против. Убрать в итерации 10.

Run: собрать, запустить, прижать фигуру к левой стене и повернуть I из горизонтального в вертикальное у самого дна.
Expected: сдвиг упирается в стену и не проходит сквозь неё; поворот I у стены срабатывает за счёт второй или третьей пробы; поворот T в колодце глубиной в одну клетку срабатывает за счёт пятой пробы.

- [ ] **Step 6: Коммит**

```bash
git add samples/tetris
git commit -m "feat(samples): add SRS rotation with wall kicks"
```
