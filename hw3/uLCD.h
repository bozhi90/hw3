#include "uLCD_4DGL.h"

extern bool runUI;
extern int sel;
extern int select_index;
extern float angle;

uLCD_4DGL uLCD(D1, D0, D2);

void uLCD_initial(void) {
    uLCD.reset();
    uLCD.background_color(BLACK);
    uLCD.textbackground_color(BLACK);
    uLCD.cls();
    uLCD.color(WHITE);

    uLCD.locate(0, 0);
    uLCD.printf("    angle = 30\n\n");
    uLCD.printf("    angle = 40\n\n");
    uLCD.printf("    angle = 50\n\n");
    uLCD.printf("  init_X = 0000\n  init_Y = 0000\n  init_Z = 0000\n\n");
    uLCD.printf("  now_X  = 0000\n  now_Y  = 0000\n  now_Z  = 0000\n\n");
}

void function_uLCD () {
    uLCD_initial();

    int pre_sel = 0;
    float a;
    int16_t pre_XYZ = 0;

    while (1) {
        ThisThread::sleep_for(50ms);
        if (runUI && pre_sel != sel) {
            uLCD.locate(1, 2*pre_sel-2);
            uLCD.printf("  ");
            uLCD.locate(1, 2*sel-2);
            uLCD.color(GREEN);
            uLCD.printf(" o");
            pre_sel = sel;
        }
        if (pre_XYZ != DataXYZ[0]) {
            pre_XYZ = DataXYZ[0];
            uLCD.locate(1, 5);
            uLCD.color(WHITE);
            uLCD.printf("\n  init_X = %4d \n  init_Y = %4d \n  init_Z = %4d \n", init_DataXYZ[0], init_DataXYZ[1], init_DataXYZ[2]);
            uLCD.printf("\n  now_X  = %4d \n  now_Y  = %4d \n  now_Z  = %4d \n", DataXYZ[0], DataXYZ[1], DataXYZ[2]);
            a = angle;
            uLCD.printf("\n  angle =%7.3f\n", a);
        }
    }
}

void uLCD_selected () {
    uLCD.locate(1, 2*select_index-2);
    uLCD.color(RED);
    uLCD.printf(" o");
}