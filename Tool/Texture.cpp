#include "stdafx.h"
#include "Texture.h"
#include "Device.h"

CTexture::CTexture()
: m_pDevice(CDevice::GetInstance())
{
}

CTexture::~CTexture()
{
}