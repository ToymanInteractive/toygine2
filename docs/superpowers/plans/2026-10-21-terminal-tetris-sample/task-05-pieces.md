# Итерация 5: Фигуры, повороты и столкновения

> Общие ограничения, команды сборки и Review Focus — в индексе плана [`2026-10-21-terminal-tetris-sample.md`](../2026-10-21-terminal-tetris-sample.md); они обязательны и для этой итерации. Соседние итерации, на которые ссылается текст, — файлы `task-NN-*.md` рядом.

**Files:**

- Modify: `samples/tetris/tetris_core.hpp`
- Modify: `samples/tetris/tetris_core.cpp`
- Modify: `samples/tetris/tetris_sample_app.cpp`

**Interfaces:**

- Consumes: ничего, кроме `<array>`, `<cstdint>`, `<span>`; временная проверка из Step 4 дополнительно берёт `terminal::Screen` и `terminal::drawBox`.
- Produces, в `tetris`:

```cpp
enum class PieceKind : uint8_t { i, j, l, o, s, t, z };
enum class Rotation : uint8_t { spawn, clockwise, doubleTurn, counterClockwise };

struct CellOffset { int8_t column; int8_t row; };

inline constexpr int32_t c_fieldColumns = 10;
inline constexpr int32_t c_fieldRows = 24;
inline constexpr int32_t c_hiddenRows = 4;
inline constexpr int32_t c_visibleRows = 20;

std::span<const CellOffset, 4> pieceCells(PieceKind kind, Rotation rotation) noexcept;
int32_t boundingBoxSize(PieceKind kind) noexcept;   // 4 для I, 3 для J/L/S/T/Z, 2 для O
bool fitsInField(const std::array<CellOffset, 4> & cells, const CellOffset & origin) noexcept;
```

`Rotation::doubleTurn`, а не `double_` из спеки: `double` — ключевое слово, а завершающее подчёркивание `AGENTS.md` оставляет приватным членам.

- [ ] **Step 1: Задать таблицы форм**

Одна `constexpr` таблица на фигуру, четыре состояния по четыре `CellOffset`, в координатах ограничивающего квадрата, ряд 0 — верхний. Состояние `spawn` — ровно из таблицы спеки; остальные три получаются поворотом по часовой по правилу `(x, y) → (n - 1 - y, x)` для квадрата размера `n`. Таблицы собираются `consteval`-функцией из состояния `spawn`, а не выписываются руками.

- [ ] **Step 2: Закрепить формы и повороты `static_assert`**

Проверить, что поворот на четыре шага возвращает исходную форму для всех семи фигур, что все четыре клетки каждого состояния различны, что все координаты в пределах `[0, n)`, и что состояние `spawn` каждой фигуры совпадает с таблицей спеки побайтово. Отдельно — что каждая форма в состоянии `spawn` умещается в 4 клетки в ширину и 2 в высоту: на этом держится размер слота предпросмотра.

Run: собрать.
Expected: сборка проходит; намеренно испорченная таблица роняет её с сообщением из `static_assert`.

- [ ] **Step 3: Реализовать `pieceCells`, `boundingBoxSize`, `fitsInField`**

`pieceCells` возвращает `std::span` на статическую таблицу. `fitsInField` проверяет, что все четыре клетки, сложенные с `origin`, попадают в `column ∈ [0, c_fieldColumns)` и `row ∈ [0, c_fieldRows)`.

- [ ] **Step 4: Показать все фигуры и повороты на экране**

Временно в `runInternal`: цикл по семи фигурам и четырём поворотам, каждый рисуется глифом `█` внутри своей рамки на поле, с ожиданием нажатия между кадрами.

Run: собрать, запустить, пролистать все 28 сочетаний.
Expected: формы совпадают с каноническими SRS, поворот идёт по часовой стрелке внутри своего квадрата.

- [ ] **Step 5: Коммит**

```bash
git add samples/tetris
git commit -m "feat(samples): add tetris piece shapes and rotation tables"
```
