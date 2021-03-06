/*********************************************************\
 *  File: PLSceneMaterialFX.cpp                          *
 *
 *  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 *  and associated documentation files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or
 *  substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <IGame/IGame.h>
#include <IGame/IGameFX.h>
#include <iparamb2.h>
#include <pbbitmap.h>
#include <IDxMaterial.h>
#include "PL3dsMaxSceneExport/PLLog.h"
#include "PL3dsMaxSceneExport/PLScene.h"
#include "PL3dsMaxSceneExport/PLSceneMaterialFX.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
PLSceneMaterialFX::PLSceneMaterialFX(PLScene &cScene, IGameMaterial *pParentIGameMaterial, IGameMaterial &cIGameMaterial, const String &sName) :
	PLSceneMaterial(cScene, pParentIGameMaterial, &cIGameMaterial, sName)
{
}

/**
*  @brief
*    Destructor
*/
PLSceneMaterialFX::~PLSceneMaterialFX()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLSceneMaterial functions             ]
//[-------------------------------------------------------]
void PLSceneMaterialFX::SaveParameters(XmlElement &cMaterialElement)
{
	// Get the 3ds Max material... I don't use IGame for this because I didn't get it working using it...
	Mtl *pMaxMaterial = m_pIGameMaterial->GetMaxMaterial();
	if (pMaxMaterial) {
		// DxMaterial or extend?
		IDxMaterial2 *pFXMaterial = static_cast<IDxMaterial2*>(m_pIGameMaterial->GetMaxMaterial()->GetInterface(IDXMATERIAL2_INTERFACE));
		if (!pFXMaterial && m_pParentIGameMaterial)
			pFXMaterial = static_cast<IDxMaterial2*>(m_pParentIGameMaterial->GetMaxMaterial()->GetInterface(IDXMATERIAL2_INTERFACE));
		if (pFXMaterial) { // Get information from the DirectX Shader material
			// Get number of bitmaps used
			int nNumOfBitmaps = pFXMaterial->GetNumberOfEffectBitmaps();

			// Write textures/bitmaps
			for (int i=0; i<nNumOfBitmaps; i++) {
				PBBitmap *pBitmap = pFXMaterial->GetEffectBitmap(i);
				if (pBitmap) {
					BitmapInfo &cBInfo = pBitmap->bi;
					const String sName = cBInfo.Name();
					if (!sName.CompareNoCase("None")) {
						String sSemantic;
						switch (pFXMaterial->GetBitmapUsage(i)) {
							case IDxMaterial2::UNKNOWN_MAP:
								sSemantic = "EmissiveMap";
								break;

							case IDxMaterial2::DIFFUSE_MAP:
								sSemantic = "DiffuseMap";
								break;

							case IDxMaterial2::BUMP_MAP:
								sSemantic = "HeightMap";
								break;

							case IDxMaterial2::SPECULAR_MAP:
								sSemantic = "SpecularMap";
								break;

							case IDxMaterial2::DISPLACEMENT_MAP:
								sSemantic = "DetailNormalMap";
								break;

							case IDxMaterial2::LIGHTMAP_MAP:
								sSemantic = "LightMap";
								break;

							case IDxMaterial2::NORMAL_MAP:
								sSemantic = "NormalMap";
								break;

							case IDxMaterial2::REFLECTION_MAP:
								sSemantic = "ReflectionMap";
								break;
						}
						if (sSemantic.GetLength())
							SaveTexture(cMaterialElement, sName, sSemantic);
					}
				}
			}
		}
	}
}
