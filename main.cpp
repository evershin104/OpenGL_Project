/* 
 * �������� ������������ �������� ������ �� ����� C++ 
 * � �������������� ���������� OpenGL.
 * 
 * ����� �������� ������� ���������� � main.h � ����� 30.
 * ����������� ���������� ����� �������������� ���� �������������� ������.
 * ��� ����������� �������������� ����� ����������� ����������
 * ������ ���� ����� �������������� ����� ��������� ������.
 * 
 * ��������/������ ������� -- PageUp/PageDown
 * ���������/��������� ����� ��������� ������ -- Up/Down
 * ���������/��������� ����������� ���������� -- Right/Left
 * 
 * Copyright (c) 2020, Vershinin Eldar <evershin@cs.petrsu.ru>
 *
 * This code is licensed under a MIT-style license.
 */

#include "main.h"

int main(int argc, char** argv)
{
    initModel();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Brownian motion");
    glClearColor(0, 0, 0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 480, 480, 0, -1, 1);
    glutDisplayFunc(display);
    timer(0);
    glutSpecialFunc(processSpecialKeys);
    glutMainLoop();
}
