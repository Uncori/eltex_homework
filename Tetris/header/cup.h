#ifndef CUP_H_
#define CUP_H_

#include "figure.h"

/* MAX_COUNT_FILL_ROW - макс кол-во заполненных рядов за 1 ход фигуры */
#define MAX_COUNT_FILL_ROW 4
#define HEIGHT_CUP 20 // высота стакана
#define WIDTH_CUP 10  // ширина стакана

/* выделение памяти под стакан */
void allocate_cup();
/* освобождение памяти стакана*/
void remove_cup();
/* получение указателя стакана*/
int **get_cup();
/* анализ стакана после падения фигуры */
void cup_analysis(int up_border, int down_border, int max_height);
/* удаление заполенных линий и смещение фигур*/
void rows_recombine(int count_filled_row, int row_for_del[], int max_height);
/* добавление фигуры в стакан */
void add_figure(Figure *figure);

/* функции очков */
int get_score();
int set_score(int value);
void down_score();
void space_score(int down_before, int down_after);
void row_score(int count_row);

#endif // CUP_H_
