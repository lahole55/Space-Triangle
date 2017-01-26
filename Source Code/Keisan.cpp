#include "common.h"

static int sCount;


/*******************************
 void KeisanInit()
 概要：弾が出す軌道の1次関数、交点の初期化、
 　　　カウントの効果音の呼び出し
 *******************************/
void KeisanInit() {
    for (int i = 0; i < SHOT_MAX; i++) {
        hen[i] = { 0 };
    }
    
    for (int i = 0; i < TEN_MAX; i++) {
        kouten[i] = { 0 };
    }
    
    sCount = LoadSoundMem("bgm\\Count10.wav");
}


/*******************************
 OneZi HenBullet(Shot a, int num)
 概要：打った弾の1次関数（軌道）を求める
 引数１：弾を撃ったときの変数
 引数２：弾の番号（種類）
 返値：打った弾の1次関数（軌道）
 *******************************/
OneZi HenBullet(Shot a, int num) { // 1点のみで計算
    OneZi tmp;
    
    tmp.slp = tan(a.bullet[num].ang - PI / 2);
    tmp.irc = a.bullet[num].y - tmp.slp * a.bullet[num].x;
    
    return tmp;
}


/*******************************
 Ten Kouten_Init(OneZi a, OneZi b)
 概要：２つの軌道が交わる予定の交点を求める
 引数１：軌道１
 引数２：軌道２
 返値：交点座標
 *******************************/
Ten Kouten_Init(OneZi a, OneZi b) {//辺同士の交点
    Ten tmp;
    
    if (a.slp != b.slp) { // 傾きが同じでないとき
        tmp.x = (b.irc - a.irc) / (a.slp - b.slp);
        tmp.y = a.slp * tmp.x + a.irc;
        tmp.flag = 1;
    }
    else // 傾きが同じのとき
        tmp.flag = 0;
    
    return tmp;
}


/*******************************
 int BulBulHitJudge(Bullet a, Bullet b, Ten c)
 概要：2つの軌道が交点に通ったかの判定
 引数１：軌道１上の弾
 引数２：軌道２上の弾
 引数３：軌道１・２が交わる予定の交点
 返値：1.交わっている　0.交わっていない
 *******************************/
int BulBulHitJudge(Bullet a, Bullet b, Ten c) {
    int x1 = c.x - a.x;
    int x2 = c.x - b.x;
    int y1 = c.y - a.y;
    int y2 = c.y - b.y;
    int r = 10;
    
    if (x1*x1 + y1*y1 <= r*r) {
        if (x2*x2 + y2*y2 <= r*r) {
            return 1;
        }
    }
    return 0;
}

/*******************************
 int EnemyInTriangle(Ten A, Ten B, Ten C, EnemyData E)
 概要：三角形内に敵がいるかの判定
 引数１：交点１
 引数２：交点２
 引数３：交点３
 引数４：敵
 返値：1.三角形内にいる　0.三角形内にいない
 *******************************/
int EnemyInTriangle(Ten A, Ten B, Ten C, EnemyData E) {
    Ten P;
    P.x = E.x;
    P.y = E.y;
    
    //線上は外側
    
    Vector2D AB = sub_vector(B, A);
    Vector2D BP = sub_vector(P, B);
    
    Vector2D BC = sub_vector(C, B);
    Vector2D CP = sub_vector(P, C);
    
    Vector2D CA = sub_vector(A, C);
    Vector2D AP = sub_vector(P, A);
    
    //外積(z成分だけ計算)
    double c1 = AB.x*BP.y - AB.y*BP.x;
    double c2 = BC.x*CP.y - BC.y*CP.x;
    double c3 = CA.x*AP.y - CA.y*AP.x;
    
    //三角形の内側にあるとき
    if ((c1 > 0 && c2 > 0 && c3 > 0) || (c1 < 0 && c2 < 0 && c3 < 0)) {
        return 1;
    }
    return 0;
}


/*******************************
 Vector2D sub_vector(const Ten a, const Ten b)
 概要：２点間のベクトルを計算
 引数１：交点１
 引数２：交点２
 返値：ベクトル
 *******************************/
Vector2D sub_vector(const Ten a, const Ten b) {
    Vector2D ret;
    
    ret.x = a.x - b.x;
    ret.y = a.y - b.y;
    
    return ret;
}


/*******************************
 double Hansya(EnemyData a, double k)
 概要：敵が弾に当たったときの、敵の反射角度を計算
 引数１：敵
 引数２：弾の角度
 返値：反射した敵の角度
 *******************************/
double Hansya(EnemyData a, double k) {
    double x = PI - a.ang + 2 * k;
    return x;
}


/*******************************
 int EneBulHitJudge2(EnemyData a, Bullet b)
 概要：敵と弾が当たったかの判定
 引数１：敵
 引数２：弾
 返値：1.当たった　0.当たっていない
 *******************************/
