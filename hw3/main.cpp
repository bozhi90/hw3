#include "mbed.h"       // mbed函式庫
#include "wifi.h"       // wifi函式庫
#include "serial_RPC.h" // RPC函式庫
#include "uLCD.h"       // uLCD函式庫

// 全域變數
bool runUI = false;	    // 手勢判斷的程式控制，Ture執行、False停止
bool runTAD = false; 	// 角度判斷的程式控制，Ture執行、False停止
int sel = 1;		    // 最後的手勢對應的值
int select_index = 1;	// 確認的值

InterruptIn button(USER_BUTTON);    // interrupt按鈕

EventQueue queue1;      // queue執行按鈕interrupt的程式
Thread thread_bt;       // 按鈕interrupt的程式的thread
Thread thread_RPC;      // 接收RPC指令的thread
Thread thread_uLCD;     // 顯示uLCD的thread
Thread thread_wifi;     // 接收wifi的thread

void bt_push () {       // 按鈕interrupt的程式
    char s[100];
    select_index = sel;
    sprintf(s, "selected");
    mqtt_queue.call(&publish_message, client_pt, s, 0); // wifi輸出"selected"
    uLCD_selected();                                    // uLCD顯示已選取
}

int main() {
    thread_bt.start(callback(&queue1, &EventQueue::dispatch_forever));  // 按鈕的thread開始執行按鈕interrupt的queue

    button.rise(queue1.event(bt_push));     // 按鈕被按下，就interrupt開執行始執行按鈕interrupt的queue
    thread_RPC.start(function_RPC);         // 開始執行RPC函式，接收RPC指令
    thread_uLCD.start(function_uLCD);       // 開始顯示uLCD
    thread_wifi.start(function_wifi);       // 開始執行wifi函式，接收與輸出wifi訊息

    return 0;
}