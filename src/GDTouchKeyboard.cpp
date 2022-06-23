// Copyright (c) GWENDESIGN. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "GDTouchKeyboard.h"

void _btnAEvent(Event& e);
void _buttonEvent(Event& e);


GDTouchKeyboard::GDTouchKeyboard()
{
}

GDTouchKeyboard::~GDTouchKeyboard()
{
}

String GDTouchKeyboard::run(String text, uint16_t setColourIn, const GFXfont fontIn)
{
  font = fontIn;
  _bc_on = {setColourIn, WHITE, setColourIn};
  _bc_off = {BLACK, WHITE, setColourIn};
  themeColor = setColourIn;
  _initKeyboard(text);
  _drawKeyboard();
  _keyboard_done = false;
  while(_keyboard_done == false)
  {
    M5.update();

    // Blinking cursor
    if(millis() > _cursor_last)
    {
      _cursor_last = millis() + 500;
      _cursor_state = !_cursor_state;
      _updateInputText();
    }
  }
  while(M5.BtnB.isPressed())
  {
    M5.update();
  }
  _deinitKeyboard();
  return _input_text;
}

void GDTouchKeyboard::_updateInputText()
{
  int oitw = M5.Lcd.textWidth(_old_input_text);
  int itw = M5.Lcd.textWidth(_input_text);

  // Hack to work around incorrect width returned by textWidth()
  //  when space char is at the end of input text
  if(_input_text.endsWith(" ") != 0)
  {
    itw += 14;
  }

  M5.Lcd.setFreeFont(&font);
  M5.Lcd.setTextSize(1);
  if(_old_input_text != _input_text)
  {
    _old_input_text = _input_text;
    M5.Lcd.fillRect(0, 0, max(oitw, itw) + 40, KEYBOARD_Y - 1, TFT_BLACK);
    M5.Lcd.drawString(_input_text, 0, 10);
  }
  else
  {
    if(_cursor_state == true)
    {
      M5.Lcd.fillRect(itw + 2, 2, 15, KEYBOARD_Y - 6, themeColor);
    }
    else
    {
      M5.Lcd.fillRect(itw + 2, 2, 15, KEYBOARD_Y - 6, TFT_BLACK);
    }
  }
}

void GDTouchKeyboard::_initKeyboard(String text)
{
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.setFreeFont(&font);
  M5.Lcd.setTextDatum(TC_DATUM);

  // Button A
  M5.Lcd.drawString("Delete", 55, 226,2);
  // Button B
  M5.Lcd.drawString("Done", 160, 226,2);
  // Button C
  M5.Lcd.drawString("Mode", 265, 226,2);

  for(int r = 0; r < ROWS; r++)
  {
    for(int c = 0; c < COLS; c++)
    {
      
      _button_list[r][c] = new Button(0, 0, 0, 0, false, "",_bc_off, _bc_on);
      _button_list[r][c]->setTextSize(1);
    }
  }

  M5.Buttons.addHandler(_buttonEvent, E_TOUCH);
  M5.Buttons.addHandler(_btnAEvent, E_RELEASE);

  _input_text = text;
  _key_mode = KEY_MODE_LETTER;
  _shift_mode = false;
}

void GDTouchKeyboard::_deinitKeyboard()
{
  M5.Buttons.delHandlers(_buttonEvent, nullptr, nullptr);
  M5.Buttons.delHandlers(_btnAEvent, nullptr, nullptr);

  for(int r = 0; r < ROWS; r++)
  {
    for(int c = 0; c < COLS; c++)
    {
      delete(_button_list[r][c]);
      _button_list[r][c] = NULL;
    }
  }
}

void GDTouchKeyboard::_drawKeyboard()
{
  int x, y;

  for(int r = 0; r < ROWS; r++)
  {
    for(int c = 0; c < COLS; c++)
    {
      x = (KEYBOARD_X + (c * KEY_W));
      y = (KEYBOARD_Y + (r * KEY_H));
      _button_list[r][c]->set(x, y, KEY_W, KEY_H);

      int key_page = 0;

      if(_key_mode == KEY_MODE_NUMBER) key_page += 2;
      if(_shift_mode == true) key_page += 1;

      String key;
      char ch = keymap[key_page][r][c];

      if(ch == '\002')  // Shift
      {
        _button_list[r][c]->setFreeFont(&font);
        _button_list[r][c]->setTextSize(1);
        key = "SHFT";
      }
      else
      {
        _button_list[r][c]->setFreeFont(&font);
        _button_list[r][c]->setTextSize(2);

        key = String(ch);
      }
      _button_list[r][c]->setLabel(key.c_str());
      _button_list[r][c]->draw();
    }
  }
}

void _btnAEvent(Event& e)
{
  // Delete all (long press) or delete one char (short press)
  if(e.button == &M5.BtnA)
  {
    if(e.duration > 500)
    {
      GDTK._input_text="";
    }
    else
    {
      GDTK._input_text = GDTK._input_text.substring(0, GDTK._input_text.length() - 1);
    }
    GDTK._updateInputText();
  }
}

void _buttonEvent(Event& e)
{
  Button& b = *e.button;

  // Delete
  if(e.button == &M5.BtnA)
  {
    // Ignore - handled in btnAEvent()
    return;
  }
  // Done
  else if(e.button == &M5.BtnB)
  {
    GDTK._keyboard_done = true;
    return;
  }
  // Key mode
  else if(e.button == &M5.BtnC)
  {
    switch(GDTK._key_mode)
    {
      case GDTK.KEY_MODE_LETTER:
        GDTK._key_mode = GDTK.KEY_MODE_NUMBER;
        break;
      default:
      case GDTK.KEY_MODE_NUMBER:
        GDTK._key_mode = GDTK.KEY_MODE_LETTER;
        break;
    }
    GDTK._shift_mode = false;
    GDTK._drawKeyboard();
    return;
  }
  else if(e.button == &M5.background)
  {
    // Ignore default background button
    return;
  }
  else
  {
    if(String(b.label()) == "SHFT")
    {
      GDTK._shift_mode = !GDTK._shift_mode;
      GDTK._drawKeyboard();
      return;
    }
    GDTK._input_text += b.label();
  }
  GDTK._updateInputText();
}

GDTouchKeyboard GDTK;
