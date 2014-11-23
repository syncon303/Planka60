#include "keymap_common.h"

const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = KEYMAP( /* Native */
      ESC,  1,    2,    3,    4,    5,    HOME, RALT, 6,    7,    8,    9,    0,    BSPC,
      FN10, Q,    W,    E,    R,    T,    LBRC, RBRC, Y,    U,    I,    O,    P,    FN11,
      RCTL, A,    S,    D,    F,    G,    MINS, EQL,  H,    J,    K,    L,    SCLN, ENT,
      LSFT, Z,    X,    C,    V,    B,    QUOT, DEL,  N,    M,    COMM, DOT,  SLSH, RSFT,
      LCTL, FN6,  LGUI, LALT, FN1,        SPC,  ENT,        FN2,  LEFT, DOWN, UP,   RGHT),
    [1] = KEYMAP( /* QWERTY->Colemak */
      ESC,  1,    2,    3,    4,    5,    HOME, RALT, 6,    7,    8,    9,    0,    BSPC,
      FN10, Q,    W,    F,    P,    G,    LBRC, RBRC, J,    L,    U,    Y,    SCLN, FN11,
      RCTL, A,    R,    S,    T,    D,    MINS, EQL,  H,    N,    E,    I,    O,    ENT,
      LSFT, Z,    X,    C,    V,    B,    QUOT, DEL,  K,    M,    COMM, DOT,  SLSH, RSFT,
      LCTL, FN6,  LGUI, LALT, FN1,        SPC,  ENT,        FN2,  LEFT, DOWN, UP,   RGHT),
    [2] = KEYMAP( /* 2: QWERTY->Dvorak */
      ESC,  1,    2,    3,    4,    5,    HOME, RALT, 6,    7,    8,    9,    0,    BSPC,
      FN10, QUOT, COMM, DOT,  P,    Y,    LBRC, RBRC, F,    G,    C,    R,    L,    FN11,
      RCTL, A,    O,    E,    U,    I,    MINS, EQL,  D,    H,    T,    N,    S,    ENT,
      LSFT, SCLN, Q,    J,    K,    X,    QUOT, DEL,  B,    M,    W,    V,    Z,    RSFT,
      LCTL, FN6,  LGUI, LALT, FN1,        SPC,  ENT,        FN2,  LEFT, DOWN, UP,   RGHT),
    [4] = KEYMAP( /* fn1 */
      TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, KP_7, KP_8, KP_9, KP_SLASH,    TRNS, 
	  TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, KP_4, KP_5, KP_6, KP_ASTERISK, TRNS,
      TRNS, MINS, EQL,  FN20, FN21, TRNS, TRNS, TRNS, TRNS, KP_1, KP_2, KP_3, KP_MINUS,    TRNS,
      TRNS, UNDO, AGAIN,GRV,  FN22, TRNS, TRNS, TRNS, TRNS, KP_0, NO, KP_DOT, KP_PLUS,     TRNS,
      TRNS, TRNS, TRNS, TRNS, FN1,        TRNS, BSPC,       FN3,  HOME, PGDN, PGUP,        END),
    [5] = KEYMAP( /* fn2 */
      F13,  F14,  F15,  F16,  F17,  F18,  NO,   NO,   F19,  F20,  F21,  F22,  F23,  F24,
      F1,   F2,   F3,   F4,   F5,   F6,   NO,   NO,   F7,   F8,   F9,   F10,  F11,  F12,
	  TRNS, LBRC, RBRC, FN23, FN24, NO,   NO,   NO,   GRV,  FN22, NO,   NO,   NO,   TRNS,
      TRNS, DEL,  CUT, COPY, PASTE, MENU, NO,   NO,   CAPS, INS,  PSCR, SLCK, PAUS, TRNS,
      TRNS, TRNS, TRNS, TRNS, FN3,        DEL, TRNS,        FN2,  MUTE, VOLD, VOLU, MPLY),
    [6] = KEYMAP( /* fn1+fn2 */
      FN7,  FN8,  FN9,  NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   PWR,
      INT1, INT2, INT3, INT4, INT5, INT6, NO,   NO,   LANG1,LANG2,LANG3,LANG4,LANG5,SLEP,
      INT7, INT8, INT9, NO,   UNDO, NO,   NO,   NO,   LANG6,AGAIN,LANG7,LANG8,LANG9,NO,
      TRNS, NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   WAKE,
      TRNS, TRNS, TRNS, TRNS, FN4,        TRNS, TRNS,       FN5,  MPRV, MPLY, MSTP, MNXT),
    [7] = KEYMAP( /* media */
      FN7,  FN8,  FN9,  NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   PWR,
      NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   SLEP,
      NO,   NO,   NO,   NO,   UNDO, NO,   NO,   NO,   NO,  AGAIN, NO,   NO,   NO,   NO,
      TRNS, NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   NO,   WAKE,
      TRNS, TRNS, TRNS, TRNS, FN4,        TRNS, TRNS,       FN5,  MPRV, MPLY, MSTP, MNXT)
};
const uint16_t PROGMEM fn_actions[] = {
    [1] = ACTION_LAYER_BIT_XOR(1, 0b0001, ON_BOTH), // swap fn1
    [2] = ACTION_LAYER_BIT_XOR(1, 0b0010, ON_BOTH), // swap fn2
    [3] = ACTION_LAYER_BIT_OR( 1, 0b0100, ON_BOTH), // + fn1 + fn2 + fn1+2
    [4] = ACTION_LAYER_BIT_AND(1, 0b1010, ON_BOTH), // - fn1
    [5] = ACTION_LAYER_BIT_AND(1, 0b1001, ON_BOTH), // - fn2
	[6] = ACTION_LAYER_MOMENTARY(7), // randomness
    [7] = ACTION_DEFAULT_LAYER_SET(0),  // set Qwerty layout
    [8] = ACTION_DEFAULT_LAYER_SET(1),  // set Colemak layout
    [9] = ACTION_DEFAULT_LAYER_SET(2),  // set Dvorak layout
    [10] = ACTION_MODS_TAP_KEY(MOD_LALT, KC_TAB),
    [11] = ACTION_MODS_TAP_KEY(MOD_LALT, KC_BSLS),
		
    [20] = ACTION_MODS_KEY(MOD_LSFT, KC_MINS),
    [21] = ACTION_MODS_KEY(MOD_LSFT, KC_EQL),
    [22] = ACTION_MODS_KEY(MOD_LSFT, KC_GRV),
    [23] = ACTION_MODS_KEY(MOD_LSFT, KC_LBRC),
    [24] = ACTION_MODS_KEY(MOD_LSFT, KC_RBRC)
};
