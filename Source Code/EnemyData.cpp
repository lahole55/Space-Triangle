/*******************************
EnemyData.cpp
概要：敵データの描画、動き、キル判定
*******************************/

#include "common.h"

static int gEnemy1, gEnemy2, gEnemy3;

static int sKill0, sKill1, sKill2, sKill3, sKill4;


void(*EnemyPattern[ENEMY_PATTERN_MAX])(int) = {
    EnemyPattern0
};

/*******************************
 void EnemyLoad()
 概要：敵の画像、倒したときの効果音をロード
 *******************************/
void EnemyLoad() {
    gEnemy1 = LoadGraph("sozai\\enemy1.png");
    gEnemy2 = LoadGraph("sozai\\enemy2.png");
    gEnemy3 = LoadGraph("sozai\\enemy3.png");
    
    sKill0 = LoadSoundMem("bgm\\kill\\Kill0.wav");
    sKill1 = LoadSoundMem("bgm\\kill\\Kill1.wav");
    sKill2 = LoadSoundMem("bgm\\kill\\Kill2.wav");
    sKill3 = LoadSoundMem("bgm\\kill\\Kill3.wav");
    sKill4 = LoadSoundMem("bgm\\kill\\Kill4.wav");
}

/*******************************
 void EnemyInit()
 概要：敵のデータを初期化
 *******************************/
void EnemyInit() {
    for (int i = 0; i < ENEMY_MAX; i++) {
        enemy[i] = { 0 };
    }
}

/*******************************
 void EnemyDraw()
 概要：敵を描画
 *******************************/
void EnemyDraw() {
    for (int i = 0; i < ENEMY_MAX; i++) {
        if (enemy[i].flag > 0) {
            switch (enemy[i].knd) {
                case 1:
                    DrawRotaGraph(enemy[i].x, enemy[i].y, 1, 0, gEnemy1, TRUE, FALSE);
                    break;
                case 2:
                    DrawRotaGraph(enemy[i].x, enemy[i].y, 1, 0, gEnemy2, TRUE, FALSE);
                    break;
                case 3:
                    DrawRotaGraph(enemy[i].x, enemy[i].y, 1, 0, gEnemy3, TRUE, FALSE);
                    break;
            }
        }
    }
}

/*******************************
 void EnemyDataLoad()
 概要：EnemyData.csvから敵の元データを読み込む
 *******************************/
void EnemyDataLoad() {
    int fp;//ファイルを読み込む型
    int input[64];
    char inputc[64];
    
    fp = FileRead_open("EnemyData.csv");//ファイル読み込み
    if (fp == NULL) {
        printfDx("read error\n");
        return;
    }
    for (int i = 0; i < 2; i++)//最初の2行読み飛ばす
        while (FileRead_getc(fp) != '\n');
    
    int n = 0;//行
    int num = 0;//列
    
    while (1) {
        for (int i = 0; i < 64; i++) {
            inputc[i] = input[i] = FileRead_getc(fp);//1文字取得する
            //printfDx("%c",inputc[i]);
            if (inputc[i] == '/') {//スラッシュがあれば
                while (FileRead_getc(fp) != '\n');//改行までループ
                i = -1;//カウンタを最初に戻して
                continue;//forまで戻る
            }
            if (input[i] == ',' || input[i] == '\n') {//カンマか改行なら
                inputc[i] = '\0';//そこまでを文字列とし
                
                break;
            }
            if (input[i] == EOF) {//ファイルの終わりなら
                goto EXFILE;//終了(EXFILEまで飛ぶ)
            }
        }
        //printfDx("%d,",atoi(inputc));
        switch (num) {
            case 0: enemyOrder[n].knd = atoi(inputc); break;
            case 1: enemyOrder[n].pattern = atoi(inputc); break;
            case 2: enemyOrder[n].wait = atoi(inputc); break;
            case 3: enemyOrder[n].x = atof(inputc); break;
            case 4: enemyOrder[n].y = atof(inputc); break;
            case 5: enemyOrder[n].muki = atof(inputc); break;
            case 6: enemyOrder[n].spd = atof(inputc); break;
            case 7: enemyOrder[n].rtn = atoi(inputc); break;
            case 8: enemyOrder[n].pt = atoi(inputc); break;
        }
        num++;
        if (num == 9) {
            num = 0;
            enemyOrder[n].flag = 1;
            n++;
        }
    }
EXFILE:
    FileRead_close(fp);
}

/*******************************
 void EnemyEnter()
 概要：元データから敵のデータを読み込む
 *******************************/
