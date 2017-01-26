#include "common.h"

static int gPlayer;
static int gBullet[SHOT_MAX];

static int sShot, sSelect, sDamage;

static PlayerKey PlayerKeyInit(int l, int r, int u, int d, int s, int c, int f);
static PlayerData PlayerDataInit(double x, double y, int g, int f, double a);
static int HitJudge0(PlayerData a, EnemyData b);


/*******************************
void PlayerLoad()
概要：プレイヤーと弾の画像・効果音を読み込む
引数：なし
返値：なし
*******************************/
void PlayerLoad() {
	gPlayer = LoadGraph("sozai\\player.png");
	gBullet[0] = LoadGraph("sozai\\pla_tama.png");
	gBullet[1] = LoadGraph("sozai\\pla_tama2.png");
	gBullet[2] = LoadGraph("sozai\\pla_tama3.png");

	sShot = LoadSoundMem("bgm\\player\\shot.wav");
	sSelect = LoadSoundMem("bgm\\select.wav");
	sDamage = LoadSoundMem("bgm\\player\\Damage.wav");
}

/*******************************
void PlayerInit()
概要：プレイヤーの初期化
引数：なし
返値：なし
*******************************/
void PlayerInit() {
	Pkey = PlayerKeyInit(KEY_INPUT_LEFT, KEY_INPUT_RIGHT, KEY_INPUT_UP, KEY_INPUT_DOWN, KEY_INPUT_Z, KEY_INPUT_X, KEY_INPUT_C);

	player = PlayerDataInit((WINDOW_WIDTH2)/2, WINDOW_HEIGHT - 100, gPlayer, 0, 0);
	player.cnt = 0;
	player.cnt2 = 0;
	player.pt = 0;
	for (int n = 0; n < 11; n++)
		player.name[n] = '\0';

	/*Playerが撃った弾を描画*/
	for (int j = 0; j < SHOT_MAX; j++) {
		pla_bul.bullet[j] = { 0 };
		pla_bul.bullet[j].ghandle = gBullet[j];
	}

	/*1発目の軌道に弾を置く*/
	for (int i = 0; i < SHOT_MAX; i++) {
		pla_bul2[i] = { 0 };
		for (int j = 0; j < SHOT_BULLET_MAX; j++) {
			pla_bul2[i].bullet[j] = { 0 };
			pla_bul2[i].bullet[j].ghandle = gBullet[i];
		}
	}
}


/*******************************
static PlayerKey PlayerKeyInit(int l, int r, int u, int d, int s, int c, int f)
概要：キーコンフィグの初期化
引数１：左に旋回するキー
引数２：右に旋回するキー
引数３：前に進むキー
引数４：後ろに進むキー
引数５：弾を撃つキー
引数６：次に撃つ弾の種類を変更するキー
引数７：弾を消すキー
返値：キーコンフィグ
*******************************/
static PlayerKey PlayerKeyInit(int l, int r, int u, int d, int s, int c, int f) {
	PlayerKey tmp;

	tmp.left = l;
	tmp.right = r;
	tmp.up = u;
	tmp.down = d;
	tmp.shot = s;
	tmp.change = c;
	tmp.free = f;

	return tmp;
}

/*******************************
static PlayerData PlayerDataInit(double x, double y, int g, int f, double a)
概要：プレイヤーの座標・画像・フラグ・角度の初期化
引数１：x座標
引数２：y座標
引数３：読み込んだ画像
引数４：生成しているかのフラグ
引数５：角度
返値：初期化したプレイヤー
*******************************/
static PlayerData PlayerDataInit(double x, double y, int g, int f, double a) {
	PlayerData tmp;
	tmp.x = x;
	tmp.y = y;
	tmp.ghandle = g;
	tmp.flag = f;
	tmp.ang = a;

	return tmp;
}

