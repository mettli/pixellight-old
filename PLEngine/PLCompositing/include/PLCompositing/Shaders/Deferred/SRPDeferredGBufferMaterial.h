/*********************************************************\
 *  File: SRPDeferredGBufferMaterial.h                   *
 *
 *  Copyright (C) 2002-2011 The PixelLight Team (http://www.pixellight.org/)
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


#ifndef __PLCOMPOSITING_DEFERRED_GBUFFER_MATERIAL_H__
#define __PLCOMPOSITING_DEFERRED_GBUFFER_MATERIAL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2.h>
#include <PLGraphics/Color/Color3.h>
#include <PLRenderer/Renderer/ProgramGenerator.h>
#include "PLCompositing/PLCompositing.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Material;
	class Renderer;
	class TextureBuffer;
	class ProgramUniform;
	class ProgramAttribute;
	class ProgramGenerator;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    SRPDeferredGBuffer-material
*/
class SRPDeferredGBufferMaterial {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Texture filtering modes
		*/
		enum ETextureFiltering {
			NoFiltering   = 0,	/**< No filtering */
			Bilinear      = 1,	/**< Bilinear */
			Anisotropic2  = 2,	/**< Anisotropic x2 */
			Anisotropic4  = 4,	/**< Anisotropic x4 */
			Anisotropic8  = 8,	/**< Anisotropic x8 */
			Anisotropic16 = 16	/**< Anisotropic x16 */
		};
		/**
		*  @brief
		*    Texture filtering modes
		*/
		pl_enum(ETextureFiltering)
			pl_enum_value(NoFiltering,		"No filtering")
			pl_enum_value(Bilinear,			"Bilinear")
			pl_enum_value(Anisotropic2,		"Anisotropic x2")
			pl_enum_value(Anisotropic4,		"Anisotropic x4")
			pl_enum_value(Anisotropic8,		"Anisotropic x8")
			pl_enum_value(Anisotropic16,	"Anisotropic x16")
		pl_enum_end

		/**
		*  @brief
		*    Direct pointers to uniforms & attributes of a generated program
		*/
		struct GeneratedProgramUserData {
			// Vertex shader attributes
			PLRenderer::ProgramAttribute *pVertexPosition;
			PLRenderer::ProgramAttribute *pVertexTexCoord0;
			PLRenderer::ProgramAttribute *pVertexTexCoord1;
			PLRenderer::ProgramAttribute *pVertexNormal;
			PLRenderer::ProgramAttribute *pVertexTangent;
			PLRenderer::ProgramAttribute *pVertexBinormal;
			// Vertex shader uniforms
			PLRenderer::ProgramUniform *pNormalScale;
			PLRenderer::ProgramUniform *pEyePos;
			PLRenderer::ProgramUniform *pWorldVP;
			PLRenderer::ProgramUniform *pWorldV;
			PLRenderer::ProgramUniform *pDisplacementMap;
			PLRenderer::ProgramUniform *pDisplacementScaleBias;
			// Fragment shader uniforms
			PLRenderer::ProgramUniform *pDiffuseColor;
			PLRenderer::ProgramUniform *pDiffuseMap;
			PLRenderer::ProgramUniform *pAlphaReference;
			PLRenderer::ProgramUniform *pSpecularColor;
			PLRenderer::ProgramUniform *pSpecularExponent;
			PLRenderer::ProgramUniform *pSpecularMap;
			PLRenderer::ProgramUniform *pNormalMap;
			PLRenderer::ProgramUniform *pNormalMapBumpiness;
			PLRenderer::ProgramUniform *pDetailNormalMap;
			PLRenderer::ProgramUniform *pDetailNormalMapBumpiness;
			PLRenderer::ProgramUniform *pDetailNormalMapUVScale;
			PLRenderer::ProgramUniform *pHeightMap;
			PLRenderer::ProgramUniform *pParallaxScaleBias;
			PLRenderer::ProgramUniform *pAmbientOcclusionMap;
			PLRenderer::ProgramUniform *pAmbientOcclusionFactor;
			PLRenderer::ProgramUniform *pLightMap;
			PLRenderer::ProgramUniform *pLightMapColor;
			PLRenderer::ProgramUniform *pEmissiveMap;
			PLRenderer::ProgramUniform *pEmissiveMapColor;
			PLRenderer::ProgramUniform *pGlowFactor;
			PLRenderer::ProgramUniform *pGlowMap;
			PLRenderer::ProgramUniform *pReflectionColor;
			PLRenderer::ProgramUniform *pReflectivity;
			PLRenderer::ProgramUniform *pReflectivityMap;
			PLRenderer::ProgramUniform *pFresnelConstants;
			PLRenderer::ProgramUniform *pReflectionMap;
			PLRenderer::ProgramUniform *pViewSpaceToWorldSpace;
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cMaterial
		*    Owner material
		*  @param[in] cProgramGenerator
		*    Program generator
		*/
		SRPDeferredGBufferMaterial(PLRenderer::Material &cMaterial, PLRenderer::ProgramGenerator &cProgramGenerator);