void EnemyEnter() {
    for (int t = 0; t < ENEMY_ORDER_MAX; t++) {
        if (enemyOrder[t].flag == 1) {
            for (int i = 0; i < ENEMY_MAX; i++) {
                if (enemy[i].flag == 0) {
                    enemy[i].flag = 1;
                    enemy[i].flag2 = -2;
                    enemy[i].knd = enemyOrder[t].knd;
                    enemy[i].pattern = enemyOrder[t].pattern;
                    enemy[i].wait = enemyOrder[t].wait;
                    enemy[i].x = enemyOrder[t].x;
                    enemy[i].y = enemyOrder[t].y;
                    enemy[i].ang = 2 * PI / 8 * enemyOrder[t].muki;
                    enemy[i].spd = enemyOrder[t].spd;
                    enemy[i].rtn = enemyOrder[t].rtn;
                    enemy[i].pt = enemyOrder[t].pt;
                    
                    enemyOrder[t].flag = 0;
                    break;
                }
            }
        }
    }
}

/*******************************
 void EnemyMove()
 概要：敵を動かす
 *******************************/
void EnemyMove() {
    for (int i = 0; i < ENEMY_MAX; i++) {
        if (enemy[i].flag > 0) {
            if (enemy[i].wait == 0) {
                EnemyPattern[enemy[i].pattern](i);
                enemy[i].cnt++;
                
                if (enemy[i].x < 0 || WINDOW_WIDTH2 < enemy[i].x) {//横から出ようとしたとき
                    if (enemy[i].flag == 2 && enemy[i].rtn > 0) {//画面内にある,反射する回数がまだあるとき
                        double kaku = PI;
                        enemy[i].ang = Hansya(enemy[i], kaku);//Keisan.cppへ
                        enemy[i].rtn--;
                        enemy[i].flag2 = -1;
                    }
                    else if (enemy[i].rtn == 0)
                        enemy[i].flag = 0;
                }
                if (enemy[i].y < 0 || WINDOW_HEIGHT < enemy[i].y) {//縦から出ようとしたとき
                    if (enemy[i].flag == 2 && enemy[i].rtn > 0) {//画面内にある,反射する回数がまだあるとき
                        double kaku = PI / 2;
                        enemy[i].ang = Hansya(enemy[i], kaku);//Keisan.cppへ
                        enemy[i].rtn--;
                        enemy[i].flag2 = -1;
                    }
                    else if (enemy[i].rtn == 0)
                        enemy[i].flag = 0;
                }
                if (enemy[i].flag == 1) {//画面外から画面内に入ってきた時
                    if (enemy[i].x >= 0 && WINDOW_WIDTH2 >= enemy[i].x && enemy[i].y >= 0 && WINDOW_HEIGHT >= enemy[i].y)
                        enemy[i].flag = 2;
                }
                
                //pla_bul2[m].bullet[n]に当たったとき
                for (int m = 0; m < SHOT_MAX; m++) {
                    if (enemy[i].flag2 != m) {//前に反射した辺と違う辺のとき
                        for (int n = 0; n < SHOT_BULLET_MAX; n++) {
                            if (pla_bul2[m].bullet[n].flag > 0) {
                                if (EneBulHitJudge2(enemy[i], pla_bul2[m].bullet[n]) > 0) {
                                    double kaku = pla_bul2[m].bullet[n].ang;
                                    enemy[i].ang = Hansya(enemy[i], kaku);//反対側に飛ばないように(出ないように)調整が必要
                                    enemy[i].flag2 = m;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            
            if (enemy[i].wait > 0)
                enemy[i].wait--;
            
        }
    }
}

/*******************************
 void EnemyKill()
 概要：敵が三角形に入ったときに倒す
 *******************************/
static double sT = 0;
static int E_num = -1;
void EnemyKill() {
    if (kouten[0].flag == 2 && kouten[1].flag == 2 && kouten[2].flag == 2) {
        for (int i = 0; i < ENEMY_MAX; i++) {
            if (enemy[i].flag > 0) {//敵を倒す判定
                if (EnemyInTriangle(kouten[0], kouten[1], kouten[2], enemy[i]) > 0) {
                    if (E_num == -1)
                        E_num = 0;
                    enemy[i].flag = 0;
                    
                    //三角形の面積を求める
                    sT = TriMenseki(kouten[0], kouten[1], kouten[2]);
                    
                    ptu += PointGet(enemy[i], sT);
                    E_num++;
                }
            }
        }
        
        switch (E_num) { // 倍率
            case 0:
                PlaySoundMem(sKill0, DX_PLAYTYPE_BACK);
                break;
            case 1:
                PlaySoundMem(sKill1, DX_PLAYTYPE_BACK);
                break;
            case 2:
                PlaySoundMem(sKill2, DX_PLAYTYPE_BACK);
                ptu *= 1.2;
                break;
            case 3:
                PlaySoundMem(sKill3, DX_PLAYTYPE_BACK);
                ptu *= 1.5;
                break;
            case 4:
                PlaySoundMem(sKill4, DX_PLAYTYPE_BACK);
                ptu *= 2.0;
                break;
        }
        E_num = -1;
    }
}
