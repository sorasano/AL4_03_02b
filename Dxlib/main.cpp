#include "DxLib.h"
#include <cmath> // sqrt

struct Vector2 {
	float x;
	float y;
};

// ウィンドウのタイトルに表示する文字列
const char TITLE[] = "LE2B_11_サノ_ソラ";

// ウィンドウ横幅
const int WIN_WIDTH = 1280;

// ウィンドウ縦幅
const int WIN_HEIGHT = 720;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	// ウィンドウサイズを手動では変更させず、
	// かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// タイトルを変更
	SetMainWindowText(TITLE);

	// 画面サイズの最大サイズ、カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// 画面サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1.0);

	// 画面の背景色を設定する
	SetBackgroundColor(0x00, 0x00, 0x00);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像などのリソースデータの変数宣言と読み込み


	// ゲームループで使う変数の宣言

	//円

	float circleX = WIN_WIDTH / 2;
	float circleY = WIN_HEIGHT / 2 + 200;
	float circleR = 50;

	float speed = 5;

	//線

	float line1X = 0;
	float line1Y = 0;

	float line2X = WIN_WIDTH;
	float line2Y = WIN_HEIGHT;

	//当たり判定

	int col = 0;

	Vector2 start_to_center;
	Vector2 end_to_center;
	Vector2 start_to_end;

	Vector2 normal_start_to_end;

	// 最新のキーボード情報用
	char keys[256] = { 0 };

	// 1ループ(フレーム)前のキーボード情報
	char oldkeys[256] = { 0 };

	// ゲームループ
	while (true) {
		// 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
		// 最新のキーボード情報を取得
		GetHitKeyStateAll(keys);

		// 画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		// 更新処理

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

		//円と線の当たり判定

		start_to_center.x = circleX - line1X;
		start_to_center.y = circleY - line1Y;
		end_to_center.x = circleX - line2X;
		end_to_center.y = circleY - line2Y;
		start_to_end.x = line2X - line1X;
		start_to_end.y = line2Y - line1Y;

		// 単位ベクトル化する

		//長さ

		float start_to_end_norm =
		std::sqrt((start_to_end.x * start_to_end.x) + (start_to_end.y * start_to_end.y));

		//正規化

		normal_start_to_end = start_to_end;

		float mag = 1 / start_to_end_norm;
		normal_start_to_end.x *= mag;
		normal_start_to_end.y *= mag;

		float distance_projection = (start_to_center.x * normal_start_to_end.y) - (normal_start_to_end.x * start_to_center.y);

		// 線分と円の最短の長さが半径よりも小さい
		if (std::fabs(distance_projection) < circleR)
		{
			// 始点 => 終点と始点 => 円の中心の内積を計算する
			float dot01 = (start_to_center.x * start_to_end.x) + (start_to_center.y * start_to_end.y);
			// 始点 => 終点と終点 => 円の中心の内積を計算する
			float dot02 = (end_to_center.x * start_to_end.x) + (end_to_center.y * start_to_end.y);

			// 二つの内積の掛け算結果が0以下なら当たり
			if (dot01 * dot02 <= 0.0f)
			{
				col = 1;
			}

		}
		else
		{
			col = 0;
		}

		//重なっていない場合

		//大きさ算出
		float start_to_center_length = std::sqrt((start_to_center.x * start_to_center.x) + (start_to_center.y * start_to_center.y));
		float end_to_center_length = std::sqrt((end_to_center.x * end_to_center.x) + (end_to_center.y * end_to_center.y));

		if (start_to_center_length < circleR ||
			end_to_center_length < circleR)
		{
			col = 1;
		}

		// 描画処理


		////あったっているときと,いないときで色を変える
		if (col == 0) {

			DrawCircle(circleX, circleY, circleR, GetColor(255, 255, 255), true);

			DrawLine(line1X, line1Y, line2X, line2Y, GetColor(255, 255, 255), true);

		}
		else {

			DrawCircle(circleX, circleY, circleR, GetColor(255,0,0), true);

			DrawLine(line1X, line1Y, line2X, line2Y, GetColor(255, 0, 0), true);

		}

		//DrawLine(start_to_center.x, start_to_center.y, end_to_center.x, end_to_center.y, GetColor(0, 0, 255), true);

		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();

		// 20ミリ秒待機(疑似60FPS)
		WaitTimer(20);

		// Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1) {
			break;
		}

		// ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
}
