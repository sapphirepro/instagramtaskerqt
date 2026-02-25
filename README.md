# InstagramTasker Qt

Desktop Qt application for managing and processing Instagram profile download tasks with a queue-based workflow.
<img width="1701" height="820" alt="instagram_tasker" src="https://github.com/user-attachments/assets/4524ed96-2830-413b-885a-3c60d787cc63" />

The app keeps a source list of profiles, lets you build an active queue, and runs a Python-based Instaloader script with per-profile options.

Important: this GUI is built specifically around Instaloader and requires it to be installed:
https://github.com/instaloader/instaloader

## Highlights

- Source profiles table with per-profile flags:
  - `Enabled`
  - `Video`
  - `Stories`
  - `Highlights`
  - `Tagged`
  - `Reels`
  - `Rating` (0..5 stars, interactive mouse hover/click)
  - `Last updated`
  - `Videos size`
- Queue management with duplicate protection and persistent resume support (`resume.txt`).
- Data source options:
  - plain text profiles list (`.txt`)
  - SQLite profiles database (`.db/.sqlite/.sqlite3`)
- Schema-safe DB updates (rating field defaults to `0` for existing databases).
- Batch service actions:
  - add all profiles with 5/4/3/2/1 stars
  - add all rated profiles (1..5)
  - add oldest profiles (with configurable batch limit)
- Disabled profiles are excluded from queue operations.
- Automatic profile disabling on `404 Not Found` download errors.
- Optional video size accounting and recalculation.
- Runtime localization support for multiple languages.
- Single-instance app behavior and system tray integration.

## Requirements

- CMake `>= 3.10`
- C++17 compiler
- Qt (Qt5 or Qt6) modules:
  - `Core`
  - `Widgets`
  - `Sql`
- Python 3 (used to run the configured Instaloader script)
- Instaloader from the official repository: https://github.com/instaloader/instaloader
- A valid path to `instaloader.py` configured in Preferences

Notes:
- Folder/gallery opening commands are configurable in `Preferences -> Environment parameters`.
- Defaults are platform-aware (`xdg-open` on Linux, `open` on macOS, `explorer` on Windows).

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
```

Notes:
- For single-config generators (Unix Makefiles/Ninja), `CMAKE_BUILD_TYPE=Release` is used.
- For multi-config generators (Visual Studio/Xcode), `--config Release` selects the build configuration.

## Run

```bash
./build/InstagramTasker
```

Windows (Visual Studio generator):

```powershell
.\build\Release\InstagramTasker.exe
```

The app is tray-oriented and starts without forcing the main window to foreground.  
Use the tray icon to show/hide the window.

## Install (optional)

```bash
cmake --install build
```

To set a custom install prefix:

```bash
cmake --install build --prefix <install_dir>
```

Linux note:
- If install prefix was not overridden, default prefix is `~/.local`, so binary is typically installed to `~/.local/bin/InstagramTasker`.

## Quick Start

1. Install Instaloader first (required): https://github.com/instaloader/instaloader
2. Open `Settings -> Preferences...`.
3. Set:
   - working directory
   - Instaloader script path (`instaloader.py` from your Instaloader installation)
   - `Environment parameters`:
     - Python executable
     - file manager command
     - gallery viewer command
   - optional downloader flags (login, user-agent, abort codes, etc.)
4. Load profiles from:
   - `File -> Load Profiles...` (text file), or
   - `File -> Load Profiles from DB...` (SQLite database)
5. Add profiles to queue:
   - use `>>` from source list,
   - or add manually via the "Add new profiles" box.
6. Click `Start`.

## Data Model

### Text mode

- Source file stores only profile names (one per line).
- Per-profile flags and runtime queue state are kept in app state / working files.

### Database mode

The app uses a `profiles` table with fields:

- `profile_name` (unique, case-insensitive)
- `enabled`
- `videos`
- `stories`
- `highlights`
- `tagged`
- `reels`
- `rating` (default `0`)
- `last_updated`
- `videos_size`

When loading older databases, the app ensures required schema exists and adds missing `rating` column with default `0`.

## Working Directory Files

- `resume.txt`  
  Saved active queue for crash-safe resume/restart behavior.

- `skipped.txt`  
  Profiles that failed download and should start disabled in source list.

## Queue and Processing Behavior

- Only profiles with `Enabled = true` can be added to active queue.
- Batch queue additions ignore disabled profiles.
- If a profile is disabled, it is removed from active queue immediately.
- Processing order is randomized for queued profiles.
- Between profiles, app sleeps random `3..15` seconds.
- Success updates `Last updated`.

## Error Handling

- On checkpoint/feedback-required patterns, processing is auto-paused and user is notified.
- On `404 Not Found` profile errors:
  - profile is auto-disabled (`Enabled` unchecked),
  - removed from active queue,
  - persisted to DB and `skipped.txt`.

## Service Menu Tools

- `Add by rating` submenu:
  - `Add 5 stars` ... `Add 1 star`
  - `Add all rated`
- `Add oldest profiles`:
  - prioritizes profiles with no `Last updated` value first
  - then oldest dates ascending
  - uses `Batch queue limit` from Preferences (default `50`)
- `Count video size` toggle
- `Recalculate video size`

## Localization

The project ships runtime `.ts` translation files (loaded without `lrelease` at runtime via a custom translator).

Supported languages include:

- English
- Russian
- German
- Spanish
- French
- Dutch
- Italian
- Portuguese
- Polish
- Czech
- Slovak
- Hungarian
- Romanian
- Swedish
- Norwegian
- Danish
- Finnish
- Greek

## License

GPL-2.0 (as referenced in the app About dialog):  
https://www.gnu.org/licenses/old-licenses/gpl-2.0.html
