#include "GamePlayScene.h"
#include<TextureManager.h>
#include<ModelManager.h>
#include<SpriteCommon.h>
#include<Object3dCommon.h>
#include<Input.h>
#ifdef USE_IMGUI
#include<ImGuiManager.h>
#endif // USE_IMGUI
#include<SceneManager.h>
#include <ParticleCommon.h>
#include <ParticleManager.h>
#include <numbers>

void GamePlayScene::Finalize() {

}

void GamePlayScene::Initialize() {
    // カメラの初期化
    camera = std::make_unique<Camera>();
    camera->SetTranslate(Vector3(0.0f, 0.0f, -100.0f));
    camera->SetRotate(Vector3(0.0f, 0.0f, 0.0f));
    Object3dCommon::GetInstance()->SetDefaultCamera(camera.get());
    ParticleCommon::GetInstance()->SetDefaultCamera(camera.get());

    // テクスチャを読み込む
    TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
    TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");

    // .objファイルからモデルを読み込む
    ModelManager::GetInstance()->LoadModel("plane.obj");
    ModelManager::GetInstance()->LoadModel("monsterBallUV.obj");
    ModelManager::GetInstance()->LoadModel("terrain.obj");    
    
    ModelManager::GetInstance()->LoadModel("uvChecker.obj");    

    // 音声ファイルを追加
    soundData = SoundLoader::GetInstance()->SoundLoadWave("Resources/Alarm01.wav");

    // 音声プレイフラグ
    soundfige = 0;

    // スプライトの初期化
    //sprite = Sprite::Create("Resources/uvChecker.png", Vector2{ 0.0f,0.0f }, 0.0f, Vector2{ 360.0f,360.0f });

    // オブジェクト作成
    object3d = Object3d::Create("monsterBallUV.obj", Transform({ {1.0f, 1.0f, 1.0f}, {0.0f, -1.6f, 0.0f}, {0.0f, 0.0f, 0.0f} }));
    object3d->SetLighting(true);
    grass = Object3d::Create("terrain.obj", Transform({ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} }));

    // LevelLoader のインスタンスを生成
    levelLoader = new LevelLoader();

    // JSONファイルからレベルデータを読み込む
    levelData = levelLoader->LoadFile("untitled"); 
    ModelManager::GetInstance()->LoadModel("uvChecker.obj");    

    // レベルデータから読み込み、オブジェクト生成
    for (auto& objData : levelData->objects) {
        Transform tr;
        tr.scale = objData.scaling;
        tr.rotate = objData.rotation;
        tr.translate = objData.translation;

        // fileName に .obj を追加して検索用文字列を作成
        std::string modelName = objData.fileName + ".obj";

        // オブジェクトを生成して、オブジェクトコンテナに渡す
        auto obj = Object3d::Create(modelName, tr);
        object3ds_.push_back(std::move(obj));
    }
}

void GamePlayScene::Update() {
#pragma region  ImGuiの更新処理開始
#ifdef USE_IMGUI
    // object3d
    //object3d->DebugUpdata("Object3d");
    //grass->DebugUpdata("Grass");

    // Camera
    camera->DebugUpdate();

    ImGui::Begin("Object3D List");
int index = 0;
for (auto& obj : object3ds_) {
    // ラベルにインデックスを付ける（同じラベルだとImGuiで衝突するため）
    std::string label = "Object " + std::to_string(index);

    if (ImGui::TreeNode(label.c_str())) {
        Vector3 scale = obj->GetTransform().scale;
        Vector3 rotate = obj->GetTransform().rotate;
        Vector3 translate = obj->GetTransform().translate;

        ImGui::Text("Position: (%.2f, %.2f, %.2f)", translate.x, translate.y, translate.z);
        ImGui::Text("Rotation: (%.2f, %.2f, %.2f)", rotate.x, rotate.y, rotate.z);
        ImGui::Text("Scale: (%.2f, %.2f, %.2f)", scale.x, scale.y, scale.z);

        ImGui::TreePop();
    }

    index++;
}
ImGui::End();
#endif // USE_IMGUI
#pragma endregion ImGuiの更新処理終了 
    /*-------------------------------------------*/
    /*--------------Cameraの更新処理---------------*/
    /*------------------------------------------*/
    camera->Update();

#pragma region 全てのObject3d個々の更新処理

    // 更新処理

    object3d->Update();

    for (auto& object : object3ds_) {
        object->Update();
    }

#pragma endregion 全てのObject3d個々の更新処理

#pragma region 全てのSprite個々の更新処理
    

#pragma endregion 全てのSprite個々の更新処理
#ifdef USE_IMGUI
// ImGuiの描画前準備
    ImGuiManager::GetInstance()->End();
#endif // USE_IMGUI
}

void GamePlayScene::Draw() {

#pragma region 全てのObject3d個々の描画処理
    // 3Dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
    Object3dCommon::GetInstance()->Commondrawing();
    	
      
  //  object3d->Draw();

    for (auto& object : object3ds_) {
        object->Draw();
    }

    // パーティクルの描画準備。パーティクルの描画に共通のグラフィックスコマンドを積む 
    ParticleCommon::GetInstance()->Commondrawing();
#pragma endregion 全てのObject3d個々の描画処理

#pragma region 全てのSprite個々の描画処理
    // Spriteの描画準備。Spriteの描画に共通のグラフィックスコマンドを積む
    SpriteCommon::GetInstance()->Commondrawing();

#pragma endregion 全てのSprite個々の描画処理
}