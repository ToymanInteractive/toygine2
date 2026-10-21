# Итерация 6: Состояние игры, поле и спавн

> Общие ограничения, команды сборки и Review Focus — в индексе плана [`2026-10-21-terminal-tetris-sample.md`](../2026-10-21-terminal-tetris-sample.md); они обязательны и для этой итерации. Соседние итерации, на которые ссылается текст, — файлы `task-NN-*.md` рядом.

**Files:**

- Modify: `samples/tetris/tetris_core.hpp`
- Modify: `samples/tetris/tetris_core.cpp`
- Modify: `samples/tetris/tetris_view.hpp`
- Modify: `samples/tetris/tetris_view.cpp`
- Modify: `samples/tetris/tetris_sample_app.cpp`

**Interfaces:**

- Consumes: `tetris::PieceKind`, `tetris::Rotation`, `tetris::CellOffset`, `tetris::pieceCells`, `tetris::fitsInField`, `tetris::c_fieldColumns`, `tetris::c_fieldRows`, `tetris::c_hiddenRows`, `tetris::c_visibleRows`, `tetris::view::drawLayout`.
- Produces, в `tetris`:

```cpp
enum class Phase : uint8_t { playing, paused, gameOver };
enum class GameOverReason : uint8_t { none, blockOut, lockOut };

inline constexpr int32_t c_queueSize = 5;

struct Cell { PieceKind kind; bool filled; };

struct PlacedPiece {
  PieceKind kind;
  Rotation rotation;
  int32_t column;   // левый столбец ограничивающего квадрата в координатах поля
  int32_t row;      // верхний ряд ограничивающего квадрата в координатах поля
};

struct GameState {
  std::array<Cell, c_fieldRows * c_fieldColumns> field;
  PlacedPiece active;
  PlacedPiece ghost;
  std::array<PieceKind, c_queueSize> queue;
  std::array<PieceKind, 7> bag;
  uint32_t bagIndex;
  uint64_t seed;
  PieceKind holdPiece;
  bool holdFilled;
  bool holdUsed;
  uint64_t score;
  uint32_t level;
  uint32_t lines;
  int32_t combo;
  bool backToBack;
  uint32_t gravityElapsedMicroseconds;
  uint32_t lockElapsedMicroseconds;
  uint32_t lockResets;
  int32_t lowestRow;
  bool lastActionWasRotation;
  uint8_t lastKickIndex;
  Phase phase;
  GameOverReason overReason;
};

inline constexpr uint64_t splitmix64(uint64_t & state) noexcept;  // тело прямо в объявлении

void startGame(GameState & state, uint64_t seed) noexcept;
bool collides(const GameState & state, const PlacedPiece & piece) noexcept;
std::span<const CellOffset, 4> activeCells(const GameState & state) noexcept;
```

- Produces, в `tetris::view`: `void drawField(terminal::Screen & screen, const GameState & state) noexcept;` и `uint8_t colorOf(PieceKind kind) noexcept;`
- Produces, в `tetris_sample_app.cpp`: `uint64_t makeSeed() noexcept;`

- [ ] **Step 1: Определить типы и константы**

Всё из блока Produces. `Cell` по умолчанию — `{PieceKind::i, false}`; `kind` у пустой клетки не читается. `splitmix64` объявляется в `tetris_core.hpp` с телом прямо в объявлении: он нужен и ядру в итерации 7, и слою приложения здесь, поэтому файловым в одном из двух `.cpp` быть не может.

- [ ] **Step 2: Реализовать `makeSeed` в `tetris_sample_app.cpp`**

Наносекунды `clock_gettime(CLOCK_MONOTONIC)`, смешанные через `tetris::splitmix64` с `pid()` от `ConsoleApplication` и с адресом локальной переменной. `std::random_device` не используется: он может бросить исключение, а исключения выключены.

- [ ] **Step 3: Реализовать `collides` и спавн**

`collides` складывает `pieceCells(piece.kind, piece.rotation)` с `{piece.column, piece.row}` и проверяет каждую клетку на `fitsInField` и на занятость в `state.field`. Спавн: ограничивающий квадрат верхним рядом в 2, левым столбцом в 3 для I/J/L/S/T/Z и в 4 для O; затем один немедленный шаг вниз, если `collides` после него отрицателен. Если фигура не помещается уже в исходной позиции — `phase = gameOver`, `overReason = blockOut`. `lastActionWasRotation = false`, `lastKickIndex = 0`, `lockElapsedMicroseconds = 0`, `lockResets = 0`, `lowestRow` — нижний ряд занятой фигуры.

- [ ] **Step 4: Реализовать `startGame`**

Обнуляет поле и счётчики, `level = 1`, `lines = 0`, `score = 0`, `combo = -1`, `backToBack = false`, `phase = playing`, `overReason = none`, `holdFilled = false`, `holdUsed = false`, `seed` сохраняется. Заполняет мешок и очередь — заглушкой из семи подряд идущих `PieceKind`, настоящий 7-bag приходит в итерации 7, — и спавнит первую фигуру.

- [ ] **Step 5: Реализовать `drawField` и `colorOf`**

`colorOf` — таблица из спеки: I 51, J 33, L 208, O 226, S 46, T 141, Z 196. `drawField` проходит видимые ряды `c_hiddenRows .. c_fieldRows - 1` и столбцы `0 .. c_fieldColumns - 1`, кладёт `█` в две соседние колонки для каждой занятой клетки; начало внутренности поля — `(c_fieldColumn + 1, 1)`, клетка `(column, row)` попадает в `(c_fieldColumn + 1 + column * c_cellColumns, 1 + row - c_hiddenRows)`. Поверх тем же способом рисуется активная фигура — её клетки даёт `activeCells(state)`.

- [ ] **Step 6: Показать поле с фигурой**

Временно в `runInternal`: `startGame(state, makeSeed())`, кадр с `drawLayout` и `drawField`, ожидание нажатия, выход.

Run: собрать, запустить.
Expected: в верхней части поля видна одна фигура в состоянии `spawn`, смещённая на один ряд вниз от самого верха; счётчики пустые.

- [ ] **Step 7: Коммит**

```bash
git add samples/tetris
git commit -m "feat(samples): add tetris game state, field and spawning"
```
