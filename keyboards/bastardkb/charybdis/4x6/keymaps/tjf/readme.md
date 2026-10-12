# Charybdis (4x6) `tjf` keymap

Built on BastardKB's userspace (release 10) with the `bastardkb/bk_pointing_device`
module.  VIA and Argos are off: the layout lives in `keymap.c` and every setting
lives in `config.h`, so nothing stored on the board overrides them.

Build: `qmk compile -kb bastardkb/charybdis/4x6/splinktegrated_rev1 -km tjf`

## Files

- `keymap.c` - layers, custom keys, and hooks.
- `config.h` - tap-hold, combos, auto mouse, pointer DPI, smart scroll,
  lighting.
- `smart_scroll.c` - `SMTSCRL`: drag-scroll locked to one axis at a time.
- `secrets.h` - login text for the `M_PASS`/`M_USER`/`M_EMAIL` keys.  Not in
  git; copy `secrets.h.example` to start.
- `argos_rgb.h` - empty stand-in so the r10 module builds without Argos.

## Layers

- **Base:** typing, with home-row mods (A Ctrl, S Shift, D Option, F Cmd).
  Hold Z for the pointer layer.
- **MISSING, KEYPAD, SYSTEM:** F-keys/arrows, numpad, board settings.

## Combos (auto pointer off only)

- J + K: left click.  K + L: smart scroll.  L + ;: right click.  All held while
  the keys are held, so modifier-clicks and drags work.

## Tap-hold

- **Mod-taps** use Speculative Hold: the modifier is on from keydown.
- **Layer-taps** use Flow Tap: right after typing, they tap at once.
- Home-row mods also use Flow Tap.  `SFT_ESC` becomes Shift as soon as another
  key goes down (`get_hold_on_other_key_press`).

## Pointer

- **Auto mouse:** on at boot; `AUTOPTR` (SYSTEM layer, M) turns it on and
  off.  Off = mouse combos and ripple lighting.  When on, moving the ball turns on `LAYER_POINTER` (layer 1).  Tune
  `AUTO_MOUSE_TIME`, `AUTO_MOUSE_THRESHOLD` and `AUTO_MOUSE_DEBOUNCE` in
  `config.h`.  `LAYER_POINTER` and `AUTO_MOUSE_DEFAULT_LAYER` must match (the
  build checks).
- **Pointer modes:** each mode has its own DPI.  `keyboard_post_init_user()`
  sets them from `POINTER_DPI`, `POINTER_SNIPING_DPI` and
  `POINTER_DRAGSCROLL_DPI` at every boot.  `DPI_MOD` changes the active mode's
  DPI until the next power-up.
- **Mode keys (hold / toggle):** `DRGSCRL`/`DRG_TOG`, `SNIPING`/`SNP_TOG`,
  `CURSOR`, `PZOOM`, `PVOLUME`, `PTABS`, `PHIST`, `PBRIGHT` (each also has a
  `_TOG` version).  `PZOOM` sends Ctrl +/-, not Cmd.
