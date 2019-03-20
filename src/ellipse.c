
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/
 
#include <graphics.h>
#include <stdio.h>
#include <system.h>
#include <math.h>

/* ------------------------------------------------------------------------
 *  函数名：
 *      void Ellipse(int a, int b, int r1, int r2)
 *  功能：
 *      画一个椭圆
 *  参数：
 *      a               ----    椭圆中心的x坐标
 *      b               ----    椭圆中心的y坐标
 *      r1              ----    椭圆长轴半径长度
 *      r2              ----    椭圆短轴半径长度
 *  返回值：
 *      无
 *  说明：
 *      无
 * -----------------------------------------------------------------------*/

void Ellipse(int a, int b, int r1, int r2)
{
    int i, j, k, l, x, y, oldi = 0, oldj = 0, oldk = 0, oldl = 0;
    int long p, q, r, s;
    for (x = 1;x <= r1;x++) {
        s = (r1 * r1) - (x * x);
        q = r2 * r2;
        r = r1 * r1;
        p = (q * s) / r;
        y = sqrt(p);
        i = a + x;
        k = a - x;
        j = b + y;
        l = b - y;
        if (x == 1) {
            Pixel(k + 1, l);
            Pixel(k + 1, j);
        }
        if (x > 1) {
            Line(i, j, oldi, oldj);
            Line(i, l, oldi, oldl);
            Line(k, j, oldk, oldj);
            Line(k, l, oldk, oldl);
        }
        oldi = i;
        oldj = j;
        oldk = k;
        oldl = l;
    }
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      void FillEllipse(int a, int b, int r1, int r2)
 *  功能：
 *      填充一个椭圆
 *  参数：
 *      a               ----    椭圆中心的x坐标
 *      b               ----    椭圆中心的y坐标
 *      r1              ----    椭圆长轴半径长度
 *      r2              ----    椭圆短轴半径长度
 *  返回值：
 *      无
 *  说明：
 *      无
 * -----------------------------------------------------------------------*/

void FillEllipse(int a, int b, int r1, int r2)
{
    int i, j, k, l, x, y, oldi = 0, oldj = 0, oldk = 0, oldl = 0;
    int long p, q, r, s;
    Line(a, b + r2 - 1, a, b - r2 + 1);
    for (x = 1;x <= r1;x++) {
        s = (r1 * r1) - (x * x);
        q = r2 * r2;
        r = r1 * r1;
        p = (q * s) / r;
        y = sqrt(p);
        i = a + x;
        k = a - x;
        j = b + y;
        l = b - y;
        Pixel(i, j);
        Pixel(i, l);
        Pixel(k, j);
        Pixel(k, l);
        if (x != r1) {
            Line(k, j, k, l);
            Line(i, j, i, l);
        }
        if (x == 1) {
            Pixel(k + 1, l);
            Pixel(k + 1, j);
        }
        if (x > 1) {
            Line(i, j, oldi, oldj);
            Line(i, l, oldi, oldl);
            Line(k, j, oldk, oldj);
            Line(k, l, oldk, oldl);
        }
        oldi = i;
        oldj = j;
        oldk = k;
        oldl = l;
    }
}

