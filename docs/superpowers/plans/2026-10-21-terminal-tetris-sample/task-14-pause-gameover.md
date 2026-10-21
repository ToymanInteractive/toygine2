# Итерация 14: Пауза, game over, рестарт и изменение размера терминала

> Общие ограничения, команды сборки и Review Focus — в индексе плана [`2026-10-21-terminal-tetris-sample.md`](../2026-10-21-terminal-tetris-sample.md); они обязательны и для этой итерации. Соседние итерации, на которые ссылается текст, — файлы `task-NN-*.md` рядом.

**Files:**

- Modify: `samples/tetris/tetris_core.hpp`
- Modify: `samples/tetris/tetris_core.cpp`
- Modify: `samples/tetris/terminal_screen.hpp`
- Modify: `samples/tetris/terminal_screen.cpp`
- Modify: `samples/tetris/tetris_view.hpp`
- Modify: `samples/tetris/tetris_view.cpp`
- Modify: `samples/tetris/tetris_sample_app.cpp`

**Interfaces:**

- Consumes: всё вышеперечисленное.
- Produces, в `tetris`: `void togglePause(GameState & state) noexcept;`
- Produces, в `terminal`: `bool installResizeHandler() noexcept;` — `sigaction` на `SIGWINCH`, выставляющий атомарный файловый флаг; `bool consumeResizeFlag() noexcept;`
- Produces, в `tetris::view`: `void drawOverlay(terminal::Screen & screen, const GameState & state, toy::StringView versionText) noexcept;`

- [ ] **Step 1: Реализовать `togglePause`**

Переключает `playing` и `paused`; на `gameOver` не действует. `advance` при `phase != playing` уже возвращается сразу, поэтому время партии не идёт. `dispatchAction` из итерации 13 начинает обрабатывать `Action::pause`.

- [ ] **Step 2: Реализовать `drawOverlay`**

При `paused` — по центру внутренности поля плашка `ПАУЗА` цветом `c_colorText` на затёртом фоне. При `gameOver` — плашка с причиной (`BLOCK OUT` или `LOCK OUT`), итоговыми `SCORE`, `LEVEL`, `LINES`, строкой `R — заново · Q — выход` и под ней `versionText` тем же цветом: спека отдаёт экрану game over версию из `ConsoleApplication`. `versionText` собирает `tetris_sample_app.cpp` в стек-буфере `std::array<char, 24>` — литерал `v`, затем major, minor и patch через `tetris::view::writeNumber` с точками между ними, — и передаёт как `toy::StringView`; `printf` внутри alternate screen не используется. `drawOverlay` подключается в кадр последним вызовом, после `drawCounters`.

- [ ] **Step 3: Обработать `SIGWINCH`**

`installResizeHandler` вешает обработчик, который только выставляет `std::atomic_flag` — `sigaction` не async-signal-safe для сложной работы. В цикле после `waitForInput`: если `consumeResizeFlag()`, перечитать `querySize()`; при размере вне окна `c_minColumns × c_minRows` … `c_maxColumns × c_maxRows` — рисовать кадр с сообщением о недопустимом размере и не вызывать `advance` и `readKeys`, при размере внутри окна — `resize` экрана и полная перерисовка. Проверять обязательно обе границы: буферы экрана статические, ёмкостью `c_maxColumns × c_maxRows`, и `resize` за пределы ёмкости — запись вне массива. `Phase` не меняется, чтобы снятие с паузы не вернуло игру в нечитаемый экран; проведённое в тесном терминале время в симуляции не засчитывается.

- [ ] **Step 4: Довести рестарт**

`restart` при `phase == gameOver` вызывает `startGame(state, makeSeed())` — новый seed на каждую партию — и `resetRepeat`. При другом `phase` игнорируется.

- [ ] **Step 5: Убрать всё временное**

Проверить, что в `tetris_sample_app.cpp` не осталось тестового узора из итерации 3, временных привязок `a`/`d`/`w`/`s` из итерации 9, принудительного 15 уровня из итераций 8 и 11 и отладочных вызовов `swapHold` из итерации 7. `nanosleep` из итерации 3 заменён на `waitForInput` ещё в итерации 8 — убедиться, что его не осталось.

- [ ] **Step 6: Прогнать партию целиком**

Run: собрать `macos-debug`; сыграть до game over. Сборка `linux-debug` неприменима: гейт семпла — только macOS Desktop (см. Global Constraints индекса плана).
Expected: старт, игра, пауза по `P` с остановкой времени, снятие с паузы, game over с итогами и причиной, рестарт по `R` с новой очередью фигур, выход по `Q` и по Ctrl-C с восстановленным терминалом. Сборка без предупреждений.

- [ ] **Step 7: Проверить изменение размера вручную**

Run: во время партии изменить размер окна терминала — сначала уменьшить ниже 48 × 24, затем вернуть.
Expected: на тесном терминале вместо поля сообщение, партия не идёт; после возврата размера рисунок целый, та же фигура на том же месте, счётчики не изменились.

- [ ] **Step 8: Проверить границу чистой логики**

Run: `grep -n "termios\|poll.h\|unistd\|ioctl\|csignal\|clock_gettime" samples/tetris/tetris_core.hpp samples/tetris/tetris_core.cpp`
Expected: ни одного совпадения. Это единственная механическая гарантия того, что логику можно перенести на API движка без переписывания.

- [ ] **Step 9: Финальные линтеры и коммит**

Run: `./tools/builder/check_code_format.sh` и `./tools/builder/check_source_headers.sh`
Expected: оба с кодом 0.

```bash
git add samples/tetris
git commit -m "feat(samples): finish tetris sample with pause, game over and resize"
```
