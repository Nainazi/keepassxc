# 奈娜子密码本

Personal password manager. This repository is a GPL fork of [KeePassXC](https://keepassxc.org) ([keepassxreboot/keepassxc](https://github.com/keepassxreboot/keepassxc)) for personal use. It is not an official KeePassXC release.

| | |
| --- | --- |
| Display name | 奈娜子密码本 |
| Executable | `nainazi-passbook` |
| Application id | `com.nainazi.passbook` |

KeePassXC is a modern, secure, and open-source password manager that stores and manages your most sensitive information. You can run it on Windows, macOS, and Linux. It saves usernames, passwords, URLs, attachments, and notes in an offline, encrypted KDBX file. This fork keeps that format, the crypto, and browser integration, and changes the name, icon, and update checks. Branding details and the build flag that disables official update checks are in [REBRAND.md](REBRAND.md).

## Quick Start

The [QuickStart Guide](https://keepassxc.org/docs/KeePassXC_GettingStarted.html) gets you started using KeePassXC on your Windows, macOS, or Linux computer using pre-compiled binaries from the [downloads page](https://keepassxc.org/download). Additionally, individual Linux distributions may ship their own versions, so please check your distribution's package list to see if KeePassXC is available. Detailed documentation is available in the [User Guide](https://keepassxc.org/docs/KeePassXC_UserGuide.html).

## Features List

KeePassXC has numerous features for novice and power users alike. Our goal is to create an application that can be used by anyone while still offering advanced features to those that need them.

### Core Features

* Create, open, and save databases in the KDBX format (KeePass-compatible with KDBX4 and KDBX3)
* All information is encrypted at rest and never exposed outside the program
* Store sensitive information in entries that are organized by groups
* Password generator
* Search for entries
* TOTP storage and generation
* YubiKey/OnlyKey challenge-response support
* Auto-Type passwords into applications
* Browser integration with Google Chrome, Mozilla Firefox, Microsoft Edge, Chromium, Vivaldi, Brave, and Tor-Browser
* Support for passkeys using the browser integration
* Entry icon download
* Import databases from CSV, 1Password, Bitwarden, Proton Pass, and KeePass1 formats

### Advanced
* Database reports (password health, HIBP, and statistics)
* Database export to CSV, XML, and HTML formats
* TOTP storage and generation
* Field references between entries
* File attachments and custom attributes
* Entry history and data restoration
* Command line interface (keepassxc-cli)
* SSH Agent integration
* FreeDesktop.org Secret Service (replace Gnome keyring, etc.)
* Additional encryption choices: Twofish and ChaCha20

For a full list of changes, read the [CHANGELOG](CHANGELOG.md) document. \
For a full list of keyboard shortcuts, see [KeyboardShortcuts.adoc](./docs/topics/KeyboardShortcuts.adoc)

## Building

Follow the upstream [Build and Install](./INSTALL.md) page. For this fork, official update checks are off unless you pass `-DKPXC_FEATURE_UPDATES=ON`. See [REBRAND.md](REBRAND.md). The GUI binary is `nainazi-passbook`. `keepassxc-cli` and `keepassxc-proxy` keep their upstream names so browser integration still matches the KeePassXC browser extension.

## Contributing

We are always looking for suggestions on how to improve KeePassXC. If you find any bugs or have an idea for a new feature, please let us know by opening a report in the [issue tracker](https://github.com/keepassxreboot/keepassxc/issues) on GitHub, or join us on [Matrix community channel](https://matrix.to/#/!zUxwGnFkUyycpxeHeM:matrix.org?via=matrix.org) or [Matrix development channel](https://matrix.to/#/!RhJPJPGwQIFVQeXqZa:matrix.org?via=matrix.org), or on IRC in [Libera.Chat](https://web.libera.chat/) channels #keepassxc and #keepassxc-dev.

You may directly contribute your own code by submitting a pull request. Please read the [CONTRIBUTING](.github/CONTRIBUTING.md) document for further information.

Contributors are required to adhere to the project's [Code of Conduct](CODE-OF-CONDUCT.md).

## Generative AI

Generative AI is fast becoming a first-party feature in most development environments, including GitHub itself. If the majority of a code submission is made using Generative AI (e.g., agent-based or vibe coding) then **we will document that in the pull request.** All code submissions go through a rigorous review process regardless of the development workflow or submitter.

## License

This fork is licensed under GPL-2.0 or GPL-3.0, the same terms as KeePassXC. Copyright remains with the original KeePassXC and KeePassX authors. Additional licensing for third-party files is detailed in [COPYING](./COPYING). Do not remove those notices.
