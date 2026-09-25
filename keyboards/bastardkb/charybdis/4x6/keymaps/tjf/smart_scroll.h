/* Smart drag-scroll: trackball motion becomes scrolling, locked to one axis.
 * Settings (SMART_SCROLL_*) live in config.h. */
#pragma once

#include "quantum.h"

void           smart_scroll_set_active(bool active); // Hold key: pressed / released.
void           smart_scroll_toggle(void);            // Toggle key: on press.
report_mouse_t smart_scroll_task(report_mouse_t report);
