// -*- mode: c++ -*-
// Copyright 2017 Amos King <amos@binarynoggin.com>
// See "LICENSE" for license details

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built"
#endif

#include "Kaleidoscope.h"
#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-Numlock.h"
#include "LED-Off.h"
#include "Kaleidoscope-LEDEffect-BootGreeting.h"
#include "Kaleidoscope-LEDEffect-Breathe.h"
#include "Kaleidoscope-LED-Stalker.h"
#include "Kaleidoscope-Model01-TestMode.h"
#include "Kaleidoscope-Model01-TestMode.h"

enum { MACRO_VERSION_INFO,
       MACRO_ANY,
       MACRO_PIPE,
       SPACE_PERIOD,
       MACRO_PERIOD_SPACE,
       NUM_MOVE,
       AUDACITY_CUT_ALL_TRACKS,
       AUDACITY_SILENCE_TRACK
     };

enum { QWERTY, FUNCTION, NUMPAD, NUMBERS }; // layers

/* This comment temporarily turns off astyle's indent enforcement
     so we can make the keymaps actually resemble the physical key layout better
*/
// *INDENT-OFF*

const Key keymaps[][ROWS][COLS] PROGMEM = {

  [QWERTY] = KEYMAP_STACKED
  (Key_Escape,   M(AUDACITY_CUT_ALL_TRACKS), M(AUDACITY_SILENCE_TRACK), XXX, XXX, XXX, Key_LEDEffectNext,
   Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_PageUp,   Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_PageDown, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,
   Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
   ShiftToLayer(FUNCTION),

   M(MACRO_ANY),  XXX, XXX, XXX,     XXX,         XXX,         LCTRL(Key_D),
   Key_Enter,     Key_Y, Key_U, Key_I,     Key_O,         Key_P,         Key_Equals,
                  Key_H, Key_J, Key_K,     Key_L,         Key_Semicolon, Key_Quote,
   LGUI(LALT(LCTRL(Key_Spacebar))),  Key_N, Key_M, Key_Comma, Key_Period,    Key_Slash,     Key_Minus,
   Key_RightShift, Key_LeftAlt, Key_Spacebar, Key_RightControl,
   ShiftToLayer(NUMBERS)),

  [FUNCTION] =  KEYMAP_STACKED
  (___,          Key_F1, Key_F2, Key_F3, Key_F4, Key_F5, Key_LEDEffectNext,
   Key_Tab,  M(MACRO_PIPE),              Key_mouseUp, ___,        Key_mouseBtnR, Key_mouseWarpEnd, Key_mouseWarpNE,
   Key_Home, Key_mouseL,       Key_mouseDn, Key_mouseR, Key_mouseBtnL, Key_mouseWarpNW,
   Key_End,  Key_PrintScreen,  Key_Insert,  ___,        Key_mouseBtnM, Key_mouseWarpSW,  Key_mouseWarpSE,
   ___, Key_Delete, ___, ___,
   ___,

   Consumer_ScanPreviousTrack, Key_F6,                Key_F7,                  Key_F8,                  Key_F9,         Key_F10,          Key_F11,
   Consumer_PlaySlashPause,    Consumer_ScanNextTrack, Key_LeftCurlyBracket,     Key_RightCurlyBracket,    Key_LeftBracket, Key_RightBracket, Key_F12,
                               Key_LeftArrow,          Key_DownArrow,            Key_UpArrow,              Key_RightArrow,  ___,              M(MACRO_PIPE),
   Key_PcApplication,          Key_Mute,               Consumer_VolumeDecrement, Consumer_VolumeIncrement, ___,             Key_Backslash,    Key_Pipe,
   ___, ___, Key_Enter, ___,
   ___),


  [NUMPAD] =  KEYMAP_STACKED
  (___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___,
   ___,

   M(MACRO_VERSION_INFO),  ___, Key_Keypad7, Key_Keypad8,   Key_Keypad9,        Key_KeypadSubtract, ___,
   ___,                    ___, Key_Keypad4, Key_Keypad5,   Key_Keypad6,        Key_KeypadAdd,      ___,
                           ___, Key_Keypad1, Key_Keypad2,   Key_Keypad3,        Key_Equals,         Key_Quote,
   ___,                    ___, Key_Keypad0, Key_KeypadDot, Key_KeypadMultiply, Key_KeypadDivide,   Key_Enter,
   ___, ___, ___, ___,
   ___),

  [NUMBERS] =  KEYMAP_STACKED
  (___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, Key_1, Key_2, Key_3, Key_4, Key_5,
   ___, ___, ___, ___, ___, ___, ___,
   Key_LeftControl, Key_Backspace, Key_LeftGui, Key_LeftShift,
   ___,

   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   Key_6, Key_7, Key_8, Key_9, Key_0, ___,
   ___, ___, ___, ___, ___, ___, ___,
   Key_RightShift, Key_LeftAlt, Key_Spacebar, Key_RightControl,
   ___)
};

/* Re-enable astyle's indent enforcement */
// *INDENT-ON*

