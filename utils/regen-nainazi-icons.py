#!/usr/bin/env python3
"""Regenerate 奈娜子密码本 icons from the mascot PNG.

Source of truth (do not substitute an older full-body or smile variant):

    share/icons/application/nainazi-mascot-source.png

The artwork is whatever PNG is committed at that path. Do not swap in an
older mascot (full-body book, purple-hair pout, or any other variant).

Usage (from the repository root):

    python3 utils/regen-nainazi-icons.py

Requires Pillow. Writes:

- PNG sizes under share/icons/application/<size>/apps/
- SVG wrappers (embedded PNG) for the in-app icon theme and mime type
- Monochrome tray masks derived from this same PNG (background keyed out)
- share/windows/keepassxc.ico and keepassxc-kdbx.ico

Color icons are this PNG. The locked color icon is the same picture darkened
so a locked tray can be told apart without a second character design.
"""

from __future__ import annotations

import base64
import io
from pathlib import Path

from PIL import Image, ImageEnhance

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "share/icons/application/nainazi-mascot-source.png"
APP_ICON_DIR = ROOT / "share/icons/application"
WINDOWS_DIR = ROOT / "share/windows"

PNG_SIZES = (16, 24, 32, 48, 64, 128, 256, 512)
ICO_SIZES = (16, 24, 32, 48, 64, 128, 256)


def content_bbox(image: Image.Image) -> tuple[int, int, int, int]:
    """Drop the flat canvas margin. The logo sits on a near-cream field."""
    rgb = image.convert("RGB")
    pixels = rgb.load()
    width, height = rgb.size
    margin = pixels[min(2, width - 1), min(2, height - 1)]

    def is_margin(color: tuple[int, int, int]) -> bool:
        return sum(abs(color[i] - margin[i]) for i in range(3)) < 36

    min_x, min_y, max_x, max_y = width, height, 0, 0
    found = False
    for y in range(height):
        for x in range(width):
            if is_margin(pixels[x, y]):
                continue
            found = True
            min_x = min(min_x, x)
            min_y = min(min_y, y)
            max_x = max(max_x, x)
            max_y = max(max_y, y)
    if not found:
        raise SystemExit(f"No icon pixels found in {SOURCE}")
    return min_x, min_y, max_x + 1, max_y + 1


