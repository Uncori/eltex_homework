#include "../header/cup.h"

#include <stdlib.h>

static int **cup;
static unsigned long score = 0;
static int multi = 1;

void allocate_cup()
{
    cup = (int **)calloc(HEIGHT_CUP, sizeof(int *));
    for (int i = 0; i < HEIGHT_CUP; ++i) {
        cup[i] = (int *)calloc(WIDTH_CUP, sizeof(int));
    }
}

void remove_cup()
{
    for (int i = 0; i < HEIGHT_CUP; ++i) {
        free(cup[i]);
    }
    free(cup);
}

int **get_cup() { return cup; }

int get_score() { return score; }
int set_score(int value) { return score = value; }

void down_score() { score += 1; }

void space_score(int down_before, int down_after)
{
    score += (down_after - down_before) * 2;
}

void row_score(int count_row)
{
    int sum = 0;
    for (int i = 1; i <= count_row; ++i) {
        sum += (i * 100) * multi;
    }
    score += sum;
    multi += 1;
}

void cup_analysis(int up_border, int down_border, int max_height)
{
    /* кол-во единиц в ряду */
    int count_unit = 0;
    /* индекс заполненных рядов единицами в массиве*/
    int index_row_array = 0;
    /* массив индексов заполненных единицами */
    int array_filled_row[MAX_COUNT_FILL_ROW] = {0};

    for (int i = down_border; i >= up_border; --i) {
        for (int j = 0; j < WIDTH_CUP; ++j) {
            if (cup[i][j]) {
                ++count_unit;
            }
            if (count_unit == WIDTH_CUP) {
                array_filled_row[index_row_array] = i;
                ++index_row_array;
            }
        }
        count_unit = 0;
    }

    if (index_row_array) {
        rows_recombine(index_row_array, array_filled_row, max_height);
        row_score(index_row_array);
    } else {
        multi = 1;
    }
}

void rows_recombine(int count_filled_row, int row_for_del[], int max_height)
{
    for (int i = count_filled_row; i >= 0; --i) {
        for (int j = row_for_del[i] - 1; j >= max_height; --j) {
            for (int k = 0; k < WIDTH_CUP; ++k) {
                cup[j + 1][k] = 0;
                if (cup[j][k]) {
                    cup[j + 1][k] = cup[j][k];
                    cup[j][k] = 0;
                }
            }
        }
    }
}

void add_figure(Figure *figure)
{
    for (int i = 0; i < figure->count_point; ++i) {
        int x = figure->point[i].x;
        int y = figure->point[i].y;
        if (x >= 0) {
            cup[x][y] = figure->index_figure;
        }
    }
}
