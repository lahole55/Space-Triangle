#include "common.h"
#include <string.h>
#pragma warning(disable:4996)

FILE *fp;


/*******************************
void HandleDataInit()
概要：フォントのサイズ・太さの初期設定
引数：なし
返値：なし
*******************************/
void HandleDataInit() {//サイズ、太さ
	fHandle[0] = CreateFontToHandle(NULL, 60, 4); //60,4
	fHandle[1] = CreateFontToHandle(NULL, 40, 3); //40,3
	fHandle[2] = CreateFontToHandle(NULL, 30, 3); //30,3

}

/*******************************
void FileLoad()
概要：ランキングファイルの読み込み、top[]に代入
引数：なし
返値：なし
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
概要：TOP5に含まれているのかの判定
引数：なし
返値：>0.順位　0.ランキング外
*******************************/
int FileRanking() {
	for (int i = 0; i < TOP; i++) {
		if (top[i].pt <= player.pt) {
			return i + 1;//順位を返す
		}
	}
	return 0;
}

/*******************************
void FileRenewal()
概要：ランキングの更新（プログラム内）
引数：なし
返値：なし
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
概要：ゲームを終了したときに、ランキングファイルの更新
引数：なし
返値：なし
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
概要：全てのロード関数を格納
引数：なし
返値：なし
*******************************/
void AllLoad() {
	FileLoad();
	PlayerLoad();
	EnemyLoad();
	EnemyDataLoad();

}

/*******************************
void AllInit()
概要：全ての初期化関数を格納
引数：なし
返値：なし
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
