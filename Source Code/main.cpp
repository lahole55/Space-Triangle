#include "common.h"

PlayerKey Pkey;
PlayerData player;
int ptu = 0;
Shot pla_bul;
Shot pla_bul2[SHOT_MAX];
OneZi hen[SHOT_MAX];
Ten kouten[TEN_MAX];
EnemyData enemy[ENEMY_MAX];
EnemyOrder enemyOrder[ENEMY_ORDER_MAX];
TopData top[TOP];
int stageCount = 20 * 60;
int Tcnt = 0;
int game_num;
int red = GetColor(255, 0, 0);
int water = GetColor(0, 255, 255);
int white = GetColor(255, 255, 255);
int black = GetColor(0, 0, 0);

GAME_STATE gstate = GAME_TITLE;

//�w�i
static int bg;
static int bgTitle;
static int bgClear;

static int gLoad1, gLoad2, gLoad3, gLoad4, gLoad5;
static int gAnime[4];

//���ʉ�
static int sCheck;
static int sSelect;
static int sBack;

//�t�H���g
int fHandle[FH];
/*
�ԍ�: �傫��, ����
0: 150, 4�@�^�C�g��
1: 40, 3�@System()�֐��A�^�C�g���ł̑I������
2: 60, 4�@�����L���O�\
3: 30, 3  �J�E���g�A���_
*/

static int cntP = 0;//���_���グ�鎞�Ɏg��
static int select_no = 0;
static int SeleCnt = 0;
static int load_num;


int WINAPI WinMain(HINSTANCE hI, HINSTANCE hp, LPSTR lpC, int nC) {
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1) return -1;

	//�w�i�̓ǂ݂���
	bg = LoadGraph("sozai\\haikei.jpg");
	bgTitle = LoadGraph("sozai\\3.jpg");
	bgClear = LoadGraph("sozai\\2.jpg");
	gLoad1 = LoadGraph("sozai\\Load01.png");
	gLoad2 = LoadGraph("sozai\\Load02.png");
	gLoad3 = LoadGraph("sozai\\Load03.png");
	gLoad4 = LoadGraph("sozai\\Load04.png");
	gLoad5 = LoadGraph("sozai\\Load05.png");
	gAnime[0] = LoadGraph("sozai\\enemy1.png");
	gAnime[1] = LoadGraph("sozai\\enemy2.png");
	gAnime[2] = LoadGraph("sozai\\enemy3.png");
	gAnime[3] = LoadGraph("sozai\\player.png");

	//���ʉ��̓ǂݍ���
	sCheck = LoadSoundMem("bgm\\check.wav");
	sSelect = LoadSoundMem("bgm\\select.wav");
	sBack = LoadSoundMem("bgm\\back.wav");

	/*�摜�ǂݍ���*/
	AllLoad();

	/*������*/
	AllInit();

	SetDrawScreen(DX_SCREEN_BACK); //�`���O���t�B�b�N�̈�̎w��
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		ClearDrawScreen();
		switch (gstate) {
		case GAME_TITLE:
			DrawGameTitle();
			break;
		case GAME_RANKING:
			DrawGameRanking();
			break;
		case GAME_LOAD_SCREAN:
			DrawGameLoadScrean();
			break;
		case GAME_MAIN:
			DrawGameMain();
			break;
		case GAME_CLEAR_0:
			DrawGameClear0();
			break;
		case GAME_CLEAR_1:
			DrawGameClear1();
			break;
		}

		ScreenFlip();
	}
	FileUpdate(); // �����L���O�t�@�C���̍X�V

	WaitKey();
	DxLib_End();
	return 0;
}


