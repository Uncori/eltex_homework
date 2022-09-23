#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../header/cup.h"
#include "../header/inet.h"
#include "../header/rbtree.h"

static int my_figure_id = 0, next_figure_id = 0;

int set_my_figure_id(int id)
{
    if (!id) {
        fprintf(stderr, "error set_my_figure_id\n");

        return -1;
    }
    my_figure_id = id;

    return 0;
}

int set_next_figure_id(int id)
{
    if (!id) {
        fprintf(stderr, "error set_next_figure_id\n");

        return -1;
    }
    next_figure_id = id;

    return 0;
}

int get_my_figure_id() { return my_figure_id; }

int get_next_figure_id() { return next_figure_id; }

void allocate_figure(Figure *figure, int count_point)
{
    figure->count_point = count_point;
    figure->point = (Figure_point *)calloc(count_point, sizeof(Figure_point));
}

void remove_figure(Figure *figure) { free(figure->point); }

void copy_figure(Figure *copy_figure, Figure *paste_figure)
{
    paste_figure->count_point = copy_figure->count_point;
    paste_figure->index_figure = copy_figure->index_figure;
    paste_figure->turn_point_index = copy_figure->turn_point_index;

    for (int i = 0; i < paste_figure->count_point; ++i) {
        paste_figure->point[i].x = copy_figure->point[i].x;
        paste_figure->point[i].y = copy_figure->point[i].y;
    }
}

void copy_by_figure_id(Figure *figura, int id)
{
    Node *tmp = rbtree_find_figure(get_rbt(), id);
    if (tmp != NULL) {
        figura->count_point = tmp->figura.count_point;
        figura->index_figure = tmp->figura.index_figure;
        figura->turn_point_index = tmp->figura.turn_point_index;
        for (int i = 0; i < figura->count_point; ++i) {
            figura->point[i].x = tmp->figura.point[i].x;
            figura->point[i].y = tmp->figura.point[i].y;
        }
    } else {
        fprintf(stderr, "*figure = %p, id = %d", figura, id);
    }
}

void move_right(Figure *figure)
{
    int breaker = 0, **cup = get_cup();

    for (int i = 0; i < figure->count_point && !breaker; ++i) {
        if (figure->point[i].y >= 0 && figure->point[i].y < WIDTH_CUP - 1 &&
            figure->point[i].x >= 0) {
            if (cup[figure->point[i].x][figure->point[i].y + 1]) {
                breaker = 1;
            }
        }
        if (figure->point[i].y == WIDTH_CUP - 1) {
            breaker = 1;
        }
    }

    if (!breaker) {
        for (int i = 0; i < figure->count_point; ++i) {
            figure->point[i].y += 1;
        }
    }
}

void move_left(Figure *figure)
{
    int breaker = 0, **cup = get_cup();

    for (int i = 0; i < figure->count_point && !breaker; ++i) {
        if (figure->point[i].y >= 0 && figure->point[i].y < WIDTH_CUP - 1 &&
            figure->point[i].x >= 0) {
            if (cup[figure->point[i].x][figure->point[i].y - 1]) {
                breaker = 1;
            }
        }
        if (figure->point[i].y == 0) {
            breaker = 1;
        }
    }

    if (!breaker) {
        for (int i = 0; i < figure->count_point; ++i) {
            figure->point[i].y -= 1;
        }
    }
}

int move_down(Figure *figure)
{
    int down = 0, **cup = get_cup(), down_border = down_border_figure(figure);

    for (int i = 0; i < figure->count_point && !down; ++i) {
        if (down_border < 0) {
            if (cup[down_border + 1][figure->point[i].y]) {
                down = 2;
            }
        }
        if (figure->point[i].x >= 0 && figure->point[i].x < HEIGHT_CUP - 1 &&
            !down) {
            if (cup[figure->point[i].x + 1][figure->point[i].y]) {
                down = 1;
            }
        }
        if (figure->point[i].x == HEIGHT_CUP - 1 && !down) {
            down = 1;
        }
    }

    if (!down) {
        for (int i = 0; i < figure->count_point; ++i) {
            figure->point[i].x += 1;
        }
    }
    int up_border = up_border_figure(figure);
    if (down == 1 && up_border < 0) {
        down = 2;
    }
    return down;
}

void rotation_figure(Figure *figure, double gradus)
{
    if (figure->index_figure != 5) {
        int **cup = get_cup(), breaker = 0;
        gradus = (gradus * PI) / 180;

        Figure tmp;
        allocate_figure(&tmp, 4);
        copy_figure(figure, &tmp);

        int x = figure->point[figure->turn_point_index].x;
        int y = figure->point[figure->turn_point_index].y;

        for (int i = 0; i < figure->count_point && !breaker; ++i) {
            int current_x = figure->point[i].x;
            int current_y = figure->point[i].y;
            int next_x = (current_x - x) * round(cos(gradus)) -
                         (current_y - y) * round(sin(gradus)) + x;
            int next_y = (current_x - x) * round(sin(gradus)) +
                         (current_y - y) * round(cos(gradus)) + y;
            if ((next_y >= 0 && next_y < WIDTH_CUP) && next_x >= 0) {
                if (next_x > HEIGHT_CUP - 1 || cup[next_x][next_y]) {
                    copy_figure(&tmp, figure);
                    remove_figure(&tmp);
                    breaker = 1;
                }
            }
            if (!breaker) {
                figure->point[i].x = next_x;
                figure->point[i].y = next_y;
            }
        }
        if (figure->index_figure == 6 && !breaker) {
            if (figure->turn_point_index == 2) {
                figure->turn_point_index -= 1;
            } else {
                figure->turn_point_index += 1;
            }
        }

        for (int i = 0; i < figure->count_point && !breaker; ++i) {
            if (figure->point[i].y > WIDTH_CUP - 1 || figure->point[i].y < 0) {
                shift_figure(figure, figure->point[i].y);
            }
        }
    }
}

