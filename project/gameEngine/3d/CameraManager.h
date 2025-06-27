#pragma once

#include "Camera.h"
#include <vector>
#include <memory>

#include <limits>

#undef max
#undef min

class CameraManager
{
public:
    // シングルトンパターンを使いたい場合
    static CameraManager& GetInstance() {
        static CameraManager instance;
        return instance;
    }

    // カメラの追加
    void AddCamera(std::shared_ptr<Camera> camera);

    // カメラの削除
    // シーンの変更時等に呼び出す
    void RemoveCamera(uint32_t index);

    // アップデート（全カメラの更新処理を実行）
    void UpdateAll(float deltaTime = 0.0f);

    // 特定のカメラのシェイクを開始
    void ShakeSpecificCamera(uint32_t index, float duration, float magnitude);

    // アクティブカメラのシェイクを開始
    // 軽い衝撃のシェイク
    // cameraManager.StartShakeActiveCamera(0.2f, 0.3f);
    // 中規模の爆発のシェイク
    // cameraManager.StartShakeActiveCamera(0.5f, 0.8f);
    // 大規模な爆発のシェイク
    // cameraManager.StartShakeActiveCamera(1.0f, 1.5f);
    void StartShakeActiveCamera(float duration, float magnitude);

	// 全てのカメラを削除
    void ClearAllCameras();

public: // ゲッター

    uint32_t GetActiveIndex()const { return activeCameraIndex_; }

    // アクティブカメラの取得
    std::shared_ptr<Camera> GetActiveCamera() const;

	// カメラの数を取得
	uint32_t GetCameraCount() const { return static_cast<uint32_t>(cameras_.size()); }

public: // セッター

    // アクティブカメラを設定
    void SetActiveCamera(uint32_t index);

private:
    CameraManager() = default;
    ~CameraManager() = default;

    CameraManager(const CameraManager&) = delete;
    CameraManager& operator=(const CameraManager&) = delete;

private:
    std::vector<std::shared_ptr<Camera>> cameras_;  // カメラのリスト
    uint32_t activeCameraIndex_ = ::std::numeric_limits<uint32_t>::max();  // アクティブなカメラのインデックス (無効値を設定)
};


