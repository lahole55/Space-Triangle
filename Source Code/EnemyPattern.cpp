#include "common.h"

/*******************************
void EnemyPattern0(int n)
�T�v�F�G���΂߂ɂ���������悤�ɂ���
�����F�Ȃ�
�Ԓl�F�Ȃ�
*******************************/
void EnemyPattern0(int n) {
	enemy[n].x += enemy[n].spd*cos(enemy[n].ang);
	enemy[n].y += enemy[n].spd*sin(enemy[n].ang);
}