int EneBulHitJudge2(EnemyData a, Bullet b) {
    int x1 = a.x - b.x;
    int y1 = a.y - b.y;
    int r = 10;
    
    if (x1*x1 + y1*y1 <= r*r) {
        return 1;
    }
    return 0;
}


/*******************************
 double TriMenseki(Ten A, Ten B, Ten C)
 概要：できた三角形の面積を計算
 引数１：交点１
 引数２：交点２
 引数３：交点３
 返値：三角形の面積
 *******************************/
double TriMenseki(Ten A, Ten B, Ten C) {
    double s = 0;
    
    Vector2D AB = sub_vector(B, A);
    Vector2D AC = sub_vector(C, A);
    
    s = fabs(AB.x*AC.y - AB.y*AC.x) / 2;
    
    return s;
}


/*******************************
 int PointGet(EnemyData a, double s)
 概要：敵を倒したときの得点の計算
 引数１：敵
 引数２：三角形の面積
 返値：得点
 *******************************/
int PointGet(EnemyData a, double s) {
    int point = 0;
    
    point = log(100000.0) / log(s) * a.pt;
    
    return point;
}


/*******************************
 void System()
 概要：メイン画面の数字を描画
 *******************************/
void System() {
    //次弾の表示
    DrawStringToHandle(WINDOW_WIDTH2 + 100, 60, "Next", black, fHandle[2]);
    DrawBox(WINDOW_WIDTH2 + 80, 100, WINDOW_WIDTH2 + 180, 200, white, TRUE);
    DrawRotaGraph(WINDOW_WIDTH2 + 130, 150, 2, 0, pla_bul.bullet[pla_bul.num].ghandle, TRUE, FALSE);
    
    //タイムの表示
    if (stageCount > 0) {
        if (stageCount <= STAGE_COUNT)
            DrawFormatStringToHandle(WINDOW_WIDTH2 + 250 / 8, 300, black, fHandle[2], "Time: %3d", stageCount / 60);
        else {
            if (stageCount % 60 == 0)
                PlaySoundMem(sCount, DX_PLAYTYPE_BACK);
            DrawFormatStringToHandle(WINDOW_WIDTH2 + 250 / 8, 300, black, fHandle[2], "Time: %3d", STAGE_COUNT / 60);
            DrawFormatStringToHandle(WINDOW_WIDTH2 / 2 - 30, WINDOW_HEIGHT / 2 - 50, red, fHandle[0], "%d", (stageCount - STAGE_COUNT) / 60);
        }
        
        // 残り時間
        if (115 * 60 < stageCount && stageCount <= 120 * 60) {
            if(stageCount % 60 == 0)
                PlaySoundMem(sCount, DX_PLAYTYPE_BACK);
            if((stageCount / 30) % 2 == 1)
                DrawStringToHandle(WINDOW_WIDTH2 / 2 - 30, WINDOW_HEIGHT / 2 - 50, "120秒", red, fHandle[0]);
        }
        if (55 * 60 < stageCount && stageCount <= 60 * 60) {
            if(stageCount % 60 == 0)
                PlaySoundMem(sCount, DX_PLAYTYPE_BACK);
            if((stageCount / 30) % 2 == 1)
                DrawStringToHandle(WINDOW_WIDTH2 / 2 - 30, WINDOW_HEIGHT / 2 - 50, "60秒", red, fHandle[0]);
        }
        if (25 * 60 < stageCount && stageCount <= 30 * 60) {
            if(stageCount % 60 == 0)
                PlaySoundMem(sCount, DX_PLAYTYPE_BACK);
            if((stageCount / 30) % 2 == 1)
                DrawStringToHandle(WINDOW_WIDTH2 / 2 - 30, WINDOW_HEIGHT / 2 - 50, "30秒", red, fHandle[0]);
        }
        
        if (stageCount / 60 <= 10) {
            if(stageCount % 60 == 0)
                PlaySoundMem(sCount, DX_PLAYTYPE_BACK);
            DrawFormatStringToHandle(WINDOW_WIDTH2 / 2 - 30, WINDOW_HEIGHT / 2 - 50, red, fHandle[0], "%d", stageCount / 60);
        }
    }
    else if (stageCount <= 0) {
        DrawStringToHandle(WINDOW_WIDTH2 + 250 / 8, 300, "Time:   0", black, fHandle[2]);
    }
    
    //ポイントの表示
    DrawStringToHandle(WINDOW_WIDTH2 + 250 / 8, 350, "Point:", black, fHandle[2]);
    DrawFormatStringToHandle(WINDOW_WIDTH2 + 250 * 7 / 8 - 15 * 6, 350, black, fHandle[2], "%7d", player.pt);
}
