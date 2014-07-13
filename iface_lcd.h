#ifndef __INCLUDE_IFACE_LCD_H_
#define __INCLUDE_IFACE_LCD_H_

#define LCD_WIDTH 16
#define LCD_HEIGHT 2

void lcdWrite(char const *txbuf);
void lcdWriteWide(char const *txbuf, size_t len, char line);
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
void lcdSetBrightness(char level);
void lcdSetPosition(char line, char column);
#endif
