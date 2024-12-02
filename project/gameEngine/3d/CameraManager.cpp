#include "CameraManager.h"

void CameraManager::AddCamera(std::shared_ptr<Camera> camera) {
    cameras_.push_back(camera);
    if (activeCameraIndex_ == -1) {
        activeCameraIndex_ = 0;  // 最初のカメラをアクティブにする
    }
}

void CameraManager::RemoveCamera(int index) {
    if (index >= 0 && index < cameras_.size()) {
        cameras_.erase(cameras_.begin() + index);
        if (index == activeCameraIndex_) {
            activeCameraIndex_ = cameras_.empty() ? -1 : 0;
        }
        else if (index < activeCameraIndex_) {
            --activeCameraIndex_;
        }
    }
}

void CameraManager::SetActiveCamera(int index) {
    if (index >= 0 && index < cameras_.size()) {
        activeCameraIndex_ = index;
    }
}

std::shared_ptr<Camera> CameraManager::GetActiveCamera() const {
    if (activeCameraIndex_ >= 0 && activeCameraIndex_ < cameras_.size()) {
        return cameras_[activeCameraIndex_];
    }
    return nullptr;
}

void CameraManager::UpdateAll() {
    for (auto& camera : cameras_) {
        camera->Update();
    }
}
