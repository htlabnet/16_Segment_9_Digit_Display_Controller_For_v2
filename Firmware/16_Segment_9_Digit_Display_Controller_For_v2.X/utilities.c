/*
 * File:   utilities.c
 * Author: thotgamma
 *
 * Created on 2018/08/08, 18:13
 */


#include "utilities.h"


// (デバッグ用関数)
// 引数の数字を二進数でディスプレイに表示する
void showBinary(int input){
    for(int i = 0; i < 8; i++){
        if((input & (1 << i)) == 0){
            segMap[i] = ~fontList[0x30];
        }else{
            segMap[i] = ~fontList[0x31];
        }
    }
}

void setMsg(char input[]) {
    for (int i = 0; i < 9; i++) {
        segMap[i] = ~fontList[input[i]];
    }
}

void setMsgWithDot(char message[], char dotFlag[]) {
    for (int i = 0; i < 9; i++) {
        segMap[i] = ~(fontList[message[i]] | ((uint32_t)(dotFlag[i] == '.') << 16));
    }
}