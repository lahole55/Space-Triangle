#include "common.h"

/*******************************
void EnemyPattern0(int n)
概要：敵を斜めにも動かせるようにする
引数：なし
返値：なし
*******************************/
void EnemyPattern0(int n) {
	enemy[n].x += enemy[n].spd*cos(enemy[n].ang);
	enemy[n].y += enemy[n].spd*sin(enemy[n].ang);
}