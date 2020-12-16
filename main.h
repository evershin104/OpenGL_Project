#include <glut.h>
#include <vector>
#include <cmath>
#include <string>

#define M_PI 3.14159265358979323846

/* Рандомные координаты частицы */
#define randX rand() % width
#define randY rand() % height

/* Рандомные начальные скорости частицы */
#define randVx rand() % 100000 / 500.0 - 100
#define randVy rand() % 100000 / 500.0 - 100

/* Ширина и высота окна */
constexpr auto width = 768;
constexpr auto height = 768;

using namespace std;

/* Инициализация модели */
void initModel(void);
/* Отрисовка отдельных элементов информации о модели */
void drawText(const char* text, int length, int x, int y);
/* Отрисовка всех элементов информации о модели */
void drawAllText(void);
/* Покадровая отрисовка */
void display(void);
/* Прорисовка следующего кадра */
void timer(int = 0);
/* Взаимодействие пользователя с моделью с помощью клавиш */
void processSpecialKeys(int key, int x, int y);


/* Масса большой частицы */
float bigMass = 30;
/* МАсса мальнькой частицы */
int smallMass = 3;
/* Сила взаимодействия частиц */
int corellation = 300;
/* Количество чатсиц */
int num = 30;

/* Текст для вывода информации о модели */
const char CorText[] = { "Corellation (Right, Left) = " };
const char MassText[] = { "Mass of small particle (Up, Down) = " };
const char NumText[] = { "Number of particlec(PG_UP, PG_DOWN) = " };

/* Единица времени */
const float DT = 0.01;

/* Структура частицы и вектор частиц */
struct P
{
    float x;    // Начальные
    float y;    // координаты частиицы
    float vx;   // Скорости
    float vy;   // частицы
    int m;      // Масса частицы
};
vector<P> p;

void drawText(const char* text, int length, int x, int y)
{
    glMatrixMode(GL_PROJECTION);
    double* matrix = new double[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0, 400, 0, 400, -5, 5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(x, y);
    for (int i = 0; i < length; i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
}
void drawAllText(void)
{
    char CorVal[5];
    sprintf_s(CorVal, size(CorVal), "%d", corellation);
    drawText(&CorText[0], size(CorText), 5, 5);
    drawText(&CorVal[0], size(CorVal), 140, 5);

    char SmallSize[3];
    sprintf_s(SmallSize, size(SmallSize), "%d", smallMass);
    drawText(&SmallSize[0], size(SmallSize), 180, 15);
    drawText(&MassText[0], size(MassText), 5, 15);

    char NumVal[5];
    sprintf_s(NumVal, size(NumVal), "%d", num);
    drawText(&NumVal[0], size(NumVal), 185, 25);
    drawText(&NumText[0], size(NumText), 5, 25);
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    for (vector<P>::iterator i = p.begin(); i != p.end(); ++i)
    {
        /* Отрисовка всех частиц, кроме основной большой */
        if (i->m != bigMass)
        {
            glPointSize(smallMass);
            glEnable(GL_POINT_SMOOTH);
            glBegin(GL_POINTS);
            glVertex2f(i->x, i->y);
            glEnd();
        }
        /* Отрисовка большой частицы */
        else
        {
            glBegin(GL_LINES);
            for (int a = 0; a < 36; ++a)
            {
                float x = i->m * cos(a * M_PI / 18);
                float y = i->m * sin(a * M_PI / 18);
                glVertex2f(i->x + x, i->y + y);
                x = i->m * cos((a + 1) * M_PI / 18);
                y = i->m * sin((a + 1) * M_PI / 18);
                glVertex2f(i->x + x, i->y + y);
            }
            glEnd();
        }
    }
    drawAllText();
    glutSwapBuffers();
}
void timer(int)
{
    for (vector<P>::iterator i = p.begin(); i != p.end(); ++i)
        for (vector<P>::iterator j = p.begin(); j != p.end(); ++j)
            if (i != j)
            {
                /* Расчет расстояния между каждой частицей с каждой */
                float d = sqrt((i->x - j->x) * (i->x - j->x) + (i->y - j->y) * (i->y - j->y));
                /* Взаимодействие внутренних сил частиц */
                if (d < i->m + j->m)
                {
                    float f = corellation * (i->m + j->m - d);
                    i->vx += f * (i->x - j->x) / d / i->m * DT;
                    i->vy += f * (i->y - j->y) / d / i->m * DT;
                    j->vx -= f * (i->x - j->x) / d / j->m * DT;
                    j->vy -= f * (i->y - j->y) / d / j->m * DT;
                }
                /* Обработка коллизии */
                if (d == i->m + j->m)
                {
                    float f = corellation * (i->m + j->m - d);
                    i->vx = -i->vx - f * (i->x - j->x) / d / i->m * DT;
                    i->vy = -i->vy - f * (i->y - j->y) / d / i->m * DT;
                    j->vx = -j->vx + f * (i->x - j->x) / d / j->m * DT;
                    j->vy = -j->vy + f * (i->y - j->y) / d / j->m * DT;
                }
            }
    /* Обработка выхода частицы за пределы экрана */
    for (vector<P>::iterator i = p.begin(); i != p.end(); ++i)
    {
        i->x += i->vx * DT;
        i->y += i->vy * DT;
        if (i->x < 0)
            i->x += width;
        if (i->y < 0)
            i->y += height;
        if (i->x > width)
            i->x -= width;
        if (i->y > height)
            i->y -= height;
    }
    display();
    glutTimerFunc(10, timer, 0);
}
void processSpecialKeys(int key, int x, int y) 
{
    switch (key)
    {
    case GLUT_KEY_UP:
        if (smallMass == 15)
            break;
        smallMass++;
        break;
    case GLUT_KEY_DOWN:
        if (smallMass == 1)
            break;
        smallMass--;
        break;
    case GLUT_KEY_LEFT:
        if (corellation == 30)
            break;
        corellation -= 30;
        break;
    case GLUT_KEY_RIGHT:
        if (corellation == 9000)
            break;
        corellation += 30;
        break;
    case GLUT_KEY_PAGE_UP:
        if (num == 100)
            break;
        p.push_back({ float(randX), float(randY), float(randVx), float(randVy), smallMass });
        num++;
        break;
    case GLUT_KEY_PAGE_DOWN:
        if (num == 1)
            break;
        p.pop_back();
        num--;
        break;
    default:
        break;
    }
}
void initModel(void)
{
    /* Инициализация основной частицы */
    P p0 = { width / 2, height / 2, 0, 0, bigMass };
    p.push_back(p0);
    /* Инициализация мелких частиц */
    for (int i = 0; i < num; ++i)
    {
        P p0 = { randX, randY, randVx, randVy, smallMass};
        p.push_back(p0);
    }
}