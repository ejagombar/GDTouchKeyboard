// Copyright (c) GWENDESIGN. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef _GDTOUCHKEYBOARD_H_
#define _GDTOUCHKEYBOARD_H_

#include <M5Core2.h>
#include <Free_Fonts.h>

class GDTouchKeyboard
{
public:
  GDTouchKeyboard();
  ~GDTouchKeyboard();
  void setColor(uint16_t setThemeColor);
  ButtonColors get_bc_on();
  ButtonColors get_bc_off();
  uint16_t getThemeColor();

  String run(String text = "",uint16_t getColor = 0x9ad9);

private:
  ButtonColors _bc_on;
  ButtonColors _bc_off;
  uint16_t themeColor;
};

extern GDTouchKeyboard GDTK;

#endif // _GDTOUCHKEYBOARD_H_
