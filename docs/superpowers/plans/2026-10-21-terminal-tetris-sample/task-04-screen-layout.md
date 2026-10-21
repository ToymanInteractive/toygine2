# Итерация 4: Раскладка экрана

> Общие ограничения, команды сборки и Review Focus — в индексе плана [`2026-10-21-terminal-tetris-sample.md`](../2026-10-21-terminal-tetris-sample.md); они обязательны и для этой итерации. Соседние итерации, на которые ссылается текст, — файлы `task-NN-*.md` рядом.

**Files:**

- Modify: `samples/tetris/terminal_screen.hpp`
- Modify: `samples/tetris/terminal_screen.cpp`
- Modify: `samples/tetris/tetris_view.hpp`
- Modify: `samples/tetris/tetris_view.cpp`
- Modify: `samples/tetris/tetris_sample_app.cpp`

**Interfaces:**

- Consumes: `terminal::Screen`, `terminal::c_minColumns`, `terminal::c_minRows`.
- Produces, в `terminal`: `void drawBox(Screen & screen, int32_t column, int32_t row, int32_t columns, int32_t rows, uint8_t color) noexcept;` — рамка box-drawing символами `╔ ═ ╗ ║ ╚ ╝`; внутренняя перемычка `╟ ─ ╢` — отдельным вызовом `drawSeparator(Screen &, int32_t column, int32_t row, int32_t columns, uint8_t color)`.
- Produces, в `tetris::view`:

```cpp
inline constexpr int32_t c_cellColumns = 2;      // одна игровая клетка — два столбца терминала
inline constexpr int32_t c_marginLeft = 1;
inline constexpr int32_t c_panelGap = 2;
inline constexpr int32_t c_holdColumn = 1;       // 1 + 0
inline constexpr int32_t c_fieldColumn = 13;     // 1 + 10 + 2
inline constexpr int32_t c_sideColumn = 37;      // 13 + 22 + 2
inline constexpr int32_t c_contentRows = 24;

inline constexpr uint8_t c_colorFrame = 244;
inline constexpr uint8_t c_colorText = 252;
inline constexpr uint8_t c_colorGhost = 240;

void drawLayout(terminal::Screen & screen) noexcept;
```

- [ ] **Step 1: Добавить `drawBox` и `drawSeparator` в `terminal`**

Оба принимают экран, начало, размер и цвет; координаты вне экрана обрезаются.

- [ ] **Step 2: Реализовать `drawLayout`**

Рамка hold — `drawBox(screen, c_holdColumn, 0, 10, 6, c_colorFrame)`, подпись `HOLD` по центру в ряду 1, `drawSeparator` в ряду 2, область предпросмотра — ряды 3–4, колонки 2–9.

Рамка поля — `drawBox(screen, c_fieldColumn, 0, 22, 22, c_colorFrame)`; внутренность начинается в `(c_fieldColumn + 1, 1)`.

Правая панель — `drawBox(screen, c_sideColumn, 0, 10, 22, c_colorFrame)`, подпись `NEXT` по центру в ряду 1, `drawSeparator` в ряду 2, пять слотов предпросмотра в рядах 3–4, 5–6, 7–8, 9–10, 11–12, `drawSeparator` в ряду 13, затем пары подпись-значение: `SCORE` в ряду 14 и значение в 15, `LEVEL` в 16 и 17, `LINES` в 18 и 19. Подписи цветом `c_colorText`, значения тем же цветом, выравнивание значений по правому краю внутренности.

Подсказки под полем, цвет `c_colorFrame`, колонка `c_marginLeft`: ряд 22 — `←/→ шаг · ↑ поворот · Z обратно · ↓ мягко`, ряд 23 — `пробел — сброс · C hold · P пауза · Q выход`.

- [ ] **Step 3: Проверить статически, что содержимое влезает в минимальный терминал**

```cpp
static_assert(c_sideColumn + 10 <= terminal::c_minColumns, "layout must fit the minimum terminal width");
static_assert(c_contentRows <= terminal::c_minRows, "layout must fit the minimum terminal height");
```

Run: собрать.
Expected: компиляция проходит; при нарушении любого из двух `static_assert` сборка падает с этим сообщением.

- [ ] **Step 4: Показать раскладку на экране**

Временно в `runInternal`: `clear`, `drawLayout`, `present`, ожидание нажатия, выход.

Run: собрать, запустить в терминале 80 × 24.
Expected: три панели и две строки подсказок, как в макете спеки, ничего не обрезано справа и снизу.

- [ ] **Step 5: Коммит**

```bash
git add samples/tetris
git commit -m "feat(samples): lay out tetris screen panels"
```