/** versionInfoMacro handles the 'firmware version info' macro
    When a key bound to the macro is pressed, this macro
    prints out the firmware build information as virtual keystrokes
*/

static void versionInfoMacro(uint8_t keyState) {
  if (keyToggledOn(keyState)) {
    Macros.type(PSTR("Keyboardio Model 01 - Kaleidoscope "));
    Macros.type(PSTR(BUILD_INFORMATION));
  }
}

/** pipeMacro handles the 'elixir pipe' macro
    When a key bound to the macro is pressed, this macro
    prints out a Elixir pipe "|>" as virtual keystrokes.
*/
static void pipeMacro(uint8_t keyState) {
  if (keyToggledOn(keyState)) {
    Macros.type(PSTR("|>"));
  }
}

static void numberMovedError(uint8_t keyState) {
  static uint16_t errorTimeout, col;
  if (keyToggledOn(keyState)) {
    errorTimeout = millis() + 1000;
    col = Macros.col;
  }

  if (errorTimeout && millis() < errorTimeout) {
    LEDControl.setCrgbAt(2, col, CRGB(0, 255, 0));
    LEDControl.setCrgbAt(0, col, CRGB(255, 0, 0));
  } else if (millis() >= errorTimeout) {
    LEDControl.refreshAt(2, col);
    LEDControl.refreshAt(0, col);
    errorTimeout = 0;
  }
}

/** anyKeyMacro is used to provide the functionality of the 'Any' key.

   When the 'any key' macro is toggled on, a random alphanumeric key is
   selected. While the key is held, the function generates a synthetic
   keypress event repeating that randomly selected key.

*/

static void anyKeyMacro(uint8_t keyState) {
  static Key lastKey;
  if (keyToggledOn(keyState))
    lastKey.keyCode = Key_A.keyCode + (uint8_t)(millis() % 36);

  if (keyIsPressed(keyState))
    kaleidoscope::hid::pressKey(lastKey);
}


/** macroAction dispatches keymap events that are tied to a macro
    to that macro. It takes two uint8_t parameters.

    The first is the macro being called (the entry in the 'enum' earlier in this file).
    The second is the state of the keyswitch. You can use the keyswitch state to figure out
    if the key has just been toggled on, is currently pressed or if it's just been released.

    The 'switch' statement should have a 'case' for each entry of the macro enum.
    Each 'case' statement should call out to a function to handle the macro in question.

*/

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  switch (macroIndex) {

    case MACRO_VERSION_INFO:
      versionInfoMacro(keyState);
      break;

    case MACRO_ANY:
      anyKeyMacro(keyState);
      break;

    case MACRO_PIPE:
      pipeMacro(keyState);
      break;

    case AUDACITY_CUT_ALL_TRACKS:
      return MACRODOWN(
          T(P),
          W(100),
          D(LeftGui),
          D(LeftShift),
          T(K),
          U(LeftShift),
          T(K),
          U(LeftGui)
          );
      break;

    case AUDACITY_SILENCE_TRACK:
      return MACRODOWN(
        T(P),
        W(100),
        D(LeftGui),
        T(L),
        U(LeftGui)
        /*W(100),*/
        /*D(LeftShift),*/
        /*T(K),*/
        /*U(LeftShift),*/
        /*W(100),*/
        /*T(Space)*/
      );
      break;

    case NUM_MOVE:
      numberMovedError(keyState);
      break;
  }
  return MACRO_NONE;
}

static void lightNumberLayer() {
  LEDControl.setCrgbAt(2, 1, CRGB(255, 0, 0));
  LEDControl.setCrgbAt(2, 2, CRGB(255, 0, 0));
  LEDControl.setCrgbAt(2, 3, CRGB(255, 0, 0));
  LEDControl.setCrgbAt(2, 4, CRGB(255, 0, 0));
  LEDControl.setCrgbAt(2, 5, CRGB(255, 0, 0));

  LEDControl.setCrgbAt(2, 10, CRGB(255, 0, 0));
  LEDControl.setCrgbAt(2, 11, CRGB(255, 0, 0));
  LEDControl.setCrgbAt(2, 12, CRGB(255, 0, 0));
  LEDControl.setCrgbAt(2, 13, CRGB(255, 0, 0));
  LEDControl.setCrgbAt(2, 14, CRGB(255, 0, 0));
  return;
}

static void layerColorOverride(bool post_clear) {
  if (!post_clear)
    return;
  if (Layer.isOn(NUMBERS)) {
    lightNumberLayer();
    return;
  }

  return;
}

void setup() {
  Kaleidoscope.setup();

  Kaleidoscope.use(
    &BootGreetingEffect,
    &TestMode,
    &LEDControl,
    &LEDOff,
    &LEDBreatheEffect,
    &StalkerEffect,
    &NumLock,
    &Macros,
    &MouseKeys
  );

  NumLock.numPadLayer = NUMPAD;

  StalkerEffect.variant = STALKER(Haunt);

  Kaleidoscope.useLoopHook(layerColorOverride);

  LEDOff.activate();
}

void loop() {
  Kaleidoscope.loop();
}
