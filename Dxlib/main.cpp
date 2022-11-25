#include "DxLib.h"
#include <cmath> // sqrt

struct Vector2 {
	float x;
	float y;
};

// �E�B���h�E�̃^�C�g���ɕ\�����镶����
const char TITLE[] = "LE2B_11_�T�m_�\��";

// �E�B���h�E����
const int WIN_WIDTH = 1280;

// �E�B���h�E�c��
const int WIN_HEIGHT = 720;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {
	// �E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	// �E�B���h�E�T�C�Y���蓮�ł͕ύX�������A
	// ���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// �^�C�g����ύX
	SetMainWindowText(TITLE);

	// ��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�(���j�^�[�̉𑜓x�ɍ��킹��)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// ��ʃT�C�Y��ݒ�(�𑜓x�Ƃ̔䗦�Őݒ�)
	SetWindowSizeExtendRate(1.0);

	// ��ʂ̔w�i�F��ݒ肷��
	SetBackgroundColor(0x00, 0x00, 0x00);

	// DXlib�̏�����
	if (DxLib_Init() == -1) { return -1; }

	// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	// �摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���


	// �Q�[�����[�v�Ŏg���ϐ��̐錾

	//�~

	float circleX = WIN_WIDTH / 2;
	float circleY = WIN_HEIGHT / 2 + 200;
	float circleR = 50;

	float speed = 5;

	//��

	float line1X = 0;
	float line1Y = 0;

	float line2X = WIN_WIDTH;
	float line2Y = WIN_HEIGHT;

	//�����蔻��

	int col = 0;

	Vector2 start_to_center;
	Vector2 end_to_center;
	Vector2 start_to_end;

	Vector2 normal_start_to_end;

	// �ŐV�̃L�[�{�[�h���p
	char keys[256] = { 0 };

	// 1���[�v(�t���[��)�O�̃L�[�{�[�h���
	char oldkeys[256] = { 0 };

	// �Q�[�����[�v
	while (true) {
		// �ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		// �ŐV�̃L�[�{�[�h�����擾
		GetHitKeyStateAll(keys);

		// ��ʃN���A
		ClearDrawScreen();
		//---------  ��������v���O�������L�q  ----------//

		// �X�V����

		if (keys[KEY_INPUT_LEFT] == 1) {
			circleX -= speed;
		}
		else if (keys[KEY_INPUT_RIGHT] == 1) {
			circleX += speed;
		}

		if (keys[KEY_INPUT_UP] == 1) {
			circleY -= speed;
		}
		else if (keys[KEY_INPUT_DOWN] == 1) {
			circleY += speed;
		}

		//�~�Ɛ��̓����蔻��

		start_to_center.x = circleX - line1X;
		start_to_center.y = circleY - line1Y;
		end_to_center.x = circleX - line2X;
		end_to_center.y = circleY - line2Y;
		start_to_end.x = line2X - line1X;
		start_to_end.y = line2Y - line1Y;

		// �P�ʃx�N�g��������

		//����

		float start_to_end_norm =
		std::sqrt((start_to_end.x * start_to_end.x) + (start_to_end.y * start_to_end.y));

		//���K��

		normal_start_to_end = start_to_end;

		float mag = 1 / start_to_end_norm;
		normal_start_to_end.x *= mag;
		normal_start_to_end.y *= mag;

		float distance_projection = (start_to_center.x * normal_start_to_end.y) - (normal_start_to_end.x * start_to_center.y);

		// �����Ɖ~�̍ŒZ�̒��������a����������
		if (std::fabs(distance_projection) < circleR)
		{
			// �n�_ => �I�_�Ǝn�_ => �~�̒��S�̓��ς��v�Z����
			float dot01 = (start_to_center.x * start_to_end.x) + (start_to_center.y * start_to_end.y);
			// �n�_ => �I�_�ƏI�_ => �~�̒��S�̓��ς��v�Z����
			float dot02 = (end_to_center.x * start_to_end.x) + (end_to_center.y * start_to_end.y);

			// ��̓��ς̊|���Z���ʂ�0�ȉ��Ȃ瓖����
			if (dot01 * dot02 <= 0.0f)
			{
				col = 1;
			}

		}
		else
		{
			col = 0;
		}

		//�d�Ȃ��Ă��Ȃ��ꍇ

		//�傫���Z�o
		float start_to_center_length = std::sqrt((start_to_center.x * start_to_center.x) + (start_to_center.y * start_to_center.y));
		float end_to_center_length = std::sqrt((end_to_center.x * end_to_center.x) + (end_to_center.y * end_to_center.y));

		if (start_to_center_length < circleR ||
			end_to_center_length < circleR)
		{
			col = 1;
		}

		// �`�揈��


		////���������Ă���Ƃ���,���Ȃ��Ƃ��ŐF��ς���
		if (col == 0) {

			DrawCircle(circleX, circleY, circleR, GetColor(255, 255, 255), true);

			DrawLine(line1X, line1Y, line2X, line2Y, GetColor(255, 255, 255), true);

		}
		else {

			DrawCircle(circleX, circleY, circleR, GetColor(255,0,0), true);

			DrawLine(line1X, line1Y, line2X, line2Y, GetColor(255, 0, 0), true);

		}

		//DrawLine(start_to_center.x, start_to_center.y, end_to_center.x, end_to_center.y, GetColor(0, 0, 255), true);

		//---------  �����܂łɃv���O�������L�q  ---------//
		// (�_�u���o�b�t�@)����
		ScreenFlip();

		// 20�~���b�ҋ@(�^��60FPS)
		WaitTimer(20);

		// Windows�V�X�e�����炭�������������
		if (ProcessMessage() == -1) {
			break;
		}

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dx���C�u�����I������
	DxLib_End();

	// ����I��
	return 0;
}
