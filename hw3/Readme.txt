hw3

程式的執行:

1. 編譯並燒錄進板子，此時uLCD初始化

2. 打開screen檢查wifi連線狀況

3. 另一終端執行py檔，使PC與板子連線並下達自定義RPC指令開始辨識手勢

4. 三種手勢對應30、40、50度，當前選擇顯示綠圈於uLCD

5. 確定選擇按下按鈕，此時綠圈變紅圈，wifi傳出選擇的訊號

6. PC接收到訊號後下達RPC指令停止辨識手勢，開始角度判斷

7. 開始角度判斷時LED3亮(藍亮)2秒後記錄當前加速度值顯示於uLCD，LED3暗(橘亮)並開始記錄加速度值

8. 當前加速度與角度顯示於uLCD，當超過選擇的角度後，wifi傳出角度的訊號

9. PC接收到角度的訊號後下達RPC指令停止角度判斷，開始辨識手勢，重複循環



程式有:

1. main.cpp : 主程式，按鈕interrupt、執行RPC、uLCD、wifi的thread

2. computer.py : PC執行，訂閱主題、接收訊息，並下達自定義RPC指令

3. wifi.h : wifi設定、接收與發出wifi訊息

4. serial_RPC.h : RPC接收與自定義RPC指令，並執行手勢辨識、角度判斷的thread

5. uLCD.h : uLCD的顯示與初始化

6. gesture_UI.h : 手勢辨識

7. tilt_angle_detection.h : 角度判斷