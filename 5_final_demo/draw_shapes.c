#include "draw_shapes.h"    
#include "switches.h"

short redrawScreen = 1;
u_int controlFontColor = COLOR_GREEN;

// Static Shape Variables
static int square_h = 0, square_w = 0, square_row = 20, square_col = (screenWidth / 4);
static char s1_state = 0, s2_state = 0; // states for squares to determine their color
static int reset_t = 0, s1_color_t = 0, s2_color_t; // timer variables

u_int change_shape_color(char s_state) {
  u_int blue = 31, green = 31, red = 31;
  switch(s_state) {
    case 0 : blue = 31; green = 31; red = 31; break;
    case 1 : blue =  0; green = 31; red =  0; break;
    case 2 : blue =  0; green =  0; red = 31; break;
    default : break;
  }
  return (blue << 11) | (green << 5) | red;
}

void draw_square_2(void) {
  // Draw a square that changes colors
  int height = 30;
  int width  = 30;
  int row = 40, col = (screenWidth / 2) + 40;
  int left_col = col - (width / 2);
  int top_row  = row - (height / 2);

  if (s2_color_t >= 20) {
    s2_color_t = 0;
    s2_state++;
    if (s2_state > 2) {
      s2_state = 0;
    }
    clearScreen(COLOR_BLUE);
  }
  u_int color = change_shape_color(s2_state);
  fillRectangle(left_col, row, width, height, color);
}

void draw_diamond(void) {
  // Shape Variables
  int height, row, col, step;
  u_char start_col, end_col, new_step, width;
  static u_char blue = 31, green = 31, red = 31;

  // Draw an Diamond shape
  height = 40;
  row = 40;
  col = (screenWidth / 2) ;
  step = 0;
  blue = 31, green = 31, red = 31;
  u_int color = (blue << 11) | (green << 5) | red;

  // draw an diamond (code is partially based off lab triangle)
  for (step = 0; step < height; step++) {
    // upper part of diamond
    if (step < (height/2)) {
      start_col    = col - (step / 2);
      end_col      = col + (step / 2);
      width = end_col - start_col;
    // lower part of diamond
    } else {
      int new_step = (height-step);
      start_col    = col - (new_step / 2);
      end_col      = col + (new_step / 2);
      width = end_col - start_col;
    }
    fillRectangle(start_col, row+step, width, 1, color);
  }
}

void draw_arrow(void) {
  // Shape Variables
  int height, width, row, col, step;
  u_char start_col, end_col, new_step;
  static u_char blue = 31, green = 31, red = 31;

  // Draw an Arrow shape
  height = 60;
  row = 80;
  col = (screenWidth / 2) + 20;
  step = 0;
  blue = 31, green = 31, red = 31;
  u_int color = (blue << 11) | (green << 5) | red;

  // draw an arrow (code is partially based off lab triangle)
  for (step = 0; step < height; step++) {
    start_col = col - (step / 2);
    end_col   = col + (step / 2);
    u_char width     = end_col - start_col;
    // lower part of arrow
    if (step >= (height/2)) {
      start_col = col - 4;
      end_col = col + 4;
      width = end_col - start_col;
    }
    fillRectangle(start_col, row+step, width, 1, color);
  }
}

void
update_shape(void)
{
  // Shape Variables
  int height, width, row, col, step;
  u_char start_col, end_col, new_step;
  static u_char blue = 31, green = 31, red = 31;

  // draw shapes on screen
  draw_square_2();
  draw_diamond();
  draw_arrow();
  
  // Text Variables
  const u_char text_row = 20;
  const u_char h_offset = 16; // horizontal offset for columns
  const u_char text_col = 40 + h_offset;
  const u_char char_width = 6;
  u_int text_color = (green << 5) | red;
  
  // Draw the word "state: -" on screen
  drawChar5x7(text_col + (char_width * 0), text_row, 'S',text_color, COLOR_BLUE);
  drawChar5x7(text_col + (char_width * 1), text_row, 'T',text_color, COLOR_BLUE);
  drawChar5x7(text_col + (char_width * 2), text_row, 'A',text_color, COLOR_BLUE);
  drawChar5x7(text_col + (char_width * 3), text_row, 'T',text_color, COLOR_BLUE);
  drawChar5x7(text_col + (char_width * 4), text_row, 'E',text_color, COLOR_BLUE);
  drawChar5x7(text_col + (char_width * 5), text_row, ':',text_color, COLOR_BLUE);
  drawChar5x7(text_col + (char_width * 6), text_row, ' ',text_color, COLOR_BLUE);

  // Change text output & location of shapes based on input 

  int left_col, top_row; // Square variables
  height = 10 + square_h;
  width  = 10 + square_w;
  left_col = square_col - (width / 2);
  top_row  = square_row - (height / 2);

  // button 1 lengthens the square
  if (switch1_down) {
    if (height < 100) { // square height limit
      square_h += 2;
    }
    drawChar5x7(text_col + (char_width * 6), text_row, '1',text_color, COLOR_BLUE);
    s2_color_t = 0;
    s2_state = 0;
  }
  // button 2 widens the square
  else if (switch2_down) {
    if (width < 30) { // square width limit
      square_w += 1;
    }
    drawChar5x7(text_col + (char_width * 6), text_row, '2',text_color, COLOR_BLUE);
    s2_color_t = 0;
    s2_state = 0;
  }
  // button 3 changes the color of the square
  else if (switch3_down) {  
    drawChar5x7(text_col + (char_width * 6), text_row, '3',text_color, COLOR_BLUE);
    s2_color_t = 0;
    s2_state = 0;
    s1_color_t++;
    if (s1_color_t > 20) {
      s1_color_t = 0;
      s1_state++;
      if (s1_state > 2) {
	s1_state = 0;
      }
      clearScreen(COLOR_BLUE);
    }
  }
  // button 4 resets or clears the screen
  else if (switch4_down) {
    drawChar5x7(text_col + (char_width * 6), text_row, '4',text_color, COLOR_BLUE);
    square_h = 0;
    square_w = 0;
    height = 10;
    width = 10;
    s1_color_t = 0;
    s1_state = 0;
    s2_color_t = 0;
    s2_state = 0;
    clearScreen(COLOR_BLUE);
  }
  else {
    drawChar5x7(text_col + (char_width * 6), text_row, '-',text_color, COLOR_BLUE);
    if (s2_color_t < 20) { s2_color_t++; }
  }
  u_int color = change_shape_color(s1_state);
  fillRectangle(left_col, square_row, width, height, color);
}