		/**
		*  @brief
		*    Destructur
		*/
		~SRPDeferredGBufferMaterial();

		/**
		*  @brief
		*    Makes this material to the currently used one
		*
		*  @param[in]  nFlags
		*    SRPDeferredGBuffer-flags to use
		*  @param[in]  nTextureFiltering
		*    Texture filtering
		*  @param[out] bColorTarget3Used
		*    Has color target 3 real information?
		*  @param[out] bColorTarget3AlphaUsed
		*    Has the alpha channel of target 3 real information?
		*
		*  @return
		*    Generated program user data, do NOT delete the memory the pointer points to
		*/
		GeneratedProgramUserData *MakeMaterialCurrent(PLGeneral::uint32 nFlags, ETextureFiltering nTextureFiltering, bool &bColorTarget3Used, bool &bColorTarget3AlphaUsed);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Vertex shader flags, flag names become to source code definitions
		*/
		enum EVertexShaderFlags {
			VS_TWOSIDED					= 1<<0,	/**< Two sided material */
			VS_DISPLACEMENTMAP			= 1<<1,	/**< Displacement map */
			VS_SECONDTEXTURECOORDINATE	= 1<<2,	/**< Second texture coordinate */
			VS_TANGENT_BINORMAL			= 1<<3,	/**< Tangent and binormal vectors */
			VS_VIEWSPACEPOSITION		= 1<<4	/**< Calculate viewspace position */
		};

