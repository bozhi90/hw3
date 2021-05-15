#include "mbed.h"
#include "wifi.h"
#include "serial_RPC.h"
#include "uLCD.h"

// 全域變數
bool runUI = false;	    // 手勢判斷的程式控制
bool runTAD = false; 	// 角度判斷的程式控制
int sel = 1;		    // 最後的手勢對應的值
int select_index = 1;	// 確認的值

InterruptIn button(USER_BUTTON);

EventQueue queue1;
Thread thread_bt;
Thread thread_RPC;
Thread thread_uLCD;
Thread thread_wifi;

void bt_push () {
    char s[100];
    select_index = sel;
    sprintf(s, "selected");
    mqtt_queue.call(&publish_message, client_pt, s, 0);
    uLCD_selected();
}

int main() {
    thread_bt.start(callback(&queue1, &EventQueue::dispatch_forever));

    button.rise(queue1.event(bt_push));
    thread_RPC.start(function_RPC);
    thread_uLCD.start(function_uLCD);
    thread_wifi.start(function_wifi);

    return 0;
}