/*******************************
void PlayerDraw()
概要：プレイヤーと弾の描画
引数：なし
返値：なし
*******************************/
void PlayerDraw() {
	/*プレーヤーが出す弾A*/
	if (pla_bul.flag > 0) {
		for (int j = 0; j < SHOT_MAX; j++) {
			if (pla_bul.bullet[j].flag > 0) {
				DrawRotaGraph(pla_bul.bullet[j].x, pla_bul.bullet[j].y, 1, pla_bul.bullet[j].ang, pla_bul.bullet[j].ghandle, TRUE, FALSE);
			}
		}
	}

	/*弾Aから出た弾B*/
	for (int i = 0; i < SHOT_MAX; i++) {
		if (pla_bul2[i].flag > 0) {
			for (int j = 0; j < SHOT_BULLET_MAX; j++) {
				if (pla_bul2[i].bullet[j].flag > 0)
					DrawRotaGraph(pla_bul2[i].bullet[j].x, pla_bul2[i].bullet[j].y, 1, pla_bul2[i].bullet[j].ang, pla_bul2[i].bullet[j].ghandle, TRUE, FALSE);
			}
		}
	}

	/*プレーヤー*/
	DrawRotaGraph(player.x, player.y, 1, player.ang, player.ghandle, TRUE, FALSE);

}

/*******************************
void PlayerMove()
概要：プレイヤーの動き
引数：なし
返値：なし
*******************************/
void PlayerMove() {

	if (CheckHitKey(Pkey.up)) {
		player.x += 5 * cos(player.ang - PI / 2);
		player.y += 5 * sin(player.ang - PI / 2);
	}
	if (CheckHitKey(Pkey.down)) {
		player.x += 5 * cos(player.ang + PI / 2);
		player.y += 5 * sin(player.ang + PI / 2);
	}
	if (CheckHitKey(Pkey.left)) {
		player.ang -= PI / 72;
	}
	if (CheckHitKey(Pkey.right)) {
		player.ang += PI / 72;
	}

	if (player.x < 15)
		player.x = 15;
	if (player.x > WINDOW_WIDTH2 - 15)
		player.x = WINDOW_WIDTH2 - 15;
	if (player.y < 15)
		player.y = 15;
	if (player.y > WINDOW_HEIGHT - 15)
		player.y = WINDOW_HEIGHT - 15;
}

/*******************************
void PlayerShotEnter()
概要：プレイヤーが撃つ弾・生成する弾の初期化
引数：なし
返値：なし
*******************************/
void PlayerShotEnter() {
	if (pla_bul.flag == 0) {
		pla_bul.flag = 1;
		pla_bul.cnt = 0;
		return;
	}

	for (int i = 0; i < SHOT_MAX; i++) {
		if (pla_bul2[i].flag == 0) {
			pla_bul2[i].flag = 1;
			pla_bul2[i].cnt = 0;
			return;
		}
	}
}