		/**
		*  @brief
		*    Fragment shader flags, flag names become to source code definitions
		*/
		enum EFragmentShaderFlags {
			FS_DIFFUSEMAP							= 1<<0,		/**< Take diffuse map into account */
				FS_ALPHATEST						= 1<<1,		/**< Use alpha test to discard fragments (FS_DIFFUSEMAP must be defined!) */
			FS_SPECULAR								= 1<<2,		/**< Use specular */
				FS_SPECULARMAP						= 1<<3,		/**< Take specular map into account (FS_SPECULAR must be set, too) */
			FS_NORMALMAP							= 1<<4,		/**< Take normal map into account */
				FS_NORMALMAP_DXT5_XGXR				= 1<<5,		/**< DXT5 XGXR compressed normal map (FS_NORMALMAP must be defined and FS_NORMALMAP_LATC2 not!) */
				FS_NORMALMAP_LATC2					= 1<<6,		/**< LATC2 compressed normal map (FS_NORMALMAP must be defined and FS_NORMALMAP_DXT5_XGXR not!) */
				FS_DETAILNORMALMAP					= 1<<7,		/**< Take detail normal map into account (FS_NORMALMAP must be defined!) */
					FS_DETAILNORMALMAP_DXT5_XGXR	= 1<<8,		/**< DXT5 XGXR compressed detail normal map (FS_NORMALMAP & FS_DETAILNORMALMAP must be defined and FS_DETAILNORMALMAP_LATC2 not!) */
					FS_DETAILNORMALMAP_LATC2		= 1<<9,		/**< LATC2 compressed detail normal map (FS_NORMALMAP & FS_DETAILNORMALMAP must be defined and FS_DETAILNORMALMAP_DXT5_XGXR not!) */
			FS_PARALLAXMAPPING						= 1<<10,	/**< Perform parallax mapping */
			FS_AMBIENTOCCLUSIONMAP					= 1<<11,	/**< Use ambient occlusion map */
			FS_LIGHTMAP								= 1<<12,	/**< Use light map */
			FS_EMISSIVEMAP							= 1<<13,	/**< Use emissive map */
			FS_GLOW									= 1<<14,	/**< Use glow */
				FS_GLOWMAP							= 1<<15,	/**< Use glow map (FS_GLOW must be defined!) */
			FS_REFLECTION							= 1<<16,	/**< Use reflection */
				FS_FRESNELREFLECTION				= 1<<17,	/**< Use fresnel reflection (FS_REFLECTION must be defined!) */
				FS_REFLECTIVITYMAP					= 1<<18,	/**< Use reflectivity map (FS_REFLECTION and FS_FRESNELREFLECTION or FS_2DREFLECTIONMAP or FS_CUBEREFLECTIONMAP must be defined!) */
				FS_2DREFLECTIONMAP					= 1<<19,	/**< Use 2D reflection mapping (FS_REFLECTION must be defined, can't be set together with FS_CUBEREFLECTIONMAP!) */
				FS_CUBEREFLECTIONMAP				= 1<<20,	/**< Use cube reflection mapping (FS_REFLECTION must be defined, can't be set together with FS_2DREFLECTIONMAP!) */
			FS_GAMMACORRECTION						= 1<<21		/**< Use gamma correction (sRGB to linear space) */
		};


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		// Material parameter
		static const PLGeneral::String TwoSided;
		static const PLGeneral::String DisplacementMap;
		static const PLGeneral::String DisplacementScale;
		static const PLGeneral::String DisplacementBias;
		static const PLGeneral::String IndexOfRefraction;
		static const PLGeneral::String FresnelReflectionPower;
		static const PLGeneral::String ReflectionColor;
		static const PLGeneral::String Reflectivity;
		static const PLGeneral::String Parallax;
		static const PLGeneral::String Glow;
		static const PLGeneral::String GlowMap;
		static const PLGeneral::String AlphaReference;
		static const PLGeneral::String SpecularColor;
		static const PLGeneral::String SpecularExponent;
		static const PLGeneral::String NormalMapBumpiness;
		static const PLGeneral::String DetailNormalMap;
		static const PLGeneral::String DetailNormalMapBumpiness;
		static const PLGeneral::String DetailNormalMapUVScale;
		// Shader parameter
		static const PLGeneral::String VertexPosition;
		static const PLGeneral::String VertexTexCoord0;
		static const PLGeneral::String VertexTexCoord1;
		static const PLGeneral::String VertexNormal;
		static const PLGeneral::String VertexTangent;
		static const PLGeneral::String VertexBinormal;
		static const PLGeneral::String NormalScale;
		static const PLGeneral::String EyePos;
		static const PLGeneral::String WorldVP;
		static const PLGeneral::String WorldV;
		static const PLGeneral::String DisplacementScaleBias;
		static const PLGeneral::String DiffuseColor;
		static const PLGeneral::String DiffuseMap;
		// static const PLGeneral::String AlphaReference;	// Already defined
		static const PLGeneral::String SpecularMap;
		static const PLGeneral::String NormalMap;
		static const PLGeneral::String HeightMap;
		static const PLGeneral::String ParallaxScaleBias;
		static const PLGeneral::String AmbientOcclusionMap;
		static const PLGeneral::String AmbientOcclusionFactor;
		static const PLGeneral::String LightMap;
		static const PLGeneral::String LightMapColor;
		static const PLGeneral::String EmissiveMap;
		static const PLGeneral::String EmissiveMapColor;
		static const PLGeneral::String GlowFactor;
		// static const PLGeneral::String GlowMap;	// Already defined
		static const PLGeneral::String ReflectivityMap;
		static const PLGeneral::String FresnelConstants;
		static const PLGeneral::String ReflectionMap;
		static const PLGeneral::String ViewSpaceToWorldSpace;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Synchronize this material cache with the owner
		*
		*  @param[in] nFlags
		*    SRPDeferredGBuffer-flags to use
		*/
		void Synchronize(PLGeneral::uint32 nFlags);

