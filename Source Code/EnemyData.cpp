#include "common.h"

static int gEnemy1, gEnemy2, gEnemy3;

static int sKill0, sKill1, sKill2, sKill3, sKill4;


void(*EnemyPattern[ENEMY_PATTERN_MAX])(int) = {
	EnemyPattern0
};

/*******************************
void EnemyLoad()
�T�v�F�G�̉摜�A�|�����Ƃ��̌��ʉ������[�h
�����F�Ȃ�
�Ԓl�F�Ȃ�
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
�T�v�F�G�̃f�[�^��������
�����F�Ȃ�
�Ԓl�F�Ȃ�
*******************************/
void EnemyInit() {
	for (int i = 0; i < ENEMY_MAX; i++) {
		enemy[i] = { 0 };
	}
}

/*******************************
void EnemyDraw()
�T�v�F�G��`��
�����F�Ȃ�
�Ԓl�F�Ȃ�
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
�T�v�FEnemyData.csv����G�̌��f�[�^��ǂݍ���
�����F�Ȃ�
�Ԓl�F�Ȃ�
*******************************/
void EnemyDataLoad() {
	int fp;//�t�@�C����ǂݍ��ތ^
	int input[64];
	char inputc[64];

	fp = FileRead_open("EnemyData.csv");//�t�@�C���ǂݍ���
	if (fp == NULL) {
		printfDx("read error\n");
		return;
	}
	for (int i = 0; i < 2; i++)//�ŏ���2�s�ǂݔ�΂�
		while (FileRead_getc(fp) != '\n');

	int n = 0;//�s
	int num = 0;//��

	while (1) {
		for (int i = 0; i < 64; i++) {
			inputc[i] = input[i] = FileRead_getc(fp);//1�����擾����
													 //printfDx("%c",inputc[i]);
			if (inputc[i] == '/') {//�X���b�V���������
				while (FileRead_getc(fp) != '\n');//���s�܂Ń��[�v
				i = -1;//�J�E���^���ŏ��ɖ߂���
				continue;//for�܂Ŗ߂�
			}
			if (input[i] == ',' || input[i] == '\n') {//�J���}�����s�Ȃ�
				inputc[i] = '\0';//�����܂ł𕶎���Ƃ�

				break;
			}
			if (input[i] == EOF) {//�t�@�C���̏I���Ȃ�
				goto EXFILE;//�I��(EXFILE�܂Ŕ��)
			}
		}
		//printfDx("%d,",atoi(inputc));
		switch (num) {
		case 0: enemyOrder[n].knd = atoi(inputc); break;//atoi:������𐔎��ɕς���
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
�T�v�F���f�[�^����G�̃f�[�^��ǂݍ���
�����F�Ȃ�
�Ԓl�F�Ȃ�
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
�T�v�F�G�𓮂���
�����F�Ȃ�
�Ԓl�F�Ȃ�
*******************************/
void EnemyMove() {
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (enemy[i].flag > 0) {
			if (enemy[i].wait == 0) {
				EnemyPattern[enemy[i].pattern](i);
				enemy[i].cnt++;

				if (enemy[i].x < 0 || WINDOW_WIDTH2 < enemy[i].x) {//������o�悤�Ƃ����Ƃ�
					if (enemy[i].flag == 2 && enemy[i].rtn > 0) {//��ʓ��ɂ���,���˂���񐔂��܂�����Ƃ�
						double kaku = PI;
						enemy[i].ang = Hansya(enemy[i], kaku);//Keisan.cpp��
						enemy[i].rtn--;
						enemy[i].flag2 = -1;
					}
					else if (enemy[i].rtn == 0)
						enemy[i].flag = 0;
				}
				if (enemy[i].y < 0 || WINDOW_HEIGHT < enemy[i].y) {//�c����o�悤�Ƃ����Ƃ�
					if (enemy[i].flag == 2 && enemy[i].rtn > 0) {//��ʓ��ɂ���,���˂���񐔂��܂�����Ƃ�
						double kaku = PI / 2;
						enemy[i].ang = Hansya(enemy[i], kaku);//Keisan.cpp��
						enemy[i].rtn--;
						enemy[i].flag2 = -1;
					}
					else if (enemy[i].rtn == 0)
						enemy[i].flag = 0;
				}
				if (enemy[i].flag == 1) {//��ʊO�����ʓ��ɓ����Ă�����
					if (enemy[i].x >= 0 && WINDOW_WIDTH2 >= enemy[i].x && enemy[i].y >= 0 && WINDOW_HEIGHT >= enemy[i].y)
						enemy[i].flag = 2;
				}

				//pla_bul2[m].bullet[n]�ɓ��������Ƃ�
				for (int m = 0; m < SHOT_MAX; m++) {
					if (enemy[i].flag2 != m) {//�O�ɔ��˂����ӂƈႤ�ӂ̂Ƃ�
						for (int n = 0; n < SHOT_BULLET_MAX; n++) {
							if (pla_bul2[m].bullet[n].flag > 0) {
								if (EneBulHitJudge2(enemy[i], pla_bul2[m].bullet[n]) > 0) {
									double kaku = pla_bul2[m].bullet[n].ang;
									enemy[i].ang = Hansya(enemy[i], kaku);//���Α��ɔ�΂Ȃ��悤��(�o�Ȃ��悤��)�������K�v
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
�T�v�F�G���O�p�`�ɓ������Ƃ��ɓ|��
�����F�Ȃ�
�Ԓl�F�Ȃ�
*******************************/
static double sT = 0;
static int E_num = -1;
void EnemyKill() {
	if (kouten[0].flag == 2 && kouten[1].flag == 2 && kouten[2].flag == 2) {
		for (int i = 0; i < ENEMY_MAX; i++) {
			if (enemy[i].flag > 0) {//�G��|������
				if (EnemyInTriangle(kouten[0], kouten[1], kouten[2], enemy[i]) > 0) {
					if (E_num == -1)
						E_num = 0;
					enemy[i].flag = 0;

					//�O�p�`�̖ʐς����߂�
					sT = TriMenseki(kouten[0], kouten[1], kouten[2]);

					ptu += PointGet(enemy[i], sT);
					E_num++;
				}
			}
		}

		switch (E_num) { // �{��
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