# Итерация 3: Буфер кадра и дифференциальный вывод

> Общие ограничения, команды сборки и Review Focus — в индексе плана [`2026-10-21-terminal-tetris-sample.md`](../2026-10-21-terminal-tetris-sample.md); они обязательны и для этой итерации. Соседние итерации, на которые ссылается текст, — файлы `task-NN-*.md` рядом.

**Files:**

- Modify: `samples/tetris/terminal_screen.hpp`
- Modify: `samples/tetris/terminal_screen.cpp`
- Modify: `samples/tetris/tetris_sample_app.cpp`

**Interfaces:**

- Consumes: `terminal::enter()`, `terminal::leave()`, `terminal::Size`, `terminal::querySize()`, `terminal::c_maxColumns`, `terminal::c_maxRows`.
- Produces, в пространстве имён `terminal`:

```cpp
struct ScreenCell { char32_t codepoint; uint8_t color; };
inline constexpr size_t c_outputBufferBytes = 32 * 1024;

class Screen {
public:
  void resize(int32_t columns, int32_t rows) noexcept;
  int32_t columns() const noexcept;
  int32_t rows() const noexcept;
  void clear(char32_t codepoint, uint8_t color) noexcept;
  void put(int32_t column, int32_t row, char32_t codepoint, uint8_t color) noexcept;
  void putText(int32_t column, int32_t row, toy::StringView text, uint8_t color) noexcept;
  void present() noexcept;
private:
  // два буфера по c_maxColumns * c_maxRows ячеек в статической памяти, текущий размер, байтовый буфер вывода
};
```

- [ ] **Step 1: Реализовать `Screen`**

Два буфера — статические массивы `ScreenCell[c_maxRows][c_maxColumns]`, около 153 КБ на оба; на стеке им не место. `put` игнорирует координаты вне текущего размера. `putText` декодирует UTF-8 из `toy::StringView` в codepoint'ы и кладёт их подряд, не выходя за правый край.

- [ ] **Step 2: Реализовать `present`**

Сравнить текущий буфер с предыдущим, для каждой изменившейся ячейки выдать `ESC[ряд;столбецH`, затем `ESC[38;5;Nm` если цвет отличается от цвета предыдущей выданной ячейки, затем UTF-8 глиф. Смежные изменённые ячейки одной строки объединять в прогон: позиционирование один раз на прогон. Всё складывается в байтовый буфер на `c_outputBufferBytes`; при заполнении — `write(STDOUT_FILENO, ...)` и продолжение с начала буфера. Штатный кадр — один системный вызов. В конце текущий и предыдущий буферы меняются местами.

- [ ] **Step 3: Проверить на тестовом узоре**

В `runInternal` после входа в терминал: `resize` по `querySize`, цикл на 200 кадров — на каждом `clear`, рисунок, зависящий от номера кадра (бегущая полоса и счётчик), `present`, ожидание около 16 мс через `nanosleep`. Временно, удаляется в итерации 8.

Run: собрать, запустить.
Expected: картинка движется без мерцания и без разрывов; курсор не виден; после выхода по любому символу содержимое экрана прежнее.

- [ ] **Step 4: Убедиться, что перерисовка дифференциальная**

Run: `strace -f -e trace=write ./out/build/.../tetris-sample-app 2>&1 | head -50` на Linux, на macOS — временно добавить счётчик выданных байтов и напечатать его после выхода.
Expected: в установившемся кадре в `write` уходит несколько сотен байтов, а не весь экран.

- [ ] **Step 5: Коммит**

```bash
git add samples/tetris
git commit -m "feat(samples): add differential terminal frame buffer"
```