		/**
		*  @brief
		*    Sets correct texture filtering modes
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] nStage
		*    Texture stage
		*  @param[in] nTextureFiltering
		*    Texture filtering
		*/
		void SetupTextureFiltering(PLRenderer::Renderer &cRenderer, PLGeneral::uint32 nStage, ETextureFiltering nTextureFiltering) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// General
		PLRenderer::Material				*m_pMaterial;			/**< Owner material, always valid! */
		PLRenderer::ProgramGenerator		*m_pProgramGenerator;	/**< Program generator, always valid! */
		PLGeneral::uint32					 m_nFlags;				/**< Used SRPDeferredGBuffer-flags */
		// Generated program
		PLRenderer::ProgramGenerator::Flags	 m_cProgramFlags;		/**< Program flags as class member to reduce dynamic memory allocations */
		// Synchronized data
			// Two sided
		bool					   m_bTwoSided;
			// Displacement mapping settings
		float					   m_fDisplacementScale;
		float					   m_fDisplacementBias;
		PLRenderer::TextureBuffer *m_pDisplacementMap;
			// Index of refraction and fresnel reflection power
		float					   m_fIndexOfRefraction;
		float					   m_fFresnelReflectionPower;
			// (2D/cube) reflection map
		PLRenderer::TextureBuffer *m_pReflectionMap;
		bool					   m_b2DReflectionMap;
			// Figure out whether or not there's reflection on this material
		bool					   m_bReflection;
			// Reflection parameters
		PLRenderer::TextureBuffer *m_pReflectivityMap;
		float					   m_fReflectivity;
		PLGraphics::Color3		   m_cReflectionColor;
			// Parallax mapping settings
		float					   m_fParallax;
		PLRenderer::TextureBuffer *m_pHeightMap;
			// Glow
		float					   m_fGlowFactor;
		PLRenderer::TextureBuffer *m_pGlowMap;
			// Ambient occlusion map
		float					   m_fAmbientOcclusionFactor;
		PLRenderer::TextureBuffer *m_pAmbientOcclusionMap;
			// Diffuse map and alpha reference
		PLGraphics::Color3		   m_cDiffuseColor;
		float					   m_fAlphaReference;
		PLRenderer::TextureBuffer *m_pDiffuseMap;
			// Specular settings
		bool					   m_bSpecular;
		PLGraphics::Color3		   m_cSpecularColor;
		PLRenderer::TextureBuffer *m_pSpecularMap;
		float					   m_fSpecularExponent;
			// Normal map
		PLRenderer::TextureBuffer *m_pNormalMap;
		float					   m_fNormalMapBumpiness;
		bool					   m_bNormalMap_DXT5_xGxR;
		bool					   m_bNormalMap_LATC2;
			// Detail normal map
		PLRenderer::TextureBuffer *m_pDetailNormalMap;
		float					   m_fDetailNormalMapBumpiness;
		PLMath::Vector2			   m_vDetailNormalMapUVScale;
		bool					   m_bDetailNormalMap_DXT5_xGxR;
		bool					   m_bDetailNormalMap_LATC2;
			// Light map
		PLGraphics::Color3		   m_cLightMapColor;
		PLRenderer::TextureBuffer *m_pLightMap;
			// Emissive map
		PLGraphics::Color3		   m_cEmissiveMapColor;
		PLRenderer::TextureBuffer *m_pEmissiveMap;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_DEFERRED_GBUFFER_MATERIAL_H__
