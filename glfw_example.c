#include <stdint.h>
#include <math.h>
#include <GLFW/glfw3.h>

#include "trifonts.h"

#ifndef M_PI
#define M_PI 3.141592653
#endif
#define M_PI2 (M_PI*2)

#define WINDOW_NAME "font test"

#define SCREEN_X   520
#define SCREEN_Y   260

#define LINE_WIDTH 2.0f

#define SCALE_X 4.0f
#define SCALE_Y 4.0f

#define COLOR_R 1.0f
#define COLOR_G 1.0f
#define COLOR_B 1.0f

#define POS_X0  0.0f 
#define POS_Y0  0.0f

#define SAMPLE_TEXT_LINE    8
#define SAMPLE_TEXT_LENGTH  (16+1)

#define CIRCLE_RESO 90
#define CIRCLE_STEP (2*M_PI/CIRCLE_RESO)

static char sample_text[SAMPLE_TEXT_LINE][SAMPLE_TEXT_LENGTH] = {
    " !\"#$%&'()*+,-./",
    "0123456789:;<=>?",
    "@ABCDEFGHIJKLMNO",
    "PQRSTUVWXYZ[\\]^_",
    "`abcdefghijklmno",
    "pqrstuvwxyz{|}~",
    "Hello, World!",
    "(:v;)<^o^\">[@_@]",
};

static void draw_circle(float x, float y, float x_radius, float y_radius, float deg_start, float deg_end, float rgb[3])
{
    glBegin(GL_LINE_STRIP);
        if( deg_end <= deg_start ) deg_end += 360;
        glColor3f(rgb[0], rgb[1], rgb[2]);
        for (double i = deg_start * M_PI2 / 360; i <= deg_end * M_PI2 / 360; i += CIRCLE_STEP) {
            glVertex2f(x + x_radius * cos(i), y - y_radius * sin(i));
        }
    glEnd();
}

static void draw_line(float x0, float y0, float x1, float y1, float rgb[3])
{
    glBegin(GL_LINE_STRIP);
        glColor3f(rgb[0], rgb[1], rgb[2]);
        glVertex2f(x0, y0);
        glVertex2f(x1, y1);
    glEnd();
}

static void draw_char(float x0, float y0, float scale_x, float scale_y, float color[3], int8_t c)
{
    c -= 32;
    if( (c >= 0) && (c <= 96) ) {
        uint32_t font = c * TF_STROKE;
        for( uint8_t i = 0; i < TF_STROKE; i++ ) {
            trifont dat = Trifont[font+i];
            if( TF_CHK(dat)) {
                draw_line(dat.l.x0*scale_x + x0, dat.l.y0*scale_y + y0,
                          dat.l.x1*scale_x + x0, dat.l.y1*scale_y + y0, color);
            }
            else {
                float rad = dat.c.rad ? dat.c.rad : 0.5f;
                draw_circle(dat.c.x0*scale_x + x0, dat.c.y0*scale_y + y0,
                            rad*scale_x,rad*scale_y,
                            dat.c.a0*45, dat.c.a1*45, color);
            }
        }
    }
}

static void draw_string(float x0, float y0, float scale_x, float scale_y, float color[3], char *c)
{
    uint32_t n = 0;
    while( c[n] != '\0' ) {
        draw_char(x0 + n * TF_X * scale_x, y0, scale_x, scale_y, color, c[n]);
        n++;
    }
}

void draw_sample_text(void)
{
    float color[3] = {COLOR_R,COLOR_G,COLOR_B};

    for( uint8_t i = 0; i < SAMPLE_TEXT_LINE; i++ ){
        draw_string(POS_X0, POS_Y0+TF_Y*i*SCALE_Y,SCALE_X,SCALE_Y,color,sample_text[i]);
    }
}

int main(void)
{
    if (!glfwInit()) {
        return 1;
    }

    GLFWwindow *window = glfwCreateWindow(SCREEN_X, SCREEN_Y, WINDOW_NAME, NULL, NULL);

    if (!window) {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    glDisable(GL_DEPTH_TEST);
    glLineWidth(LINE_WIDTH);

    while (!glfwWindowShouldClose(window)) {
        int32_t width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, height, 0, 0, 1);
        glMatrixMode(GL_MODELVIEW);

        draw_sample_text();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