void DrawGameTitle() {
	DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, bgClear, TRUE);//�w�i�\��
	DrawStringToHandle(WINDOW_WIDTH/10, WINDOW_HEIGHT/10, "Space Triangle", white, fHandle[0]);

	if (SeleCnt == 0) {
		if (CheckHitKey(Pkey.up)) {
			PlaySoundMem(sSelect, DX_PLAYTYPE_BACK);
			select_no = (select_no + SELE_TITLE - 1) % SELE_TITLE;
			SeleCnt = 20;
			stageCount = 20 * 60;
		}
		if (CheckHitKey(Pkey.down)) {
			PlaySoundMem(sSelect, DX_PLAYTYPE_BACK);
			select_no = (select_no + 1) % SELE_TITLE;
			SeleCnt = 20;
			stageCount = 20 * 60;
		}
	}
	if (SeleCnt > 0)
		SeleCnt--;

	switch (select_no) {
	case 0:
		DrawStringToHandle(WINDOW_WIDTH/3, WINDOW_HEIGHT*6/10, "�Q�[���J�n", white, fHandle[1]);
		DrawStringToHandle(WINDOW_WIDTH / 3+100, WINDOW_HEIGHT * 7 / 10, "�����L���O", white, fHandle[1]);
		break;
	case 1:
		DrawStringToHandle(WINDOW_WIDTH / 3 + 100, WINDOW_HEIGHT * 6 / 10, "�Q�[���J�n", white, fHandle[1]);
		DrawStringToHandle(WINDOW_WIDTH / 3, WINDOW_HEIGHT * 7 / 10, "�����L���O", white, fHandle[1]);
		break;
	}

	if (CheckHitKey(KEY_INPUT_Z)) {
		if (SeleCnt == 0) {
			PlaySoundMem(sCheck, DX_PLAYTYPE_NORMAL);
			switch (select_no) {
			case 0:
				gstate = GAME_LOAD_SCREAN;
				stageCount = 8 * 60;
				select_no = 0;
				if (game_num == 0)
					load_num = 1;
				else
					load_num = rand() % 5 + 1;
				break;
			case 1:
				gstate = GAME_RANKING;
				break;
			}
		}
	}
}


void DrawGameRanking() {
	DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, bgClear, TRUE);//�w�i
	DrawStringToHandle(50, 50, "�����L���O", red, fHandle[0]);

	for (int i = 0; i < TOP; i++)
		DrawFormatStringToHandle(200, 200 + 70 * i, red, fHandle[1], "%d�� %10s = %7d", i + 1, top[i].name, top[i].pt);

	DrawStringToHandle(WINDOW_WIDTH - 250, WINDOW_HEIGHT - 100, "�߂�:Push_X", red, fHandle[1]);

	if (CheckHitKey(KEY_INPUT_X)) {
		PlaySoundMem(sBack, DX_PLAYTYPE_NORMAL);
		gstate = GAME_TITLE;
	}
}


void DrawGameLoadScrean() {
	stageCount--;
	switch (load_num) {
	case 1:
		DrawRotaGraph(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0.75, 0, gLoad1, TRUE);
		break;
	case 2:
		DrawRotaGraph(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 1, 0, gLoad2, TRUE);
		break;
	case 3:
		DrawRotaGraph(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 1, 0, gLoad3, TRUE);
		break;
	case 4:
		DrawRotaGraph(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 1, 0, gLoad4, TRUE);
		break;
	case 5:
		DrawRotaGraph(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 1, 0, gLoad5, TRUE);
		break;
	}
	
	for (int i = 0; i <= 3; i++) {
		if((stageCount / 30)%5 <= i)
			DrawGraph(970 - 50 * i, 670, gAnime[3-i], TRUE);
	}

	if (stageCount <= 0) {
		AllInit();
		stageCount = STAGE_COUNT + 4 * 60;
		gstate = GAME_MAIN;
	}
}


