#!/bin/bash
# Turns a finished build into portable zips: symbols split off, SDL2 and OpenAL
# inside each .app, ad-hoc signed. The build directory is left untouched.
# Usage: scripts/package_macos.sh <dir holding the .app bundles> <output dir> <zip name>
set -euo pipefail

BUILD_DIR=$1
OUT_DIR=$2
NAME=$3

STAGE="$OUT_DIR/$NAME"
SYMBOLS="$OUT_DIR/$NAME-debug"
mkdir "$STAGE" "$SYMBOLS"

for GAME in vanillatd vanillara; do
    ditto "$BUILD_DIR/$GAME.app" "$STAGE/$GAME.app"

    APP="$STAGE/$GAME.app"
    BIN="$APP/Contents/MacOS/$GAME"
    LIBS="$APP/Contents/libs"

    dsymutil "$BIN" -o "$SYMBOLS/$GAME.dSYM"
    strip -Sx "$BIN"
    dylibbundler --create-dir --bundle-deps --overwrite-files --dest-dir "$LIBS" --fix-file "$BIN"

    # sdl2-compat opens SDL3 with dlopen, so dylibbundler can't see it. It looks
    # for libSDL3.dylib in its own directory.
    if strings "$LIBS"/libSDL2*.dylib | grep -q '@loader_path/libSDL3.dylib'; then
        SDL3_LIB_DIR=${SDL3_LIB_DIR:-$(brew --prefix sdl3)/lib}
        cp -L "$SDL3_LIB_DIR/libSDL3.dylib" "$LIBS/libSDL3.dylib"
        chmod u+w "$LIBS/libSDL3.dylib"
        install_name_tool -id "@executable_path/../libs/libSDL3.dylib" "$LIBS/libSDL3.dylib"
    fi

    # Inner code has to be signed first, --deep doesn't look in Contents/libs.
    for LIB in "$LIBS"/*.dylib; do
        codesign --force --sign - "$LIB"
    done
    codesign --force --sign - "$APP"
    codesign --verify --strict "$APP"
done

cp License.txt resources/LEEME.txt resources/cfepatchredux.ini resources/cfepatchredux_ra.ini scripts/is3extract.py "$STAGE/"
ditto -c -k --keepParent "$STAGE" "$OUT_DIR/$NAME.zip"
ditto -c -k --keepParent "$SYMBOLS" "$OUT_DIR/$NAME-debug.zip"
