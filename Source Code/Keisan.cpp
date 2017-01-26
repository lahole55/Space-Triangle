#include "common.h"

static int sCount;


/*******************************
void KeisanInit()
�T�v�F�e���o���O����1���֐��A��_�̏������A
�@�@�@�J�E���g�̌��ʉ��̌Ăяo��
�����F�Ȃ�
�Ԓl�F�Ȃ�
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
�T�v�F�ł����e��1���֐��i�O���j�����߂�
�����P�F�e���������Ƃ��̕ϐ�
�����Q�F�e�̔ԍ��i��ށj
�Ԓl�F�ł����e��1���֐��i�O���j
*******************************/
OneZi HenBullet(Shot a, int num) { // 1�_�݂̂Ōv�Z
	OneZi tmp;

	tmp.slp = tan(a.bullet[num].ang - PI / 2);
	tmp.irc = a.bullet[num].y - tmp.slp * a.bullet[num].x;

	return tmp;
}


/*******************************
Ten Kouten_Init(OneZi a, OneZi b)
�T�v�F�Q�̋O���������\��̌�_�����߂�
�����P�F�O���P
�����Q�F�O���Q
�Ԓl�F��_���W
*******************************/
Ten Kouten_Init(OneZi a, OneZi b) {//�ӓ��m�̌�_
	Ten tmp;

	if (a.slp != b.slp) { // �X���������łȂ��Ƃ�
		tmp.x = (b.irc - a.irc) / (a.slp - b.slp);
		tmp.y = a.slp * tmp.x + a.irc;
		tmp.flag = 1;
	}
	else // �X���������̂Ƃ�
		tmp.flag = 0;

	return tmp;
}


/*******************************
int BulBulHitJudge(Bullet a, Bullet b, Ten c)
�T�v�F2�̋O������_�ɒʂ������̔���
�����P�F�O���P��̒e
�����Q�F�O���Q��̒e
�����R�F�O���P�E�Q�������\��̌�_
�Ԓl�F1.������Ă���@0.������Ă��Ȃ�
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
�T�v�F�O�p�`���ɓG�����邩�̔���
�����P�F��_�P
�����Q�F��_�Q
�����R�F��_�R
�����S�F�G
�Ԓl�F1.�O�p�`���ɂ���@0.�O�p�`���ɂ��Ȃ�
*******************************/
int EnemyInTriangle(Ten A, Ten B, Ten C, EnemyData E) {
	Ten P;
	P.x = E.x;
	P.y = E.y;

	//����͊O��

	Vector2D AB = sub_vector(B, A);
	Vector2D BP = sub_vector(P, B);

	Vector2D BC = sub_vector(C, B);
	Vector2D CP = sub_vector(P, C);

	Vector2D CA = sub_vector(A, C);
	Vector2D AP = sub_vector(P, A);

	//�O��(z���������v�Z)
	double c1 = AB.x*BP.y - AB.y*BP.x;
	double c2 = BC.x*CP.y - BC.y*CP.x;
	double c3 = CA.x*AP.y - CA.y*AP.x;

	//�O�p�`�̓����ɂ���Ƃ�
	if ((c1 > 0 && c2 > 0 && c3 > 0) || (c1 < 0 && c2 < 0 && c3 < 0)) {
		return 1;
	}
	return 0;
}


/*******************************
Vector2D sub_vector(const Ten a, const Ten b)
�T�v�F�Q�_�Ԃ̃x�N�g�����v�Z
�����P�F��_�P
�����Q�F��_�Q
�Ԓl�F�x�N�g��
*******************************/
Vector2D sub_vector(const Ten a, const Ten b) {
	Vector2D ret;

	ret.x = a.x - b.x;
	ret.y = a.y - b.y;

	return ret;
}


/*******************************
double Hansya(EnemyData a, double k)
�T�v�F�G���e�ɓ��������Ƃ��́A�G�̔��ˊp�x���v�Z
�����P�F�G
�����Q�F�e�̊p�x
�Ԓl�F���˂����G�̊p�x
*******************************/
double Hansya(EnemyData a, double k) {
	double x = PI - a.ang + 2 * k;
	return x;
}


