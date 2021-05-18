/*****************************************************************************/
/*                RPC函式庫，接收RPC指令與自定義RPC function                     */
/*****************************************************************************/

#include "mbed_rpc.h"
#include "tilt_angle_detection.h"       // 角度判斷函式庫
#include "gesture_UI.h"                 // 手勢判斷函式庫

extern bool runUI;	                    // 手勢判斷的程式控制
extern bool runTAD; 	                // 角度判斷的程式控制

DigitalOut myled(LED1);                 // LED1，顯示執行手勢判斷
DigitalOut myled2(LED2);                // LED2，顯示執行角度判斷
BufferedSerial pc(USBTX, USBRX);

Thread thread_GUI;                      // 執行手勢判斷的thread
Thread thread_TAD;                      // 執行角度判斷的thread



/*****************************************************************************/
/*                            RPC主程式，接收RPC指令                            */
/*****************************************************************************/
void function_RPC() {
    char buf[256], outbuf[256];

    FILE *devin = fdopen(&pc, "r");
    FILE *devout = fdopen(&pc, "w");
    while (1) {
        memset(buf, 0, 256);
        for (int i = 0; ; i++) {
            char recv = fgetc(devin);
            if (recv == '\n') {
                printf("\r\n");
                break;
            }
            buf[i] = fputc(recv, devout);
        }
        RPC::call(buf, outbuf);
        printf("%s\r\n", outbuf);
    }
}



/*****************************************************************************/
/*                           自定義RPC，開始手勢判斷                            */
/*****************************************************************************/
void Gesture_UI(Arguments *in, Reply *out) {
    myled = 1;
    runUI = true;
    thread_GUI.start(function_GUI);
}
RPCFunction gesture_UI(&Gesture_UI, "Gesture_UI");



/*****************************************************************************/
/*                           自定義RPC，結束手勢判斷                            */
/*****************************************************************************/
void Gesture_UI_stop(Arguments *in, Reply *out) {
    myled = 0;
    runUI = false;
}
RPCFunction gesture_UI_stop(&Gesture_UI_stop, "Gesture_UI_stop");



/*****************************************************************************/
/*                           自定義RPC，開始角度測量                            */
/*****************************************************************************/
void Tilt_angle_detection (Arguments *in, Reply *out) {
    myled2 = 1;
    runTAD = true;
    thread_TAD.start(function_TAD);
}
RPCFunction TA_detection(&Tilt_angle_detection, "Tilt_angle_detection");



/*****************************************************************************/
/*                           自定義RPC，結束角度測量                            */
/*****************************************************************************/
void Tilt_angle_detection_stop (Arguments *in, Reply *out) {    // 角度測量
    myled2 = 0;
    runTAD = false;
}
RPCFunction TA_detection_stop(&Tilt_angle_detection_stop, "Tilt_angle_detection_stop");