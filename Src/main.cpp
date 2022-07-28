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
    //�v���W�F�N�g�𒼉��ɍ쐬�����log.tex���쐬�ɂ���
    SetOutApplicationLogValidFlag(FALSE);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    //���̃Q�[���̐ݒ������������
    GameConfig::Initialize();
    //��������̏�����
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

    //�ϐ��̒�`�Ƃ�(�g��Ȃ��Ă�����)

    Eagle::System::SetUseDebugOutput(false);

    Eagle::System::Start();

    Eagle::System::Physics::SetFixedTimestep(1.f / 60.f);

    Eagle::System::SetProcedure(MainProc);

    Eagle::System::Fps::SetUseFpsControl(true);
    Eagle::System::Fps::SetLimit(144.f);

    //========================== �I�u�W�F�N�g�̐����Ɛݒ� ================================//

    GameScore::LoadHighScore();

    Eagle::GameObject::Create("BootProcessor")
        ->AttachComponent<BootProcessor>();

    //==================================================================================//

    while (DxLib::ProcessMessage() == 0 && Eagle::System::Run());

    GameScore::HighScoreUpdate();

    Eagle::System::End();

    GameConfig::Finalize();
    SoundManager::Finalize();

    //DxLib�̏I������
    DxLib::DxLib_End();

    return 0;
}

bool DxInitEx()
{
    auto windowSize = GameConfig::GetWindowSize();
    auto resolution = GameConfig::GetResolution();
    auto titleName  = GameConfig::GetTitleName();

    //�E�B���h�E�̉𑜓x��ݒ肷��
    SetGraphMode(windowSize.x, windowSize.y, 32);

    //�E�B���h�E�̑傫����ݒ肷��
    SetWindowSize(resolution.x, resolution.y);

    //TRUE�Ńv���O�������t���X�N���[������E�B���h�E�ɕς���(�f�o�b�O����FALSE�ɂ��邱�Ƃ͂����߂��܂���)
    ChangeWindowMode(TRUE);

    //����ʂɕ`����s��
    SetDrawScreen(DX_SCREEN_BACK);

    //�񓯊��ǂݍ��݂��\�ɂ���(�t�@�C���̓ǂݍ��݂��I������܂Ńv���O�������҂��Ȃ�)
    SetUseASyncLoadFlag(true);

    //��ʂ̔w�i�F��ݒ�
    SetBackgroundColor(0, 0, 0);

    //�E�B���h�E�̍���ɕt�����閼�O
    SetMainWindowText(titleName.c_str());

    SetWaitVSyncFlag(false);

    // �c�w���C�u��������������
    if (DxLib_Init() == -1)
    {
        return false;
    }

    return true;
}