def crop_square(image: Image.Image) -> Image.Image:
    left, top, right, bottom = content_bbox(image)
    cropped = image.convert("RGBA").crop((left, top, right, bottom))
    side = max(cropped.size)
    canvas = Image.new("RGBA", (side, side), (0, 0, 0, 0))
    offset = ((side - cropped.width) // 2, (side - cropped.height) // 2)
    canvas.paste(cropped, offset)
    return canvas


def darken(image: Image.Image, factor: float = 0.72) -> Image.Image:
    red, green, blue, alpha = image.convert("RGBA").split()
    rgb = Image.merge("RGB", (red, green, blue))
    rgb = ImageEnhance.Brightness(rgb).enhance(factor)
    darkened = rgb.convert("RGBA")
    darkened.putalpha(alpha)
    return darkened


def dominant_color(image: Image.Image) -> tuple[int, int, int]:
    small = image.convert("RGB").resize((48, 48), Image.Resampling.BOX)
    counts: dict[tuple[int, int, int], int] = {}
    for red, green, blue in small.getdata():
        key = (red // 8 * 8, green // 8 * 8, blue // 8 * 8)
        counts[key] = counts.get(key, 0) + 1
    return max(counts, key=counts.get)


def silhouette(image: Image.Image, fill: tuple[int, int, int]) -> Image.Image:
    """Tray mask: keep the character, drop the flat squircle and the outer margin."""
    source = image.convert("RGBA")
    plate = dominant_color(source)
    margin = source.getpixel((0, 0))[:3]
    pixels = source.load()
    width, height = source.size
    out = Image.new("RGBA", (width, height), (0, 0, 0, 0))
    dest = out.load()
    for y in range(height):
        for x in range(width):
            red, green, blue, alpha = pixels[x, y]
            if alpha < 16:
                continue
            color = (red, green, blue)
            plate_distance = sum(abs(color[i] - plate[i]) for i in range(3))
            margin_distance = sum(abs(color[i] - margin[i]) for i in range(3))
            if plate_distance < 42 or margin_distance < 36:
                continue
            dest[x, y] = (*fill, 255)
    return out


def png_bytes(image: Image.Image, size: int) -> bytes:
    resized = image.resize((size, size), Image.Resampling.LANCZOS)
    buffer = io.BytesIO()
    resized.save(buffer, format="PNG", optimize=True)
    return buffer.getvalue()


def write_png(image: Image.Image, path: Path, size: int) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(png_bytes(image, size))


def svg_wrapping(image: Image.Image, comment: str) -> str:
    encoded = base64.b64encode(png_bytes(image, 256)).decode("ascii")
    return (
        '<?xml version="1.0" encoding="UTF-8"?>\n'
        f"<!-- {comment} -->\n"
        '<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" '
        'viewBox="0 0 256 256">\n'
        f'  <image width="256" height="256" href="data:image/png;base64,{encoded}" '
        f'xlink:href="data:image/png;base64,{encoded}"/>\n'
        "</svg>\n"
    )


def write_ico(image: Image.Image, path: Path) -> None:
    """PNG-compressed ICO (Vista+), which is what the Windows resource compiler embeds."""
    import struct

    frames = [image.resize((size, size), Image.Resampling.LANCZOS) for size in ICO_SIZES]
    blobs: list[bytes] = []
    for frame in frames:
        buffer = io.BytesIO()
        frame.save(buffer, format="PNG", optimize=True)
        blobs.append(buffer.getvalue())

    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("wb") as handle:
        handle.write(struct.pack("<HHH", 0, 1, len(frames)))
        offset = 6 + 16 * len(frames)
        for frame, blob in zip(frames, blobs):
            width, height = frame.size
            handle.write(
                struct.pack(
                    "<BBBBHHII",
                    width if width < 256 else 0,
                    height if height < 256 else 0,
                    0,
                    0,
                    1,
                    32,
                    len(blob),
                    offset,
                )
            )
            offset += len(blob)
        for blob in blobs:
            handle.write(blob)


def main() -> None:
    if not SOURCE.is_file():
        raise SystemExit(f"Missing mascot source: {SOURCE}")

    master = crop_square(Image.open(SOURCE))
    locked = darken(master)
    mono_dark = silhouette(master, (15, 15, 18))
    mono_light = silhouette(master, (255, 255, 255))

    for size in PNG_SIZES:
        write_png(master, APP_ICON_DIR / f"{size}x{size}/apps/keepassxc.png", size)
        write_png(locked, APP_ICON_DIR / f"{size}x{size}/apps/keepassxc-locked.png", size)
        write_png(master, APP_ICON_DIR / f"{size}x{size}/apps/keepassxc-unlocked.png", size)

    apps = APP_ICON_DIR / "scalable/apps"
    mime = APP_ICON_DIR / "scalable/mimetypes"
    apps.mkdir(parents=True, exist_ok=True)
    mime.mkdir(parents=True, exist_ok=True)

    note = "奈娜子密码本 — generated from nainazi-mascot-source.png. Do not hand-edit."
    (apps / "keepassxc.svg").write_text(svg_wrapping(master, note), encoding="utf-8")
    (apps / "keepassxc-unlocked.svg").write_text(
        svg_wrapping(master, note + " Unlocked tray uses the same logo."), encoding="utf-8"
    )
    (apps / "keepassxc-locked.svg").write_text(
        svg_wrapping(locked, note + " Locked state is the same logo, darkened."), encoding="utf-8"
    )
    (mime / "application-x-keepassxc.svg").write_text(
        svg_wrapping(master, note + " KDBX mime icon."), encoding="utf-8"
    )
    (apps / "keepassxc-monochrome-dark.svg").write_text(
        svg_wrapping(mono_dark, note + " Tray mask, dark."), encoding="utf-8"
    )
    (apps / "keepassxc-monochrome-dark-locked.svg").write_text(
        svg_wrapping(mono_dark, note + " Tray mask, dark, locked."), encoding="utf-8"
    )
    (apps / "keepassxc-monochrome-light.svg").write_text(
        svg_wrapping(mono_light, note + " Tray mask, light."), encoding="utf-8"
    )
    (apps / "keepassxc-monochrome-light-locked.svg").write_text(
        svg_wrapping(mono_light, note + " Tray mask, light, locked."), encoding="utf-8"
    )

    write_ico(master, WINDOWS_DIR / "keepassxc.ico")
    write_ico(master, WINDOWS_DIR / "keepassxc-kdbx.ico")
    print(f"Icons written from {SOURCE}")


if __name__ == "__main__":
    main()
