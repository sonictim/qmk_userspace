/* Smart drag-scroll.
 *
 * While active, trackball motion becomes scrolling.  The first
 * SMART_SCROLL_LOCK_THRESHOLD counts of movement decide the axis (vertical or
 * horizontal); after that, only that axis scrolls until scrolling stops, or
 * until the ball sits still for SMART_SCROLL_RELOCK_MS, after which the next
 * movement picks the axis again.
 */
#include "smart_scroll.h"
#include <stdlib.h>

// Defaults; set these in config.h.
#ifndef SMART_SCROLL_LOCK_THRESHOLD
#    define SMART_SCROLL_LOCK_THRESHOLD 16
#endif
#ifndef SMART_SCROLL_DIVISOR
#    define SMART_SCROLL_DIVISOR 12.0f
#endif
#ifndef SMART_SCROLL_RELOCK_MS
#    define SMART_SCROLL_RELOCK_MS 300
#endif

typedef enum {
    SCROLL_AXIS_NONE,
    SCROLL_AXIS_V,
    SCROLL_AXIS_H,
} scroll_axis_t;

static bool          smart_scroll_active      = false;
static scroll_axis_t smart_scroll_axis        = SCROLL_AXIS_NONE;
static int16_t       smart_scroll_probe_x     = 0; // Motion gathered before the axis is chosen.
static int16_t       smart_scroll_probe_y     = 0;
static float         smart_scroll_acc         = 0; // Sub-tick remainder on the locked axis.
static uint32_t      smart_scroll_last_motion = 0;

static void smart_scroll_reset(void) {
    smart_scroll_axis    = SCROLL_AXIS_NONE;
    smart_scroll_probe_x = 0;
    smart_scroll_probe_y = 0;
    smart_scroll_acc     = 0;
}

void smart_scroll_set_active(bool active) {
    smart_scroll_active = active;
    smart_scroll_reset();
}

void smart_scroll_toggle(void) {
    smart_scroll_set_active(!smart_scroll_active);
}

report_mouse_t smart_scroll_task(report_mouse_t report) {
    if (!smart_scroll_active) {
        return report;
    }

    int16_t dx = report.x;
    int16_t dy = report.y;
    report.x   = 0;
    report.y   = 0;

    if (dx == 0 && dy == 0) {
        return report;
    }
    // Re-pick the axis if the ball has been still long enough.
    if (SMART_SCROLL_RELOCK_MS > 0 && smart_scroll_axis != SCROLL_AXIS_NONE && timer_elapsed32(smart_scroll_last_motion) > SMART_SCROLL_RELOCK_MS) {
        smart_scroll_reset();
    }
    smart_scroll_last_motion = timer_read32();

    if (smart_scroll_axis == SCROLL_AXIS_NONE) {
        smart_scroll_probe_x += dx;
        smart_scroll_probe_y += dy;
        int16_t ax = abs(smart_scroll_probe_x);
        int16_t ay = abs(smart_scroll_probe_y);
        if (ax + ay < SMART_SCROLL_LOCK_THRESHOLD) {
            return report; // Not enough movement to decide yet.
        }
        smart_scroll_axis = (ay >= ax) ? SCROLL_AXIS_V : SCROLL_AXIS_H;
        // Carry the probe motion into the first scroll so nothing is lost.
        dx = smart_scroll_probe_x;
        dy = smart_scroll_probe_y;
    }

    smart_scroll_acc += (float)(smart_scroll_axis == SCROLL_AXIS_V ? -dy : dx) / SMART_SCROLL_DIVISOR;

    int16_t ticks = (int16_t)smart_scroll_acc; // Truncates toward zero; remainder carries over.
    if (ticks > 127) ticks = 127;
    if (ticks < -127) ticks = -127;
    smart_scroll_acc -= ticks;

    if (smart_scroll_axis == SCROLL_AXIS_V) {
        report.v = ticks;
    } else {
        report.h = ticks;
    }
    return report;
}