/*******************************
int EneBulHitJudge2(EnemyData a, Bullet b)
�T�v�F�G�ƒe�������������̔���
�����P�F�G
�����Q�F�e
�Ԓl�F1.���������@0.�������Ă��Ȃ�
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
�T�v�F�ł����O�p�`�̖ʐς��v�Z
�����P�F��_�P
�����Q�F��_�Q
�����R�F��_�R
�Ԓl�F�O�p�`�̖ʐ�
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
�T�v�F�G��|�����Ƃ��̓��_�̌v�Z
�����P�F�G
�����Q�F�O�p�`�̖ʐ�
�Ԓl�F���_
*******************************/
int PointGet(EnemyData a, double s) {
	int point = 0;

	point = log(100000.0) / log(s) * a.pt;

	return point;
}


/*******************************
void System()
�T�v�F���C����ʂ̐�����`��
�����F�Ȃ�
�Ԓl�F�Ȃ�
*******************************/
void System() {
	//���e�̕\��
	DrawStringToHandle(WINDOW_WIDTH2 + 100, 60, "Next", black, fHandle[2]);
	DrawBox(WINDOW_WIDTH2 + 80, 100, WINDOW_WIDTH2 + 180, 200, white, TRUE);
	DrawRotaGraph(WINDOW_WIDTH2 + 130, 150, 2, 0, pla_bul.bullet[pla_bul.num].ghandle, TRUE, FALSE);

	//�^�C���̕\��
	if (stageCount > 0) {
		if (stageCount <= STAGE_COUNT)
			DrawFormatStringToHandle(WINDOW_WIDTH2 + 250 / 8, 300, black, fHandle[2], "Time: %3d", stageCount / 60);
		else {
			if (stageCount % 60 == 0)
				PlaySoundMem(sCount, DX_PLAYTYPE_BACK);
			DrawFormatStringToHandle(WINDOW_WIDTH2 + 250 / 8, 300, black, fHandle[2], "Time: %3d", STAGE_COUNT / 60);
			DrawFormatStringToHandle(WINDOW_WIDTH2 / 2 - 30, WINDOW_HEIGHT / 2 - 50, red, fHandle[0], "%d", (stageCount - STAGE_COUNT) / 60);
		}

		// �c�莞��
		if (115 * 60 < stageCount && stageCount <= 120 * 60) {
			if(stageCount % 60 == 0)
				PlaySoundMem(sCount, DX_PLAYTYPE_BACK);
			if((stageCount / 30) % 2 == 1)
				DrawStringToHandle(WINDOW_WIDTH2 / 2 - 30, WINDOW_HEIGHT / 2 - 50, "120�b", red, fHandle[0]);
		}
		if (55 * 60 < stageCount && stageCount <= 60 * 60) {
			if(stageCount % 60 == 0)
				PlaySoundMem(sCount, DX_PLAYTYPE_BACK);
			if((stageCount / 30) % 2 == 1)
				DrawStringToHandle(WINDOW_WIDTH2 / 2 - 30, WINDOW_HEIGHT / 2 - 50, "60�b", red, fHandle[0]);
		}
		if (25 * 60 < stageCount && stageCount <= 30 * 60) {
			if(stageCount % 60 == 0)
				PlaySoundMem(sCount, DX_PLAYTYPE_BACK);
			if((stageCount / 30) % 2 == 1)
				DrawStringToHandle(WINDOW_WIDTH2 / 2 - 30, WINDOW_HEIGHT / 2 - 50, "30�b", red, fHandle[0]);
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

	//�|�C���g�̕\��
	DrawStringToHandle(WINDOW_WIDTH2 + 250 / 8, 350, "Point:", black, fHandle[2]);
	DrawFormatStringToHandle(WINDOW_WIDTH2 + 250 * 7 / 8 - 15 * 6, 350, black, fHandle[2], "%7d", player.pt);
}