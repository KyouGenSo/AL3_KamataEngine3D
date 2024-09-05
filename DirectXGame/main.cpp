#include "Audio.h"
#include "AxisIndicator.h"
#include "DirectXCommon.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "Xinput.h"

// シーン
#include "GameClearScene.h"
#include "GameOverScene.h"
#include "GameScene.h"
#include "TitleScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	XINPUT_STATE joyState;
	XINPUT_STATE prevState;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;

	// シーン
	GameScene* gameScene = nullptr;
	TitleScene* titleScene = nullptr;
	GameOverScene* gameOverScene = nullptr;
	GameClearScene* gameClearScene = nullptr;

	// シーン遷移用変数
	bool isTitle = true;
	bool isTitleInit = false;
	bool isGame = false;
	bool isGameInit = false;
	bool isGameOver = false;
	bool isGameClear = false;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"LE2A_10_キョウ_ゲンソ_AL3");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// bgm
	uint32_t bgmTitleSH_ = audio->LoadWave("titleBGM.wav");
	uint32_t bgmTitleVH_ = 0;
	uint32_t bgmGameSH_ = audio->LoadWave("inGameBGM.wav");
	uint32_t bgmGameVH_ = 0;

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	// ゲームシーンの初期化
	//gameScene = new GameScene();
	//gameScene->Initialize();

	// タイトルシーンの初期化
	titleScene = new TitleScene();

	// ゲームオーバーシーンの初期化
	gameOverScene = new GameOverScene();
	gameOverScene->Initialize();

	// ゲームクリアシーンの初期化
	gameClearScene = new GameClearScene();
	gameClearScene->Initialize();

	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		if (input->GetJoystickState(0, joyState) && input->GetJoystickStatePrevious(0, prevState)) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A && !(prevState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				if (isTitle) {
					isTitle = false;
					isGameOver = false;
					isGameClear = false;
					isGame = true;
				} else if (isGameOver) {
					isGameOver = false;
					isTitle = true;
				} else if (isGameClear) {
					isGameClear = false;
					isTitle = true;
				}
			}
		}

		// シーン遷移
		if (isTitle) {
			if (!isTitleInit) {
				titleScene->Initialize();
				audio->StopWave(bgmGameVH_);
				bgmTitleVH_ = audio->PlayWave(bgmTitleSH_, true, 0.2f);
				isGameInit = false;
				isTitleInit = true;
			}
		} else if (isGame) {
			if (!isGameInit) {
				gameScene = new GameScene();
				gameScene->Initialize();
				audio->StopWave(bgmTitleVH_);
				bgmGameVH_ = audio->PlayWave(bgmGameSH_, true, 0.2f);
				isTitleInit = false;
				isGameInit = true;
			}

			if (gameScene->GetIsGameClear()) {
				isGame = false;
				isGameClear = true;
			} else if (gameScene->GetIsGameOver()) {
				isGame = false;
				isGameOver = true;
			}
		} else if (isGameOver) {
			audio->StopWave(bgmGameVH_);
			isTitleInit = false;
			//if (isTitleInit) {
			//	
			//	delete gameScene; 
			//	
			//}
		} else if (isGameClear) {
			audio->StopWave(bgmGameVH_);
			isTitleInit = false;
			//if (isTitleInit) {
			//	audio->StopWave(bgmGameVH_);
			//	delete gameScene;
			//	
			//}
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();
		// ゲームシーンの毎フレーム処理
		if (isTitle) {
			titleScene->Update();
		} else if (isGame) {
			gameScene->Update();
		} else if (isGameOver) {
			gameOverScene->Update();
		} else if (isGameClear) {
			gameClearScene->Update();
		}
		// 軸表示の更新
		axisIndicator->Update();
		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();
		// シーンの描画
		if (isTitle) {
			titleScene->Draw();
		} else if (isGame) {
			gameScene->Draw();
		} else if (isGameOver) {
			gameOverScene->Draw();
		} else if (isGameClear) {
			gameClearScene->Draw();
		}
		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	delete gameScene;
	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}