// Copyright (c) GWENDESIGN. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef _GDTOUCHKEYBOARD_H_
#define _GDTOUCHKEYBOARD_H_

#include <M5Core2.h>
#include <Free_Fonts.h>




void _btnAEvent(Event& e);
void _buttonEvent(Event& e);

class GDTouchKeyboard;

class GDTouchKeyboard
{
public:
  GDTouchKeyboard();
  ~GDTouchKeyboard();

  String run(String text = "",uint16_t setColourIn = 0x0ad9, bool getIsEditable = true, const GFXfont fontIn = FreeMonoBold9pt7b);

  friend void _btnAEvent(Event& e);
  friend void _buttonEvent(Event& e);

private:

  #define KEYBOARD_X (2)
  #define KEYBOARD_Y (26)

  #define KEY_W (45)
  #define KEY_H (50)

  #define COLS (7)
  #define ROWS (4)

  #define MAX_SHIFT_MODE (4)

  const char keymap[MAX_SHIFT_MODE][ROWS][COLS] =
  {
    {
    {'a', 'b', 'c', 'd', 'e', 'f', 'g'},
    {'h', 'i', 'j', 'k', 'l', 'm', 'n'},
    {'o', 'p', 'q', 'r', 's', 't', 'u'},
    {'v', 'w', 'x', 'y', 'z', ' ', '\002'}, // 002 = shift
    },
    {
    {'A', 'B', 'C', 'D', 'E', 'F', 'G'},
    {'H', 'I', 'J', 'K', 'L', 'M', 'N'},
    {'O', 'P', 'Q', 'R', 'S', 'T', 'U'},
    {'V', 'W', 'X', 'Y', 'Z', ' ', '\002'}, // 002 = shift
    },
    {
    {'0', '1', '2', '3', '4', '5', '6'},
    {'7', '8', '9', '`', '-', '=', '['},
    {']', '\\', ';', '\'', ',', '.', '/'},
    {' ', ' ', ' ', ' ', ' ', ' ', '\002'}, // 002 = shift
    },
    {
    {'~', '!', '@', '#', '$', '%', '^'},
    {'&', '*', '(', ')', '_', '+', '{'},
    {'}', '|', ':', '"', '<', '>', '?'},
    {' ', ' ', ' ', ' ', ' ', ' ', '\002'}, // 002 = shift
    },
  };



  void _updateInputText(void);
  void _initKeyboard(String text = "");
  void _deinitKeyboard(void);
  void _drawKeyboard(void);



  typedef enum
  {
    KEY_MODE_LETTER = 0,
    KEY_MODE_NUMBER = 1,
  } key_mode_t;

  ButtonColors _bc_on;
  ButtonColors _bc_off;
  uint16_t themeColor = 0x0ad9;
  Button *_button_list[ROWS][COLS];
  String _input_text = "";
  String _old_input_text = "";
  key_mode_t _key_mode = KEY_MODE_LETTER;
  bool _shift_mode = false;
  bool _keyboard_done = false;
  uint32_t _cursor_last;
  bool _cursor_state = false;
  GFXfont font;
  bool isEditable = false;
  String promptText = "";
};

extern GDTouchKeyboard GDTK;

#endif // _GDTOUCHKEYBOARD_H_
