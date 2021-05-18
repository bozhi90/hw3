/*****************************************************************************/
/*                            uLCD函式庫，顯示uLCD                             */
/*****************************************************************************/

#include "uLCD_4DGL.h"

extern bool runUI;
extern int sel;                     // 當前選擇的角度
extern int select_index;            // 最終選擇的角度
extern float angle;                 // 當前角度值

uLCD_4DGL uLCD(D1, D0, D2);         // uLCD接腳



/*****************************************************************************/
/*                                  uLCD初始化                                */
/*****************************************************************************/
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



/*****************************************************************************/
/*                                  uLCD主程式                                */
/*****************************************************************************/
void function_uLCD () {
    uLCD_initial();                         // 加速度計初始化

    int pre_sel = 0;                        // 紀錄前一個迴圈的選擇，用於判斷畫面刷新
    float a;                                // 儲存角度值，避免角度變化
    int16_t pre_XYZ = 0;                    // 紀錄前一個迴圈的加速度，用於判斷畫面刷新

    while (1) {
        ThisThread::sleep_for(50ms);
        if (runUI && pre_sel != sel) {      // 選擇改變刷新uLCD
            uLCD.locate(1, 2*pre_sel-2);
            uLCD.printf("  ");              // 清掉前一個
            uLCD.locate(1, 2*sel-2);
            uLCD.color(GREEN);
            uLCD.printf(" o");              // 記錄新的
            pre_sel = sel;                  // 更新sel
        }
        if (pre_XYZ != DataXYZ[0]) {        // 加速度有變化
            pre_XYZ = DataXYZ[0];           // 更新pre_XYZ
            uLCD.locate(1, 5);
            uLCD.color(WHITE);              // 刷新加速度
            uLCD.printf("\n  init_X = %4d \n  init_Y = %4d \n  init_Z = %4d \n", init_DataXYZ[0], init_DataXYZ[1], init_DataXYZ[2]);
            uLCD.printf("\n  now_X  = %4d \n  now_Y  = %4d \n  now_Z  = %4d \n", DataXYZ[0], DataXYZ[1], DataXYZ[2]);
            a = angle;
            uLCD.printf("\n  angle =%7.3f\n", a);   // 刷新角度
        }
    }
}



/*****************************************************************************/
/*                                   uLCD選擇                                */
/*****************************************************************************/
void uLCD_selected () {
    uLCD.locate(1, 2*select_index-2);           // 跳到按鈕按下時選擇的角度值
    uLCD.color(RED);                            // 改為紅色圈
    uLCD.printf(" o");
}