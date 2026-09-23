# 奈娜子密码本 rebrand

This tree is a personal fork of [KeePassXC](https://keepassxc.org) (`keepassxreboot/keepassxc`). It is not an official KeePassXC build and it does not replace the upstream project.

The program is still GPL-2.0-or-later (GPL-2 or GPL-3). Copyright stays with the KeePassXC and KeePassX authors. See [COPYING](COPYING).

| | |
| --- | --- |
| Display name | 奈娜子密码本 |
| Executable | `nainazi-passbook` |
| Application id | `com.nainazi.passbook` |
| Organization | `Nainazi` / `com.nainazi.passbook` |

KDBX and the crypto code are unchanged. Browser integration stays compiled in (`KPXC_FEATURE_BROWSER` is still ON). The browser extension protocol is unchanged, so this batch does not edit `keepassxc-browser`.

## Build with official update checks off

Update checks are **off by default**. CMake does not define `KPXC_FEATURE_UPDATES`, so the GUI hides “Check for Updates”, does not start the hourly timer, and `UpdateChecker::checkForUpdates()` returns before it can request `https://api.github.com/repos/keepassxreboot/keepassxc/releases`.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Do not pass `-DKPXC_FEATURE_UPDATES=ON` unless you want that GitHub query compiled back in. If you do, a new config defaults `GUI/CheckForUpdates` to false and the startup prompt defaults to No. An existing `~/.config/keepassxc/keepassxc.ini` that already has `CheckForUpdates=true` would still query upstream when the feature is compiled in.

Snapcraft also passes `-DKPXC_FEATURE_UPDATES=OFF`.

Icon download and other network features are a separate switch (`KPXC_FEATURE_NETWORK`, still ON). This batch only disables the official update check.

## What changed

### Identity

- `src/main.cpp`: `organizationName` = Nainazi, `organizationDomain` = `com.nainazi.passbook`, `applicationName` = `nainazi-passbook`, `applicationDisplayName` = 奈娜子密码本, desktop file name and `KPXC_QUALIFIED_APPNAME` = `com.nainazi.passbook`.
- `src/gui/MainWindow.cpp` / `MainWindow.ui`: window title 奈娜子密码本. Tray “Quit” label is 退出奈娜子密码本.
- `src/gui/Application.cpp`: single-instance lock and socket use `nainazi-passbook`, so this process does not attach to a running official KeePassXC.
- `src/gui/AboutDialog.ui`: title, product line, GPL fork attribution, bug link `https://github.com/Nainazi/keepassxc/issues`. Upstream maintainer and contributor lists are still shown.
- `src/gui/WelcomeWidget.cpp` / `.ui`: welcome line is 奈娜子密码本.
- `src/gui/DatabaseOpenDialog.cpp`, `DatabaseOpenWidget.cpp` / `.ui`: unlock title 解锁奈娜子密码本数据库.
- `src/gui/wizard/NewDatabaseWizard.cpp`: 新建奈娜子密码本数据库…
- `src/core/Tools.cpp`: debug info prefix 奈娜子密码本.
- `src/cli/keepassxc-cli.cpp`: help text names the fork. The binary is still `keepassxc-cli`.

### Package id, desktop, installers

- `CMakeLists.txt`: `APP_ID` is `com.nainazi.passbook`. `PROGNAME` is `nainazi-passbook` on Linux, macOS, and Windows. Linux data and plugin directories are `share/nainazi-passbook` and `lib/nainazi-passbook`. The CMake `project()` name is still `KeePassXC` so existing cache and library target names stay valid.
- Desktop, polkit, and AppStream files renamed to `share/linux/com.nainazi.passbook.*`. Polkit action id is `com.nainazi.passbook.unlockDatabase` (`src/quickunlock/Polkit.cpp`).
- `share/macosx/Info.plist.in`: display name 奈娜子密码本, bundle id from `MACOSX_BUNDLE_IDENTIFIER` = `com.nainazi.passbook`.
- `src/CMakeLists.txt` Windows version resource: internal name `nainazi-passbook`, product name 奈娜子密码本, company Nainazi.
- NSIS display name 奈娜子密码本. WiX shortcuts and file association description use 奈娜子密码本. WiX file ids are `CM_FP_nainazi_passbook.exe` / `CM_CP_nainazi_passbook.exe` because CPack turns the hyphen into an underscore. Upgrade GUID is `12BC3652-C288-4AB0-BE7D-66CFD46D827E` so this installer is not the official KeePassXC product.
- `snap/snapcraft.yaml`: snap name and command `nainazi-passbook`. Native-messaging paths and the D-Bus interface name are unchanged.
- `share/linux/appimage-apprun.sh` and `utils/keepassxc-flatpak-wrapper.sh` launch `nainazi-passbook`. They still exec `keepassxc-proxy` and `keepassxc-cli`.
- `docs/man/keepassxc.1.adoc` installs as `nainazi-passbook.1`.

### Icons

Application, tray, mime, and Windows icons are generated from one mascot file:

`share/icons/application/nainazi-mascot-source.png`

That file is the user-approved final artwork and the only source. Do not redraw it or substitute another file. It is a soft flat logo in a blue color series: golden-blonde twin-tails, a blue cat-ear headband with a padlock, a lock choker, a key on an off-shoulder crop top, half-lidded smug eyes, a wide teasing grin with a fang and blush, and a stylized cover-mouth hand with four distinct fingers, fully inside a cream rounded square. There are no earrings.

Regenerate (needs Pillow):

```sh
python3 utils/regen-nainazi-icons.py
```

The script writes PNG sizes under `share/icons/application/`, SVG wrappers that embed those PNGs (qrc names stay `keepassxc*.svg`), monochrome tray masks derived from the same picture, and `share/windows/keepassxc.ico` plus `keepassxc-kdbx.ico`. The locked color icon is the same logo darkened so a locked tray can be told apart. The default tray appearance is the color logo.

`share/windows/create-ico.sh` still builds an ICO from the SVG when the CMake `icons` target runs and Inkscape is installed. Commit the files the Python script writes so a Windows build does not depend on that step.

In-app `QIcon` names stay `keepassxc` / `keepassxc-monochrome-*` because that is the qrc theme id (`src/gui/Icons.cpp`). Installed hicolor app icons are renamed to `nainazi-passbook*.svg` / `.png` so a system KeePassXC icon is not overwritten. The desktop `Icon=` is `nainazi-passbook`. Flatpak uses `com.nainazi.passbook`. The KDBX mime icon file name stays `application-x-keepassxc.svg`.

### Theme

View → Theme → 奈娜子 is the default (`GUI/ApplicationTheme` = `nainazi`). Light, dark, automatic, and classic remain. The theme is `NainaziStyle` plus `src/gui/styles/nainazi/nainazistyle.qss`: lavender, pink, and cream palette, rounder buttons and fields. It does not replace the KeePassXC layout.

### Auto-Type

- New configs default the global shortcut to Ctrl+Shift+A. Change it under Settings → Auto-Type. The in-app Always on Top shortcut moved to Ctrl+Shift+O so it does not take the same keys.
- Auto-Type runs only when the user triggers it. There is no global keylogger and no silent clipboard fill.
- New configs use an 800 ms start delay and a 50 ms key delay (`AutoTypeStartDelay`, `AutoTypeDelay`). A stored value is left alone. Settings → Auto-Type explains the Chinese IME case: finish composition before typing, and raise the delays if characters are dropped.
- If it cannot run, the user sees a message: no database, database locked (unlock dialog titled for Auto-Type), no matching entry (including a title that differs across monitors or after focus loss), missing or closed target window, the target window changing mid-sequence, or a Windows administrator / UAC window that blocks keystrokes.
- New configs lock the database after 15 minutes idle (`Security/LockDatabaseIdle` = true, `Security/LockDatabaseIdleSeconds` = 900) and clear the clipboard after 10 seconds (`Security/ClearClipboardTimeout`). Those were already the defaults; stored values are left alone.
- New configs generate 24-character passwords with upper case, lower case, numbers, and symbols, excluding look-alike characters (`PasswordGenerator/Length` and the existing symbol flags). A stored generator setup is left alone.
- Entry and group editors can insert `{USERNAME}{TAB}{PASSWORD}{ENTER}`, `{USERNAME}{TAB}{PASSWORD}{TAB}{ENTER}`, or `{PASSWORD}{ENTER}`. Custom sequences still work.
- KDBX and the browser protocol are unchanged.

## Left unchanged on purpose

- KDBX read/write and crypto. `Metadata` generator string is still `KeePassXC` inside the database file.
- `KPXC_FEATURE_BROWSER` and the browser protocol: native messaging host `org.keepassxc.keepassxc_browser`, socket `/org.keepassxc.KeePassXC.BrowserServer`, binaries `keepassxc-proxy` and `keepassxc-cli`.
- D-Bus interface `org.keepassxc.KeePassXC.MainWindow`.
- Linux config directory `~/.config/keepassxc` and `~/.local/state/keepassxc` (`src/core/Config.cpp`). Windows and macOS `QStandardPaths` follow organization `Nainazi` and application `nainazi-passbook`, so those platforms do not share the official KeePassXC settings folder.
- Help and donate menu links that open keepassxc.org when the user clicks them. Those are not the update checker.
- Browser protocol strings, the KeePassXC-Browser extension name, native-messaging host names, and links that open keepassxc.org on purpose.

`share/translations/keepassxc_zh_CN.ts` and `keepassxc_zh_TW.ts` translate the new Auto-Type, delay, and product-name strings. The window title and About heading stay hardcoded 奈娜子密码本.

## Windows packaging

The installer path is the existing CPack WiX setup (`CPACK_GENERATOR` is `ZIP;WIX`), plus the NSIS variables already in `src/CMakeLists.txt`.

- Add/Remove Programs name and the WiX product name are 奈娜子密码本. The Start Menu shortcut is 奈娜子密码本.
- The install directory page has an optional desktop shortcut checkbox (`INSTALLDESKTOPSHORTCUT`). It is off unless the user checks it.
- `.kdbx` opens with `nainazi-passbook` (WiX `wix-patch.xml`, and NSIS registry commands if that generator is used).
- Uninstall removes the Start Menu shortcut, the optional desktop shortcut, and this product's `.kdbx` ProgId. `keepassxc-proxy.exe` is still the browser helper and is stopped on install and uninstall. The upgrade GUID is not the official KeePassXC GUID.

A Windows machine builds the installer with the usual KeePassXC Qt 6 / Botan configure, then `cmake --build . --config Release` and `cpack -G WIX`. This environment has no Qt 6 or Botan, so that build was not run here.

## TODO

- KeePassXC-Browser protocol strings, group names, and the extension download prompt still say KeePassXC-Browser on purpose.
- macOS `keepassxc.icns` / `Assets.car` still need `xcrun actool` on a Mac. Windows `keepassxc.ico` is already generated by `utils/regen-nainazi-icons.py`.
- Database entry icons under `share/icons/database/` are the KeePass icon set, not the application icon.
- Action icons, wizard background, and installer banner PNGs are still the upstream art.
- WiX hyphen-to-underscore ids should be confirmed with a real `cpack -G WIX` run before publishing a Windows installer.
