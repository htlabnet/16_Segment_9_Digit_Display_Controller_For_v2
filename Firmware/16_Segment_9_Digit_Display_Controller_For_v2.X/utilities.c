/*
 * File:   utilities.c
 * Author: thotgamma
 *
 * Created on 2018/08/08, 18:13
 */


#include "utilities.h"


// (デバッグ用関数)
// 引数の数字を二進数でディスプレイに表示する
void showBinary(uint8_t input){
    for(int i = 0; i < 8; i++){
        if((input & (1 << i)) == 0){
            segMap[8-i] = ~fontList[0x30];
        }else{
            segMap[8-i] = ~fontList[0x31];
        }
    }
}

void setMsg(uint8_t input[]) {
    for (int i = 0; i < 9; i++) {
        segMap[i] = ~fontList[input[i]];
    }
}

void setMsgWithDot(uint8_t message[], uint8_t dotFlag[]) {
    for (int i = 0; i < 9; i++) {
        segMap[i] = ~(fontList[message[i]] | ((uint32_t)(dotFlag[i] == '.') << 16));
    }
}

uint8_t bcd2dec(uint8_t input) {
    return (input >> 4)*10 + (input & 0X0F);
}

uint8_t dec2bcd(uint8_t input) {
    return ((input/10)<<4) | (input%10);
}