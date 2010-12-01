/*********************************************************\
 *  File: SRPDeferredEdgeAA.cpp                          *
 *
 *  Copyright (C) 2002-2010 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  PixelLight is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PixelLight is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color3.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include <PLRenderer/Effect/EffectManager.h>
#include "PLCompositing/FullscreenQuad.h"
#include "PLCompositing/SRPBegin.h"
#include "PLCompositing/Shaders/Deferred/SRPDeferredGBuffer.h"
#include "PLCompositing/Shaders/Deferred/SRPDeferredEdgeAA.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPDeferredEdgeAA)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDeferredEdgeAA::SRPDeferredEdgeAA() :
	ShaderLanguage(this),
	WeightScale(this),
	Flags(this),
	m_pProgramGenerator(NULL)
{
}

/**
*  @brief
*    Destructor
*/
SRPDeferredEdgeAA::~SRPDeferredEdgeAA()
{
	// Destroy the program generator
	if (m_pProgramGenerator)
		delete m_pProgramGenerator;
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPDeferredEdgeAA::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Get the instance of the "PLCompositing::SRPDeferredGBuffer" scene renderer pass
	SRPDeferredGBuffer *pSRPDeferredGBuffer = GetGBuffer();
	if (pSRPDeferredGBuffer) {
		// Get the fullscreen quad instance
		FullscreenQuad *pFullscreenQuad = pSRPDeferredGBuffer->GetFullscreenQuad();
		if (pFullscreenQuad) {
			// Get the vertex buffer of the fullscreen quad
			VertexBuffer *pVertexBuffer = pFullscreenQuad->GetVertexBuffer();
			if (pVertexBuffer) {
				// Get the normal/depth texture buffer to use
				TextureBufferRectangle *pNormalDepthTextureBuffer = pSRPDeferredGBuffer->GetRenderTargetTextureBuffer(1);
				if (pNormalDepthTextureBuffer) {
					// Get the "PLCompositing::SRPBegin" instance
					SRPBegin *pSRPBegin = (SRPBegin*)GetFirstInstanceOfSceneRendererPassClass("PLCompositing::SRPBegin");
					if (pSRPBegin) {
						// We need up-to-date front render target content, so swap the render targets
						pSRPBegin->SwapRenderTargets();

						// Get the front render target of SRPBegin, this holds the current content
						SurfaceTextureBuffer *pFrontSurfaceTextureBuffer = pSRPBegin->GetFrontRenderTarget();
						if (pFrontSurfaceTextureBuffer && pFrontSurfaceTextureBuffer->GetTextureBuffer()) {
							// Get the shader language to use
							String sShaderLanguage = ShaderLanguage;
							if (!sShaderLanguage.GetLength())
								sShaderLanguage = cRenderer.GetDefaultShaderLanguage();

							// Create the program generator if there's currently no instance of it
							if (!m_pProgramGenerator || m_pProgramGenerator->GetShaderLanguage() != sShaderLanguage) {
								// If there's an previous instance of the program generator, destroy it first
								if (m_pProgramGenerator) {
									delete m_pProgramGenerator;
									m_pProgramGenerator = NULL;
								}

								// Choose the shader source codes depending on the requested shader language
								String sDeferredEdgeAA_VS;
								String sDeferredEdgeAA_FS;
								if (sShaderLanguage == "GLSL") {
									#include "SRPDeferredEdgeAA_GLSL.h"
									sDeferredEdgeAA_VS = sDeferredEdgeAA_GLSL_VS;
									sDeferredEdgeAA_FS = sDeferredEdgeAA_GLSL_FS;
								} else if (sShaderLanguage == "Cg") {
									#include "SRPDeferredEdgeAA_Cg.h"
									sDeferredEdgeAA_VS = sDeferredEdgeAA_Cg_VS;
									sDeferredEdgeAA_FS = sDeferredEdgeAA_Cg_FS;
								}

								// Create the program generator
								if (sDeferredEdgeAA_VS.GetLength() && sDeferredEdgeAA_FS.GetLength())
									m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDeferredEdgeAA_VS, "arbvp1", sDeferredEdgeAA_FS, "arbfp1", true);
							}

							// If there's no program generator, we don't need to continue
							if (m_pProgramGenerator) {
								// Reset all render states to default
								cRenderer.GetRendererContext().GetEffectManager().Use();

								// Reset the program flags
								m_cProgramFlags.Reset();

								// Show edges?
								if ((GetFlags() & ShowEdges) || (GetFlags() & ShowEdgesOnly))
									PL_ADD_FS_FLAG(m_cProgramFlags, FS_SHOW_EDGES)

								// Show edges only?
								if ((GetFlags() & ShowEdgesOnly) != 0)
									PL_ADD_FS_FLAG(m_cProgramFlags, FS_SHOW_EDGESONLY)

								// Take more samples?
								if ((GetFlags() & MoreSamples) != 0)
									PL_ADD_FS_FLAG(m_cProgramFlags, FS_MORE_SAMPLES)

								// Get a program instance from the program generator using the given program flags
								ProgramGenerator::GeneratedProgram *pGeneratedProgram = m_pProgramGenerator->GetProgram(m_cProgramFlags);

								// Make our program to the current one
								if (pGeneratedProgram && cRenderer.SetProgram(pGeneratedProgram->pProgram)) {
									// Set pointers to uniforms & attributes of a generated program if they are not set yet
									GeneratedProgramUserData *pGeneratedProgramUserData = (GeneratedProgramUserData*)pGeneratedProgram->pUserData;
									if (!pGeneratedProgramUserData) {
										pGeneratedProgram->pUserData = pGeneratedProgramUserData = new GeneratedProgramUserData;
										Program *pProgram = pGeneratedProgram->pProgram;
										// Vertex shader attributes
										static const String sVertexPosition = "VertexPosition";
										pGeneratedProgramUserData->pVertexPosition	= pProgram->GetAttribute(sVertexPosition);
										// Vertex shader uniforms
										static const String sTextureSize = "TextureSize";
										pGeneratedProgramUserData->pTextureSize		= pProgram->GetUniform(sTextureSize);
										// Fragment shader uniforms
										static const String sMinGradient = "MinGradient";
										pGeneratedProgramUserData->pMinGradient		= pProgram->GetUniform(sMinGradient);
										static const String sWeightScale = "WeightScale";
										pGeneratedProgramUserData->pWeightScale		= pProgram->GetUniform(sWeightScale);
										static const String sEdgeColor = "EdgeColor";
										pGeneratedProgramUserData->pEdgeColor		= pProgram->GetUniform(sEdgeColor);
										static const String sFrontMap = "FrontMap";
										pGeneratedProgramUserData->pFrontMap		= pProgram->GetUniform(sFrontMap);
										static const String sNormalDepthMap = "NormalDepthMap";
										pGeneratedProgramUserData->pNormalDepthMap	= pProgram->GetUniform(sNormalDepthMap);
									}

									// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
									if (pGeneratedProgramUserData->pVertexPosition)
										pGeneratedProgramUserData->pVertexPosition->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);

									// Set texture size
									if (pGeneratedProgramUserData->pTextureSize)
										pGeneratedProgramUserData->pTextureSize->Set(pNormalDepthTextureBuffer->GetSize());

									// Set min gradient
									if (pGeneratedProgramUserData->pMinGradient)
										pGeneratedProgramUserData->pMinGradient->Set((pNormalDepthTextureBuffer->GetFormat() == TextureBuffer::R32G32B32A32F) ? 0.00001f : 0.001f);

									// Set weight scale
									if (pGeneratedProgramUserData->pWeightScale)
										pGeneratedProgramUserData->pWeightScale->Set(WeightScale);

									// Set edge color
									if (pGeneratedProgramUserData->pEdgeColor)
										pGeneratedProgramUserData->pEdgeColor->Set((GetFlags() & ShowEdgesOnly) ? Color3::White : Color3::Red);

									// Front map
									if (pGeneratedProgramUserData->pFrontMap) {
										const int nTextureUnit = pGeneratedProgramUserData->pFrontMap->Set(pFrontSurfaceTextureBuffer->GetTextureBuffer());
										if (nTextureUnit >= 0) {
											cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Clamp);
											cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Clamp);
											cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Linear);
											cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Linear);
											cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
										}
									}

									// Normal depth map
									if (pGeneratedProgramUserData->pNormalDepthMap) {
										const int nTextureUnit = pGeneratedProgramUserData->pNormalDepthMap->Set(pNormalDepthTextureBuffer);
										if (nTextureUnit >= 0) {
											cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU,  TextureAddressing::Clamp);
											cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV,  TextureAddressing::Clamp);
											cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
											cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
											cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
										}
									}

									// Draw the fullscreen quad
									cRenderer.SetRenderState(RenderState::ScissorTestEnable, false);
									pFullscreenQuad->Draw(true);
								}
							}
						}
					}
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
