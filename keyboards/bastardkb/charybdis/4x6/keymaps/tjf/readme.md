# Charybdis (4x6) `via` keymap

The Charydbis (4x6) `via` keymap is inspired from the original [Dactyl Manuform](../../../../../handwired/dactyl_manuform) default keymap, with some features and changes specific to the Charybdis.

This layout supports RGB matrix. However, due to space constraints on the MCU, only a limited number of effect can be enabled at once. Look at the `config.h` file and enable your favorite effect.

## Customizing the keymap

### Dynamic DPI scaling

Use the following keycodes to change the default DPI:

-   `POINTER_DEFAULT_DPI_FORWARD`: increases the DPI; decreases when shifted;
-   `POINTER_DEFAULT_DPI_REVERSE`: decreases the DPI; increases when shifted.

There's a maximum of 16 possible values for the sniping mode DPI. See the [Charybdis documentation](../../README.md) for more information.

Use the following keycodes to change the sniping mode DPI:

-   `POINTER_SNIPING_DPI_FORWARD`: increases the DPI; decreases when shifted;
-   `POINTER_SNIPING_DPI_REVERSE`: decreases the DPI; increases when shifted.

There's a maximum of 4 possible values for the sniping mode DPI. See the [Charybdis documentation](../../README.md) for more information.

### Drag-scroll

Use the `DRAGSCROLL_MODE` keycode to enable drag-scroll on hold. Use the `DRAGSCROLL_TOGGLE` keycode to enable/disable drag-scroll on key press.

### Sniping

Use the `SNIPING_MODE` keycode to enable sniping mode on hold. Use the `SNIPING_MODE_TOGGLE` (aliased as `SNP_TOG`) keycode to enable/disable sniping mode on key press.

Change the value of `CHARYBDIS_AUTO_SNIPING_ON_LAYER` to automatically enable sniping mode on layer change. By default, sniping mode is enabled on the pointer layer:

```c
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER
```

### Auto pointer layer

**Note:** this keymap builds against the BastardKB fork with the
`bastardkb/bk_pointing_device` community module, which uses QMK's core
auto-mouse feature. The old `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_*` defines
below no longer exist -- they are silently ignored. The knobs that matter are
in `config.h`:

- `AUTO_MOUSE_DEFAULT_LAYER` -- which layer to switch to (here: `LAYER_POINTER`);
- `AUTO_MOUSE_TIME` -- idle ms before the layer is released (core default 650);
- `AUTO_MOUSE_THRESHOLD` -- movement needed to trigger (core default 10);
- `AUTO_MOUSE_DEBOUNCE` -- ms after a keypress before the ball can re-trigger
  (core default 25).

The feature itself is enabled/disabled from a flag in EEPROM, toggleable in
Argos. `keyboard_post_init_user()` in `keymap.c` forces it on at boot.

## Layout

![Keymap layout (generated with keyboard-layout-editor.com)](https://i.imgur.com/qI7phR7.png)
