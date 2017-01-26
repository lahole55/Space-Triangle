/*******************
Space Triangle
*******************/

#include "DxLib.h"
#include <math.h>

// 320*240  640*480  800*600  1024*768  1280*1024
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define WINDOW_WIDTH2 (WINDOW_WIDTH-250)
#define PI 3.1415926535
#define SELE_TITLE 2
#define FH 3 // フォント数
#define TOP 5
#define SHOT_MAX 3
#define SHOT_BULLET_MAX 1000
#define TEN_MAX 3 // 交点
#define ENEMY_MAX 4
#define ENEMY_ORDER_MAX 84
#define ENEMY_PATTERN_MAX 1
#define STAGE_COUNT 180*60 // 180*60

// 列挙体
typedef enum {
	GAME_TITLE,
	GAME_RANKING,
	GAME_LOAD_SCREAN,
	GAME_MAIN,
	GAME_CLEAR_0,
	GAME_CLEAR_1,
}GAME_STATE;

/* 構造体 */
typedef struct {
	int left, right, up, down;
	int shot, change, free;
}PlayerKey;

typedef struct {
	double x, y;
	int ghandle;
	int flag;
	double ang;
	int cnt; // 弾を撃てる間隔
	int cnt2; // 弾を変更できる間隔
	int pt;
	int rank;
	char name[11];
}PlayerData;

typedef struct {
	int flag, cnt;
	double x, y, ang, spd;
	int ghandle;
}Bullet;

typedef struct {
	int flag, cnt, num;
	Bullet bullet[SHOT_BULLET_MAX];
}Shot;

typedef struct {
	double slp, irc; // 傾き、切片
}OneZi;//1次関数

typedef struct {
	double x, y;
	int flag;
	int h1, h2; // 交点にある弾の番号
}Ten;

typedef struct {
	double x, y;
}Vector2D; // 三角形と弾の当たり判定をするときに使用

typedef struct {
	double x, y;
	int ghandle;
	int flag;
	int cnt; // 動きで使用
	int flag2; // 反射で使用
	double ang;
	int pattern, knd, spd, wait, rtn, pt;
}EnemyData;

typedef struct {
	double x, y;
	int flag;
	int pattern, knd, spd, wait, rtn, pt;
	double muki;
}EnemyOrder;

typedef struct {
	char name[11];
	int pt;
}TopData;


extern PlayerKey Pkey;
extern PlayerData player;
extern int ptu; // ポイントアップ
extern Shot pla_bul;
extern Shot pla_bul2[];
extern OneZi hen[];
extern Ten kouten[];
extern EnemyData enemy[];
extern EnemyOrder enemyOrder[];
extern TopData top[]; //ランキングトップ5を格納
extern int fHandle[]; // フォント
extern int stageCount;
extern int Tcnt; // 三角形が表示される時間
extern int game_num; // ゲームをした回数
extern int red;
extern int water;
extern int white;
extern int black;


void DrawGameTitle();
void DrawGameRanking();
void DrawGameLoadScrean();
void DrawGameMain();
void DrawGameClear0(); // 結果発表
void DrawGameClear1(); // ランキング入りしたときの名前入力等

/* PlayerData.cpp */
void PlayerLoad();
void PlayerInit();
void PlayerDraw();
void PlayerMove();
void PlayerShotEnter();
void PlayerBulletMove();
void PlayerHitJudge(); // PlayerとEnemyの当たり判定(当たったら、ポイントをマイナスにする)

/* Keisan.cpp */
void KeisanInit();
OneZi HenBullet(Shot a, int num);
Ten Kouten_Init(OneZi a, OneZi b);
int BulBulHitJudge(Bullet a, Bullet b, Ten c);
int EnemyInTriangle(Ten A, Ten B, Ten C, EnemyData E);
Vector2D sub_vector(const Ten a, const Ten b);
double Hansya(EnemyData a, double k);
int EneBulHitJudge2(EnemyData a, Bullet b);
double TriMenseki(Ten A, Ten B, Ten C);
int PointGet(EnemyData a, double s);
void System();

/* EnemyData.cpp */
void EnemyLoad();
void EnemyInit();
void EnemyDraw();
void EnemyDataLoad();
void EnemyEnter();
void EnemyMove();
void EnemyKill();

/* EnemyPattern.cpp */
void EnemyPattern0(int n);

/* FileData.cpp */
void HandleDataInit();
void FileLoad();
int FileRanking();
void FileRenewal();
void FileUpdate();
void AllLoad();
void AllInit();

#pragma once