void shift_figure(Figure *figure, int y)
{
    if (y < 0) {
        for (int i = 0; i < figure->count_point; ++i) {
            figure->point[i].y += 1;
        }
    }
    if (y > WIDTH_CUP - 1) {
        for (int i = 0; i < figure->count_point; ++i) {
            figure->point[i].y -= 1;
        }
    }
}

void shift_left(Figure *figure)
{
    for (int i = 0; i < figure->count_point; ++i) {
        figure->point[i].y += 3;
    }
}

void shift_down(Figure *figure)
{
    for (int i = 0; i < figure->count_point; ++i) {
        figure->point[i].x += 4;
    }
}

int up_border_figure(Figure *figure)
{
    int up_border = 20;
    for (int i = 0; i < figure->count_point; ++i) {
        int current_x = figure->point[i].x;
        if (current_x < up_border) {
            up_border = current_x;
        }
    }
    return up_border;
}

int down_border_figure(Figure *figure)
{
    int down_border = -1;
    for (int i = 0; i < figure->count_point; ++i) {
        int current_x = figure->point[i].x;
        if (current_x > down_border) {
            down_border = current_x;
        }
    }
    return down_border;
}

void init_figure()
{

    FILE *file_point;
    file_point = fopen("resource/figure_point.txt", "r");
    if (file_point != NULL) {
        allocate_rbt();
        RBT *tree = get_rbt();
        Figure tmp;
        allocate_figure(&tmp, 4);
        int mass[63] = {0};

        for (int i = 0; !feof(file_point); ++i) {
            fscanf(file_point, "%d", &mass[i]);
        }

        for (int i = 0; i < COUNT_FIGURE - 4; ++i) {
            tmp.index_figure = i + 1;
            tmp.turn_point_index = mass[i * 9];
            for (int j = 0; j < tmp.count_point; ++j) {
                tmp.point[j].x = mass[1 + (j * 2) + (i * 9)];
                tmp.point[j].y = mass[2 + (j * 2) + (i * 9)];
            }
            rbtree_insert(tree, tmp);
        }
        fclose(file_point);
        remove_figure(&tmp);

        for (int i = 0; i < 4; ++i) {
            if (i == 3) {
                random_figure(&tmp, i + 8, i + 2);
            } else {
                random_figure(&tmp, i + 8, i + 1);
            }
            rbtree_insert(tree, tmp);
            remove_figure(&tmp);
        }
    }
}

void random_figure(Figure *figura, int index, int count_point)
{
    allocate_figure(figura, count_point);
    int beg_x = 0 + rand() % (3 - 0 + 1); // A + rand()%(B-A+1)
    int beg_y = 0 + rand() % (3 - 0 + 1); // A + rand()%(B-A+1)
    int check_array[4][4] = {0};
    check_array[beg_x][beg_y] = 1;

    int j = 1;
    figura->index_figure = index;
    figura->count_point = count_point;
    figura->turn_point_index = 0;
    figura->point[0].x = beg_x;
    figura->point[0].y = beg_y;

    while (j < count_point) {
        int x = 0 + rand() % (3 - 0 + 1);
        int y = 0 + rand() % (3 - 0 + 1);
        if (beg_x - x == 1 && beg_y == y && !check_array[x][y]) {
            check_array[x][y] = 1;
            figura->point[j].x = x;
            figura->point[j].y = y;
            ++j;
            beg_x = x;
            beg_y = y;
        } else if (beg_x == x && y - beg_y == 1 && !check_array[x][y]) {
            check_array[x][y] = 1;
            figura->point[j].x = x;
            figura->point[j].y = y;
            ++j;
            beg_x = x;
            beg_y = y;
        } else if (x - beg_x == 1 && beg_y == y && !check_array[x][y]) {
            check_array[x][y] = 1;
            figura->point[j].x = x;
            figura->point[j].y = y;
            ++j;
            beg_x = x;
            beg_y = y;
        } else if (beg_x == x && beg_y - y == 1 && !check_array[x][y]) {
            check_array[x][y] = 1;
            figura->point[j].x = x;
            figura->point[j].y = y;
            ++j;
            beg_x = x;
            beg_y = y;
        }
    }
    check_random_figure(figura);
}

void check_random_figure(Figure *figura)
{
    int down_border = down_border_figure(figura);
    if (down_border >= 0) {
        for (int i = 0; i < figura->count_point; ++i) {
            figura->point[i].x -= down_border + 1;
        }
    }
}
