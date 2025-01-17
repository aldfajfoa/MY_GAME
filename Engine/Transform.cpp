#include "Transform.h"
#include "Direct3D.h"

Transform::Transform()
    :position_({ 0,0,0 }),rotate_({ 0,0,0 }),scale_({ 1,1,1 }),
    matTranslate_(XMMatrixIdentity()),
    matRotate_(XMMatrixIdentity()),
    matScale_(XMMatrixIdentity()),
    pParent_(nullptr)
{
}

Transform::~Transform()
{
}

void Transform::Release()
{
    SAFE_RELEASE(pParent_);
    SAFE_DELETE(pParent_);
}

void Transform::Calculation()
{
    //SRTの順番で掛けると、ワールド変換になる
    matTranslate_ = XMMatrixTranslation(position_.x, position_.y, position_.z);

    //R=Z*X*Yの順番で行列を掛けるとDirectXの回転になる
    XMMATRIX rx = XMMatrixRotationX(XMConvertToRadians( rotate_.x ));
    XMMATRIX ry = XMMatrixRotationY(XMConvertToRadians( rotate_.y ));
    XMMATRIX rz = XMMatrixRotationZ(XMConvertToRadians( rotate_.z ));
    matRotate_ = rz * rx * ry; //R = z * x * y

    matScale_ = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
}

XMMATRIX Transform::GetWorldMatrix()
{
    if (pParent_ != nullptr) 
    {
        return matScale_ * matRotate_ * matTranslate_ * pParent_->GetWorldMatrix();
    }
    else 
    {
        return matScale_ * matRotate_ * matTranslate_;
    }
}

XMMATRIX Transform::GetNormalMatrix()
{
    //平行移動は無視、回転は掛ける、スケールは元に戻す
    return(matRotate_ * XMMatrixInverse(nullptr, matScale_));
}
