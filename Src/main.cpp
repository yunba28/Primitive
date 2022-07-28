#include "game_utility.hpp"
#include "boot_processor.hpp"
#include "game_score.hpp"
#include "sound_manager.hpp"

#include <crtdbg.h>

void MainProc(Eagle::Scene* _scene)
{
    Input::Update();
    clsDx();
    _scene->Update();
    ClearDrawScreen();
    _scene->Render();
    ScreenFlip();
}

bool DxInitEx();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //プロジェクトを直下に作成されるlog.texを非作成にする
    SetOutApplicationLogValidFlag(FALSE);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    //このゲームの設定を初期化する
    GameConfig::Initialize();
    //音声昨日の初期化
    SoundManager::Initialize();

    const int WindWidth = 800;
    const int WindHeight = 600;

    GameConfig::SetWindowSize(WindWidth, WindHeight);
    GameConfig::SetResolution(WindWidth, WindHeight);
    GameConfig::SetTitleName("Primitive");

    if (!DxInitEx())
    {
        Debug::Message("Code : -1 Initialize Error", "Error Code");
        return -1;
    }

    //変数の定義とか(使わなくてもいい)

    Eagle::System::SetUseDebugOutput(false);

    Eagle::System::Start();

    Eagle::System::Physics::SetFixedTimestep(1.f / 60.f);

    Eagle::System::SetProcedure(MainProc);

    Eagle::System::Fps::SetUseFpsControl(true);
    Eagle::System::Fps::SetLimit(144.f);

    //========================== オブジェクトの生成と設定 ================================//

    GameScore::LoadHighScore();

    Eagle::GameObject::Create("BootProcessor")
        ->AttachComponent<BootProcessor>();

    //==================================================================================//

    while (DxLib::ProcessMessage() == 0 && Eagle::System::Run());

    GameScore::HighScoreUpdate();

    Eagle::System::End();

    GameConfig::Finalize();
    SoundManager::Finalize();

    //DxLibの終了処理
    DxLib::DxLib_End();

    return 0;
}

bool DxInitEx()
{
    auto windowSize = GameConfig::GetWindowSize();
    auto resolution = GameConfig::GetResolution();
    auto titleName  = GameConfig::GetTitleName();

    //ウィンドウの解像度を設定する
    SetGraphMode(windowSize.x, windowSize.y, 32);

    //ウィンドウの大きさを設定する
    SetWindowSize(resolution.x, resolution.y);

    //TRUEでプログラムをフルスクリーンからウィンドウに変える(デバッグ時にFALSEにすることはお勧めしません)
    ChangeWindowMode(TRUE);

    //裏画面に描画を行う
    SetDrawScreen(DX_SCREEN_BACK);

    //非同期読み込みを可能にする(ファイルの読み込みが終了するまでプログラムが待たない)
    SetUseASyncLoadFlag(true);

    //画面の背景色を設定
    SetBackgroundColor(0, 0, 0);

    //ウィンドウの左上に付けられる名前
    SetMainWindowText(titleName.c_str());

    SetWaitVSyncFlag(false);

    // ＤＸライブラリ初期化処理
    if (DxLib_Init() == -1)
    {
        return false;
    }

    return true;
}