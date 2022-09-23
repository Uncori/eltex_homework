#ifndef FIGURE_H_
#define FIGURE_H_

#define PI 3.1415926535 // число пи для поворотов
#define COUNT_FIGURE 11 // кол-во стандартных фигур
#define CLOCKWISE -90   // по часовой
#define COUNTCLOWISE 90 // против часовой

typedef struct {
    int x;
    int y;
} Figure_point;

typedef struct {
    int index_figure;     // индекс фигуры
    int turn_point_index; // точка поворота фигуры
    int count_point;      // кол-во точек
    Figure_point *point; // массив структур точек фигуры
} Figure;

int get_my_figure_id();
int get_next_figure_id();
int set_my_figure_id(int id);
int set_next_figure_id(int id);

/* выделение памяти под структуры точек */
void allocate_figure(Figure *figure, int count_point);
/* освобождение памяти структур точек */
void remove_figure(Figure *figure);
/* копирование одной фигуры в другую */
void copy_figure(Figure *copy_figure, Figure *paste_figure);
/* создать фигуру и передать в неё */
void copy_by_figure_id(Figure *figura, int id);

/* движение фигуры */
void move_right(Figure *figure);
void move_left(Figure *figure);
int move_down(Figure *figure);
/* поворот фигуры */
void rotation_figure(Figure *figure, double gradus);
/* сдвиг фигуры от стены */
void shift_figure(Figure *figure, int y);
/* сдвиг фигуры к центру */
void shift_left(Figure *figure);
/* сдвиг для смещения фигуры в окне */
void shift_down(Figure *figure);

/* получение верхней и нижней точки фигуры*/
int up_border_figure(Figure *figure);
int down_border_figure(Figure *figure);

/* вызов функций инициализации*/
void init_figure();
void random_figure(Figure *figura, int index, int count_point);
void check_random_figure(Figure *figura);

#endif // FIGURE_H_
