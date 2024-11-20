/* Copyright 2024 FibreTTP

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

enum custom_keycodes {
    LOCKGUI = SAFE_RANGE,
};

bool GUI_LOCK_STATUS = false;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12	     Del           Rotary(Mute)
//      ~        1        2        3        4        5        6        7        8        9        0         -       (=)	     BackSpc           Ins
//      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]        \                 Home
//      Caps     A        S        D        F        G        H        J        K        L        ;        "                 Enter             End
//      Sh_L              Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       PrtSc
//      Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down     Right


    // The FN key by default maps to a momentary toggle to layer 1 to provide access to the QK_BOOT key (to put the board into bootloader mode). Without
    // this mapping, you have to open the case to hit the button on the bottom of the PCB (near the USB cable attachment) while plugging in the USB
    // cable to get the board into bootloader mode - definitely not fun when you're working on your QMK builds. Remove this and put it back to KC_RGUI
    // if that's your preference.
    //
    // To put the keyboard in bootloader mode, use FN+backslash. If you accidentally put it into bootloader, you can just unplug the USB cable and
    // it'll be back to normal when you plug it back in.
    //
    // This keyboard defaults to 6KRO instead of NKRO for compatibility reasons (some KVMs and BIOSes are incompatible with NKRO).
    // Since this is, among other things, a "gaming" keyboard, a key combination to enable NKRO on the fly is provided for convenience.
    // Press Fn+N to toggle between 6KRO and NKRO. This setting is persisted to the EEPROM and thus persists between restarts.
    [0] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,           KC_MUTE,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_INS,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_HOME,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_END,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   KC_PSCR,
        KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(1),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),

    [1] = LAYOUT(
        KC_NO,   KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,  KC_NO,            KC_NO,  
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,            KC_SCRL,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   QK_BOOT,          KC_PGUP,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,            KC_NO,            KC_PGDN,
        KC_NO,            KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   NK_TOGG, KC_NO,   KC_NO,   KC_NO,   KC_NO,            KC_NO,   KC_MPLY, KC_NO,  
        KC_NO,   LOCKGUI, KC_NO,                              KC_NO,                              KC_NO,   KC_TRNS, KC_NO,   KC_MPRV, KC_MSTP, KC_MNXT
    ),


};
// clang-format on

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
  // Taken from Stickandgum's keymap:
  // https://github.com/qmk/qmk_firmware/tree/cf935d97ae479e7a1e1f2f2f248b93e52e4cc69e/keyboards/gmmk/pro/rev1/ansi/keymaps/stickandgum
  if (get_mods() & MOD_MASK_CTRL) { // If CTRL is held
    uint8_t mod_state = get_mods(); // Store all modifiers that are held
    unregister_mods(MOD_MASK_CTRL); // Immediately unregister the CRTL key (don't send CTRL-PgDn) - del_mods doesn't work here (not immediate)
    if (clockwise) {
      tap_code(KC_PGDN);
    } else {
      tap_code(KC_PGUP);
    }
    set_mods(mod_state); // Add back in the CTRL key - so ctrl-key will work if ctrl was never released after paging.
  } else if (get_mods() & MOD_MASK_SHIFT) {
    uint8_t mod_state = get_mods();
    unregister_mods(MOD_MASK_SHIFT);
    if (clockwise) {
      #ifdef MOUSEKEY_ENABLE   // If using the mouse scroll - make sure MOUSEKEY is enabled
        tap_code(KC_MS_WH_DOWN);
      #else
        tap_code(KC_VOLU);
      #endif
    } else {
      #ifdef MOUSEKEY_ENABLE
        tap_code(KC_MS_WH_UP);
      #else
        tap_code(KC_VOLD);
      #endif
    }
    set_mods(mod_state);
  } else if (clockwise) { // All else volume.
    tap_code(KC_VOLU);
  } else {
    tap_code(KC_VOLD);
  }
  //return true; //set to return false to counteract enabled encoder in pro.c
  return false;
}
#endif // ENCODER_ENABLE

void keyboard_post_init_user(void) {
    #ifdef RGB_MATRIX_ENABLE
        rgb_matrix_set_flags(LED_FLAG_NONE);
        rgb_matrix_set_color_all(0, 0, 0);
    #endif // RGB_MATRIX_ENABLE
}

#ifdef RGB_MATRIX_ENABLE
bool rgb_matrix_indicators_user(void) {
    int LEFT_LEDS[] = {67, 70, 73, 76, 80, 83, 87, 91, 2, 4, 5, 3};
    int RIGHT_LEDS[] = {68, 71, 74, 77, 81, 84, 88, 92, 75, 86, 82, 72};
    int i;
    if (host_keyboard_led_state().caps_lock) {
        for (i = 0; i < 12; i++) {
            rgb_matrix_set_color(LEFT_LEDS[i], 255, 255, 255);
        }
    } else {
        for (i = 0; i < 12; i++) {
            rgb_matrix_set_color(LEFT_LEDS[i], 0, 0, 0);
        }
    }
    if (host_keyboard_led_state().scroll_lock) {
        for (i = 0; i < 12; i++) {
            rgb_matrix_set_color(RIGHT_LEDS[i], 255, 255, 255);
        }
    } else {
        for (i = 0; i < 12; i++) {
            rgb_matrix_set_color(RIGHT_LEDS[i], 0, 0, 0);
        }
    }
    return true;
}
#endif // RGB_MATRIX_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case LOCKGUI:
        if (record->event.pressed) {
            keymap_config.no_gui = !keymap_config.no_gui;
            #ifdef RGB_MATRIX_ENABLE
                if (GUI_LOCK_STATUS) {
                    rgb_matrix_set_color(11, 0, 0, 0);
                } else {
                    rgb_matrix_set_color(11, 40, 40, 40);
                }
            #endif // RGB_MATRIX_ENABLE
            GUI_LOCK_STATUS = !GUI_LOCK_STATUS;
            }
        break;
    }
    return true;
}