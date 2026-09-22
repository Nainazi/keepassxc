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

Replaced artwork (indigo book and lock, not the green KeePassXC shield):

- `share/icons/application/scalable/apps/keepassxc.svg`
- `keepassxc-locked.svg`, `keepassxc-unlocked.svg`
- `keepassxc-monochrome-dark.svg`, `keepassxc-monochrome-light.svg`, and the `-locked` pair
- `share/icons/application/scalable/mimetypes/application-x-keepassxc.svg`
- `share/icons/application/256x256/apps/keepassxc.png` (raster of the new SVG)

In-app `QIcon` names stay `keepassxc` / `keepassxc-monochrome-*` because that is the qrc theme id (`src/gui/Icons.cpp`). Installed hicolor app icons are renamed to `nainazi-passbook*.svg` / `.png` so a system KeePassXC icon is not overwritten. The desktop `Icon=` is `nainazi-passbook`. Flatpak uses `com.nainazi.passbook`. The KDBX mime icon file name stays `application-x-keepassxc.svg`.

## Left unchanged on purpose

- KDBX read/write and crypto. `Metadata` generator string is still `KeePassXC` inside the database file.
- `KPXC_FEATURE_BROWSER` and the browser protocol: native messaging host `org.keepassxc.keepassxc_browser`, socket `/org.keepassxc.KeePassXC.BrowserServer`, binaries `keepassxc-proxy` and `keepassxc-cli`.
- D-Bus interface `org.keepassxc.KeePassXC.MainWindow`.
- Linux config directory `~/.config/keepassxc` and `~/.local/state/keepassxc` (`src/core/Config.cpp`). Windows and macOS `QStandardPaths` follow organization `Nainazi` and application `nainazi-passbook`, so those platforms do not share the official KeePassXC settings folder.
- Help and donate menu links that open keepassxc.org when the user clicks them. Those are not the update checker.
- Translation catalogs (`share/translations/*.ts`). Product-name strings that this batch changed are marked `notr` or are no longer the old English source, so a zh_CN catalog cannot put “KeePassXC” back on the window title, About dialog, or welcome screen.

## TODO

- Many settings, reports, and dialog strings still say KeePassXC (`src/gui/ApplicationSettingsWidget*.ui`, database settings, message boxes). They are not the window title or About name.
- macOS `keepassxc.icns` / `Assets.car` and Windows `keepassxc.ico` are generated by `utils/makeappicons.sh` and `share/windows/create-ico.sh`. Regenerate them from the new SVG before shipping those packages. Source SVG and the 256px PNG are already replaced.
- Database entry icons under `share/icons/database/` are the KeePass icon set, not the application icon.
- Action icons, wizard background, and installer banner PNGs are still the upstream art.
- WiX hyphen-to-underscore ids should be confirmed with a real `cpack -G WIX` run before publishing a Windows installer.
