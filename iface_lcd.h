#ifndef __INCLUDE_IFACE_LCD_H_
#define __INCLUDE_IFACE_LCD_H_

#define LCD_WIDTH 16
#define LCD_HEIGHT 2

void lcdWrite(char *txbuf);
void lcdClear();
void lcdMoveLeft();
void lcdMoveRight();
void lcdScrollLeft();
void lcdScrollRight();
void lcdVisualDisplayOn();
void lcdVisualDisplayOff();
void lcdUnderlineCursorOn();
void lcdUnderlineCursorOff();
void lcdBoxCursorOn();
void lcdBoxCursorOff();
#endif
