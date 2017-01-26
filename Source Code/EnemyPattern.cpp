#include "common.h"

/*******************************
void EnemyPattern0(int n)
概要：敵を斜めにも動かせるようにする
引数１：敵の番号
*******************************/
void EnemyPattern0(int n) {
	enemy[n].x += enemy[n].spd*cos(enemy[n].ang);
	enemy[n].y += enemy[n].spd*sin(enemy[n].ang);
}