static int f = 0;
void DrawGameMain() {
	DrawExtendGraph(0, 0, WINDOW_WIDTH2, WINDOW_HEIGHT, bg, TRUE);//�w�i�\��
	if(stageCount <= STAGE_COUNT)
		PlayerBulletMove();
	EnemyDraw();
	EnemyMove();
	EnemyKill();
	PlayerDraw();
	DrawBox(WINDOW_WIDTH2, 0, WINDOW_WIDTH, WINDOW_HEIGHT, water, TRUE);
	System();

	stageCount--;

	if (stageCount > 0 && stageCount <= STAGE_COUNT) {
		PlayerShotEnter();
		EnemyEnter();
		PlayerMove();
		PlayerHitJudge();

	}
	else if (stageCount <= 0) {
		player.flag = 1;//�G�ɓ�����Ȃ��A�e�������Ȃ��Ȃ�
		DrawStringToHandle(300, WINDOW_HEIGHT / 2 - 50, "Finish!", red, fHandle[0]);

		if (stageCount <= -3 * 60 && f == 0) {
			gstate = GAME_CLEAR_0;
			player.rank = FileRanking();
			stageCount = 13 * 60;//13�b
			game_num++;
		}
	}

	//���_�̌v�Z
	if (cntP == 0) {
		if (ptu > 33) {
			player.pt += 33;
			ptu -= 33;
			cntP = 3;
			f = 1;
		}
		else if (ptu > 3) {
			player.pt += 3;
			ptu -= 3;
			f = 1;
			cntP = 3;
		}
		else if (player.pt > 0) {
			player.pt += ptu;
			ptu = 0;
			f = 0;
		}
	}
	if (cntP > 0)
		cntP--;
	if (player.pt >= 1000000)
		player.pt = 999999;

}

void DrawGameClear0() {//���ʔ��\
	DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, bgClear, TRUE);//�w�i
	stageCount--;


	for (int i = 0; i < TOP; i++)
		DrawFormatStringToHandle(250, 300 + 70 * i, red, fHandle[1], "%d�� %10s : %7d", i + 1, top[i].name, top[i].pt);

	if (stageCount <= 10 * 60) {
		if(player.rank == 0)
			DrawFormatStringToHandle(100, 100, red, fHandle[1], "�����N�O %7d", player.rank, player.pt);
		else
			DrawFormatStringToHandle(100, 100, red, fHandle[1], "��%d�� %7d", player.rank, player.pt);

		if (player.rank > 0) {//�����L���O����˖��O���͂�
			DrawStringToHandle(150, 160, "new_recode", red, fHandle[0]);
			if (stageCount == 0) {
				gstate = GAME_CLEAR_1;
				stageCount = 15 * 60;//15�b
			}

		}
		else {//�����L���O�O�˃^�C�g����
			if (stageCount == 0) {
				gstate = GAME_TITLE;
				AllInit();
			}
		}
	}

}

void DrawGameClear1() {//�����L���O����
	DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, bgClear, TRUE);//�w�i

	DrawFormatStringToHandle(100, 200, red, fHandle[1], "Point:%7d", player.pt);

	if (player.name[0] == '\0') {
		DrawStringToHandle(200, 300, "���O����͂��Ă�������(���p10�����A���{��5����)", red, fHandle[2]);
		KeyInputString(200, 400, 10, player.name, FALSE);
		f = 0;
	}
	else {
		stageCount--;

		if (f == 0) {
			FileRenewal();
			f = 1;
		}

		for (int i = 0; i < TOP; i++)
			DrawFormatStringToHandle(250, 250 + 70 * i, red, fHandle[1], "%d�� %10s : %7d", i + 1, top[i].name, top[i].pt);

		if (CheckHitKey(KEY_INPUT_X))
			f = 2;

		if (stageCount == 0 || f == 2) {
			if (f == 2)
				PlaySoundMem(sBack, DX_PLAYTYPE_NORMAL);
			gstate = GAME_TITLE;
			AllInit();
			select_no = 0;
			SeleCnt = 40;
			f = 0;
		}

		DrawStringToHandle(WINDOW_WIDTH - 250, WINDOW_HEIGHT - 100, "�߂�:Push_X", red, fHandle[1]);
	}
}