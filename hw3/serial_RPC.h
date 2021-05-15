#include "mbed_rpc.h"
#include "tilt_angle_detection.h"
#include "gesture_UI.h"

extern bool runUI;	                                    // 手勢判斷的程式控制
extern bool runTAD; 	                                // 角度判斷的程式控制

DigitalOut myled(LED1);
DigitalOut myled2(LED2);
BufferedSerial pc(USBTX, USBRX);

Thread thread_GUI;
Thread thread_TAD;

/******************************************************************************/
void function_RPC () {                                          // RPC接收
    char buf[256], outbuf[256];

    FILE *devin = fdopen(&pc, "r");
    FILE *devout = fdopen(&pc, "w");
    while(1) {
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
/******************************************************************************/
void Gesture_UI (Arguments *in, Reply *out) {                   // 開始手勢判斷
    myled = 1;
    runUI = true;
    thread_GUI.start(function_GUI);
}
RPCFunction gesture_UI(&Gesture_UI, "Gesture_UI");
/******************************************************************************/
void Gesture_UI_stop (Arguments *in, Reply *out) {              // 結束手勢判斷
    myled = 0;
    runUI = false;
}
RPCFunction gesture_UI_stop(&Gesture_UI_stop, "Gesture_UI_stop");
/******************************************************************************/
void Tilt_angle_detection (Arguments *in, Reply *out) {         // 開始角度測量
    myled2 = 1;
    runTAD = true;
    thread_TAD.start(function_TAD);
}
RPCFunction TA_detection(&Tilt_angle_detection, "Tilt_angle_detection");
/******************************************************************************/
void Tilt_angle_detection_stop (Arguments *in, Reply *out) {    // 結束角度測量
    myled2 = 0;
    runTAD = false;
}
RPCFunction TA_detection_stop(&Tilt_angle_detection_stop, "Tilt_angle_detection_stop");
/******************************************************************************/