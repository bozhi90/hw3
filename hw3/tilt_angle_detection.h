/*****************************************************************************/
/*                 角度測量函式庫，初始化角度值並讀取現在角度值                     */
/*****************************************************************************/

#include "stm32l475e_iot01_accelero.h"
#include "math.h"

DigitalOut myled3(LED3);                                // LED3，顯示是否完成初始化
int16_t DataXYZ[3] = {0};                               // 當下加速度的量值
int16_t init_DataXYZ[3] = {0};                          // 初始化加速度的量值
float angle = 0;                                        // 推算的角度
extern bool runTAD;                                     // 控制角度判斷的開始與停止
extern int select_index;



/*****************************************************************************/
/*                                 角度計算                                   */
/*****************************************************************************/
float angle_calculate (void) {
    float angle = 0;
    float x = 0, y = 0;
    int i;

    for (i = 0; i < 3; i++) {
        x += DataXYZ[i] * init_DataXYZ[i];              // 分子為兩加速度做內積
        y += init_DataXYZ[i] * init_DataXYZ[i];         // 分母為長度平方
    }
    angle = x / y;                                      // cos(角度)
    if (angle > 1) return 0;
    else return acos(angle) * 180 / 3.14;               // 換算角度
}



/*****************************************************************************/
/*                           角度判斷主程式，收集角度資料                         */
/*****************************************************************************/
void function_TAD (void) {
    bool init = false;                                  // 是否需要初始化
    char str[100];
    BSP_ACCELERO_Init();                                // 六軸感測器初始化
    
    while (1) {
        while (!runTAD) {                               // 停止角度判斷
            if (init == true) {                         // 結束手勢判斷，歸零
                init_DataXYZ[0] = 0;
                init_DataXYZ[1] = 0;
                init_DataXYZ[2] = 0;
                DataXYZ[0] = 0;
                DataXYZ[1] = 0;
                DataXYZ[2] = 0;
                init = false;                           // 歸零完畢，沒有初始化
            }
            ThisThread::sleep_for(100ms);
        }
        if (init == false) {                            // 開始角度判斷，初始化
            myled3 = 0;
            ThisThread::sleep_for(2s);
            BSP_ACCELERO_AccGetXYZ(init_DataXYZ);       // 初始化當下加速度
            myled3 = 1;
            init = true;                                // 完成初始化
        }
        BSP_ACCELERO_AccGetXYZ(DataXYZ);                // 紀錄當下加速度
        angle = angle_calculate();                      // 角度計算
        
        if (angle > 10 * select_index + 20) {           // 角度大於設定值
            sprintf(str,"angle = %g", angle);
            mqtt_queue.call(&publish_message, client_pt, str, 1);   // wifi輸出角度值
        }
        ThisThread::sleep_for(500ms);
    }
}