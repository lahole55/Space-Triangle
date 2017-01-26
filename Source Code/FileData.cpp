#include "common.h"
#include <string.h>
#pragma warning(disable:4996)

FILE *fp;


/*******************************
void HandleDataInit()
�T�v�F�t�H���g�̃T�C�Y�E�����̏����ݒ�
�����F�Ȃ�
�Ԓl�F�Ȃ�
*******************************/
void HandleDataInit() {//�T�C�Y�A����
	fHandle[0] = CreateFontToHandle(NULL, 60, 4); //60,4
	fHandle[1] = CreateFontToHandle(NULL, 40, 3); //40,3
	fHandle[2] = CreateFontToHandle(NULL, 30, 3); //30,3

}

/*******************************
void FileLoad()
�T�v�F�����L���O�t�@�C���̓ǂݍ��݁Atop[]�ɑ��
�����F�Ȃ�
�Ԓl�F�Ȃ�
*******************************/
void FileLoad() {
	fp = fopen("Top5_1.txt", "r");

	if (fp == NULL)
		return;
	else {
		for (int i = 0; i < TOP; i++) {
			fscanf(fp, "%s %d", top[i].name, &top[i].pt);
		}
		fscanf(fp, "%d", &game_num);
		fclose(fp);
	}
}

/*******************************
int FileRanking()
�T�v�FTOP5�Ɋ܂܂�Ă���̂��̔���
�����F�Ȃ�
�Ԓl�F>0.���ʁ@0.�����L���O�O
*******************************/
int FileRanking() {
	for (int i = 0; i < TOP; i++) {
		if (top[i].pt <= player.pt) {
			return i + 1;//���ʂ�Ԃ�
		}
	}
	return 0;
}

/*******************************
void FileRenewal()
�T�v�F�����L���O�̍X�V�i�v���O�������j
�����F�Ȃ�
�Ԓl�F�Ȃ�
*******************************/
void FileRenewal() {
	for (int j = TOP - 1; j > player.rank - 1; j--) {
		top[j].pt = top[j - 1].pt;
		strcpy(top[j].name, top[j - 1].name);
	}
	top[player.rank - 1].pt = player.pt;
	strcpy(top[player.rank - 1].name, player.name);
}


/*******************************
void FileUpdate()
�T�v�F�Q�[�����I�������Ƃ��ɁA�����L���O�t�@�C���̍X�V
�����F�Ȃ�
�Ԓl�F�Ȃ�
*******************************/
void FileUpdate() {
	fp = fopen("Top5_1.txt", "w");

	if (fp == NULL)
		return;
	else {
		for (int i = 0; i < TOP; i++) {
			fprintf(fp, "%10s   %7d\n", top[i].name, top[i].pt);
		}
		fprintf(fp, "%d\n", game_num);
		fclose(fp);
	}
}


/*******************************
void AllLoad()
�T�v�F�S�Ẵ��[�h�֐����i�[
�����F�Ȃ�
�Ԓl�F�Ȃ�
*******************************/
void AllLoad() {
	FileLoad();
	PlayerLoad();
	EnemyLoad();
	EnemyDataLoad();

}

/*******************************
void AllInit()
�T�v�F�S�Ă̏������֐����i�[
�����F�Ȃ�
�Ԓl�F�Ȃ�
*******************************/
void AllInit() {
	stageCount = 20 * 60;
	HandleDataInit();
	PlayerInit();
	KeisanInit();
	EnemyInit();
	for (int t = 0; t < ENEMY_ORDER_MAX; t++)
		enemyOrder[t].flag = 1;

}
