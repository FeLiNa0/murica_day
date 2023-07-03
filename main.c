#include <assert.h>
#include <stdio.h>

#define ORIGINAL 13

#define ENABLE_COLORS true

#ifdef ENABLE_COLORS
#define RESET_COLORS "\x1b[0m"
#define BOLD_TERM "\x1b[1m"
#else
#define RESET_COLORS ""
#define BOLD_TERM ""
#endif

// https://en.wikipedia.org/wiki/X_Macro
#define FOR_EACH_COLOR(DO) \
  DO(black, 0, 0, 0) \
  DO(white, 255, 255, 255) \
  DO(red, 255, 0, 0) \
  DO(green, 0, 255, 0) \
  DO(blue, 0, 0, 255)

#define FOR_EACH_MURICA_COLOR(DO) \
  DO(red, 255, 0, 0) \
  DO(white, 255, 255, 255) \
  DO(blue, 0, 0, 255)

#define DEFINE_INT(name, r, g, b) int name = b + g * 0xff + r * 0xffff;
FOR_EACH_COLOR(DEFINE_INT)

#ifdef ENABLE_COLORS
// Using macro system to do variable name generation with ##
// Using macro system to do string interpolation with #
#define DEFINE_BACK_TERM_COLOR(name, r, g, b) char* name ## _back_term_color = "\x1b[48;2;" #r ";" #g ";" #b "m";
#define DEFINE_FORE_TERM_COLOR(name, r, g, b) char* name ## _fore_term_color = "\x1b[38;2;" #r ";" #g ";" #b "m";
#else
#define DEFINE_BACK_TERM_COLOR(name, r, g, b) char* name ## _back_term_color = "";
#define DEFINE_FORE_TERM_COLOR(name, r, g, b) char* name ## _fore_term_color = "";
#endif

FOR_EACH_COLOR(DEFINE_BACK_TERM_COLOR)
FOR_EACH_COLOR(DEFINE_FORE_TERM_COLOR)

void draw_flag(unsigned state_count) {
  assert(state_count == 50);

  unsigned star_count = 0;

  // Draw each row of stars
  // Draw red or white stripes to the right of stars
  unsigned total_columns = 50;
  // One stripe per row of stars leaves just 4 long stripes below the stars
  // Usually it's 7 stripes and 9 rows of stars, here it's 9 stripes for each 9 star-rows
  unsigned total_star_rows = 9;
  unsigned remaining_stripes = ORIGINAL - total_star_rows;
  unsigned row = 1;
  for (; row <= total_star_rows; row++) {
    unsigned even_row = row % 2 == 0;
    unsigned stars_in_row = 6;
    
    unsigned column = 1;

    if (even_row) {
      column += 1;
      stars_in_row = 5;
      printf("%s ", blue_back_term_color);
    }

    for (; column <= stars_in_row * 2; column += 2) {
      star_count++;
      printf("%s %sx", blue_back_term_color, white_fore_term_color);
    }
    
    for (unsigned j = 0; j < (even_row ? 2 : 1); j++) {
      column += 1;
      printf("%s ", blue_back_term_color);
    }

    for (; column <= total_columns; column++) {
      printf("%s_", even_row ? white_back_term_color : red_back_term_color);
    }
    printf("\n");
  }

  assert(star_count == state_count);
  
  // Draw remaining red or white stripes
  for (; row <= total_star_rows + remaining_stripes; row++) {
    unsigned even_row = row % 2 == 0;
    for (unsigned column = 1; column <= total_columns; column++) {
      printf("%s_", even_row ? white_back_term_color : red_back_term_color);
    }
    printf("\n");
  }
}

int main(void) {
#define PRINT_RGB_NAME(name, r, g, b) printf("%s %s ", name ## _back_term_color, #name);
#define COMMA_SEP_COL_LIST(name, r, g, b) name ## _back_term_color,

  printf("%sHappy %s%s%s'MU%sRI%sCA%s %sd%sa%sy%s!!%s!!%s!!\n%s",
    RESET_COLORS,
    black_back_term_color,
    BOLD_TERM,
    FOR_EACH_MURICA_COLOR(COMMA_SEP_COL_LIST)
    RESET_COLORS,
    FOR_EACH_MURICA_COLOR(COMMA_SEP_COL_LIST)
    FOR_EACH_MURICA_COLOR(COMMA_SEP_COL_LIST)
    // The FOR_EACH_MURICA_COLOR(COMMA_SEP_COL_LIST) macro will include a trailing comma,
    // so add a trailing empty string to finish the list of printf params
    RESET_COLORS
  );
  FOR_EACH_MURICA_COLOR(PRINT_RGB_NAME)
  printf("\n");

  draw_flag(50);
  
  return 0;
}