/*******************************
void PlayerBulletMove()
概要：弾の動き、三角形ができる条件の作成
引数：なし
返値：なし
*******************************/
void PlayerBulletMove() {
	/*pla_bulの設定*/
	if (player.flag == 0 && Tcnt == 0) {
		if (player.cnt == 0) {
			if (CheckHitKey(Pkey.shot)) {
				if (pla_bul.flag > 0) {
					if (pla_bul.bullet[pla_bul.num].flag == 0) {
						PlaySoundMem(sShot, DX_PLAYTYPE_BACK);
						pla_bul.bullet[pla_bul.num].flag = 1;
						pla_bul.bullet[pla_bul.num].x = player.x;
						pla_bul.bullet[pla_bul.num].y = player.y;
						pla_bul.bullet[pla_bul.num].ang = player.ang;
						pla_bul.bullet[pla_bul.num].spd = 6;
						pla_bul.bullet[pla_bul.num].cnt = 2;

						hen[pla_bul.num] = HenBullet(pla_bul, pla_bul.num);

						for (int j = 0; j < SHOT_BULLET_MAX; j++) {//これまであった線を消す
							pla_bul2[pla_bul.num].bullet[j].flag = 0;
						}

						pla_bul.num = (pla_bul.num + 1) % SHOT_MAX;
						player.cnt = 30;
					}
				}
			}
		}

		if (player.cnt2 == 0) {
			if (CheckHitKey(Pkey.change)) {
				if (pla_bul.flag > 0) {
					PlaySoundMem(sSelect, DX_PLAYTYPE_BACK);
					pla_bul.num = (pla_bul.num + 1) % SHOT_MAX;
					player.cnt2 = 20;
				}
			}
		}
	}

	if (CheckHitKey(Pkey.free)) {
		for (int i = 0; i < SHOT_MAX; i++) {
			pla_bul.bullet[i].flag = 0; // 進む弾を消す
			for (int j = 0; j < SHOT_BULLET_MAX; j++) {
				pla_bul2[i].bullet[j].flag = 0;
			}
			kouten[i].flag = 0;
			Tcnt = 0;
		}
	}
	if (player.cnt > 0)
		player.cnt--;
	if (player.cnt2 > 0)
		player.cnt2--;


	for (int i = 0; i < SHOT_MAX; i++) {
		if (pla_bul.bullet[i].flag == 1) {
			pla_bul.bullet[i].x += pla_bul.bullet[i].spd * cos(pla_bul.bullet[i].ang - PI / 2);
			pla_bul.bullet[i].y += pla_bul.bullet[i].spd * sin(pla_bul.bullet[i].ang - PI / 2);


			/*敵との当たり判定(当たったら、敵をはじく)*/


			/*画面外に出たら、*/
			if (pla_bul.bullet[i].x < 0 || pla_bul.bullet[i].x > WINDOW_WIDTH2 || pla_bul.bullet[i].y < 0 || pla_bul.bullet[i].y > WINDOW_HEIGHT)
				pla_bul.bullet[i].flag = 0;

		}
	}


	/*pla_bul2の設定*/
	for (int i = 0; i < SHOT_MAX; i++) {
		if (pla_bul.bullet[i].cnt == 0 && pla_bul.bullet[i].flag > 0) {
			for (int j = 0; j < SHOT_BULLET_MAX; j++) {
				if (pla_bul2[i].bullet[j].flag == 0) {
					pla_bul2[i].bullet[j].flag = 1;
					pla_bul2[i].bullet[j].x = pla_bul.bullet[i].x;
					pla_bul2[i].bullet[j].y = pla_bul.bullet[i].y;
					pla_bul2[i].bullet[j].ang = pla_bul.bullet[i].ang;
					pla_bul2[i].bullet[j].cnt = 90000;//900

					pla_bul.bullet[i].cnt = 2;
					break;
				}
			}
		}

		if (pla_bul.bullet[i].cnt > 0)
			pla_bul.bullet[i].cnt--;
	}

	//交点を求める
	if (pla_bul2[0].bullet[0].flag > 0 && pla_bul2[1].bullet[0].flag > 0)
		kouten[0] = Kouten_Init(hen[0], hen[1]);
	if (pla_bul2[1].bullet[0].flag > 0 && pla_bul2[2].bullet[0].flag > 0)
		kouten[1] = Kouten_Init(hen[1], hen[2]);
	if (pla_bul2[2].bullet[0].flag > 0 && pla_bul2[0].bullet[0].flag > 0)
		kouten[2] = Kouten_Init(hen[2], hen[0]);

	/*弾が重なっているかの判定*/
	//hen[0]とhen[1]
	if (kouten[0].flag == 1) {
		for (int j0 = 0; j0 < SHOT_BULLET_MAX; j0++) {
			if (pla_bul2[0].bullet[j0].flag > 0) {
				for (int j1 = 0; j1 < SHOT_BULLET_MAX; j1++) {
					if (pla_bul2[1].bullet[j1].flag > 0) {
						if (BulBulHitJudge(pla_bul2[0].bullet[j0], pla_bul2[1].bullet[j1], kouten[0]) > 0) {
							kouten[0].flag = 2;
							kouten[0].h1 = j0;
							kouten[0].h2 = j1;
							break;
						}
					}
				}
			}
		}
	}
	else if(kouten[0].flag == 2){
		if (pla_bul2[0].bullet[kouten[0].h1].flag == 0 || pla_bul2[1].bullet[kouten[0].h2].flag == 0)
			kouten[0].flag = 0;
	}

	//hen[1]とhen[2]
	if (kouten[1].flag == 1) {
		for (int j1 = 0; j1 < SHOT_BULLET_MAX; j1++) {
			if (pla_bul2[1].bullet[j1].flag > 0) {
				for (int j2 = 0; j2 < SHOT_BULLET_MAX; j2++) {
					if (pla_bul2[2].bullet[j2].flag > 0) {
						if (BulBulHitJudge(pla_bul2[1].bullet[j1], pla_bul2[2].bullet[j2], kouten[1]) > 0) {
							kouten[1].flag = 2;
							kouten[1].h1 = j1;
							kouten[1].h2 = j2;
							break;
						}
					}
				}
			}
		}
	}
	else if(kouten[1].flag == 2){
		if (pla_bul2[1].bullet[kouten[1].h1].flag == 0 || pla_bul2[2].bullet[kouten[1].h2].flag == 0)
			kouten[1].flag = 0;
	}

	//hen[2]とhen[0]
	if (kouten[2].flag == 1) {//交点が存在しない場合
		for (int j2 = 0; j2 < SHOT_BULLET_MAX; j2++) {
			if (pla_bul2[2].bullet[j2].flag > 0) {
				for (int j0 = 0; j0 < SHOT_BULLET_MAX; j0++) {
					if (pla_bul2[0].bullet[j0].flag > 0) {
						if (BulBulHitJudge(pla_bul2[2].bullet[j2], pla_bul2[0].bullet[j0], kouten[2]) > 0) {
							kouten[2].flag = 2;
							kouten[2].h1 = j2;
							kouten[2].h2 = j0;
							break;
						}
					}
				}
			}
		}
	}
	else if(kouten[2].flag == 2){
		if (pla_bul2[2].bullet[kouten[2].h1].flag == 0 || pla_bul2[0].bullet[kouten[2].h2].flag == 0)
			kouten[2].flag = 0;
	}

	//上二つが成り立つかの判定
	if (kouten[0].flag == 2 && kouten[1].flag == 2 && kouten[2].flag == 2) {
		if (Tcnt == 0)
			Tcnt = 120;
		DrawTriangle(kouten[0].x, kouten[0].y, kouten[1].x, kouten[1].y, kouten[2].x, kouten[2].y, white, TRUE);
	}

	/*消えるまでのカウントの計算*/
	//三角形ができたとき
	if (Tcnt == 1) {//弾をすべて消す
		for (int i = 0; i < SHOT_MAX; i++) {
			pla_bul.bullet[i].flag = 0;
			for (int j = 0; j < SHOT_BULLET_MAX; j++) {
				pla_bul2[i].bullet[j].flag = 0;
			}
		}
	}
	if (Tcnt > 0)
		Tcnt--;
	//自然に消えていくとき
	for (int i = 0; i < SHOT_MAX; i++) {
		for (int j = 0; j < SHOT_BULLET_MAX; j++) {
			if (pla_bul2[i].bullet[j].flag > 0) {
				if (pla_bul2[i].bullet[j].cnt == 0)
					pla_bul2[i].bullet[j].flag = 0;

				if (pla_bul2[i].bullet[j].cnt > 0)
					pla_bul2[i].bullet[j].cnt--;
			}
		}
	}

}

/*******************************
void PlayerHitJudge()
概要：プレイヤーと敵が当たったときにポイントを減らす
引数：なし
返値：なし
*******************************/
void PlayerHitJudge() {
	if (player.flag == 0) {
		for (int i = 0; i < ENEMY_MAX; i++) {
			if (HitJudge0(player, enemy[i]) == 1) {
				PlaySoundMem(sDamage, DX_PLAYTYPE_BACK);
				ptu -= 3;
			}
		}
	}
}

/*******************************
static int HitJudge0(PlayerData a, EnemyData b)
概要：プレイヤーと敵のあたり判定
引数１：プレイヤー
引数２：敵
返値：1.当たった　0.当たっていない
*******************************/
static int HitJudge0(PlayerData a, EnemyData b) {
	int x = a.x - b.x;
	int y = a.y - b.y;
	int h = 20;

	if (x*x + y*y < h*h) {
		return 1;
	}
	else {
		return 0;
	}
}