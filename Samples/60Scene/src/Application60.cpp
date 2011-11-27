/*********************************************************\
 *  File: Application60.cpp                              *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Tools/Timing.h>
#include <PLInput/Input/Controller.h>
#include <PLInput/Input/Controls/Control.h>
#include <PLRenderer/RendererContext.h>
#include <PLScene/Scene/SPScene.h>
#include <PLScene/Scene/SceneContainer.h>
#include "Application60.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLInput;
using namespace PLRenderer;
using namespace PLScene;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(Application60)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Application60::Application60(Frontend &cFrontend) : EngineApplication(cFrontend),
	SlotOnControl(this)
{
}

/**
*  @brief
*    Destructor
*/
Application60::~Application60()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a control event has occurred
*/
void Application60::OnControl(Control &cControl)
{
	// Check whether the escape key was pressed
	if (cControl.GetType() == ControlButton && cControl.GetName() == "KeyboardEscape")
		Exit(0); // Shut down the application
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::CoreApplication functions     ]
//[-------------------------------------------------------]
void Application60::OnInit()
{
	// This method is call by the application framework when application should initialize itself.
	// At this point, the core components of the application are ready to be used.

	// Call base implementation
	EngineApplication::OnInit();

	// Set the used scene renderer (optional filename of a fallback scene renderer to use in case the desired scene renderer can't be used as second parameter)
	if (GetSceneRendererTool().SetSceneRenderer(GetScene(), "Deferred.sr", "FixedFunctions.sr")) {
		// Configure the compositing system by using the comfort scene renderer tool.

		// Clear the content of the current used render target by using gray (this way, in case on an graphics error we might still see at least something)
		GetSceneRendererTool().SetPassAttribute("Begin", "ColorClear", "0.5 0.5 0.5 0");

		// We're setting the attribute "Flags" of the scene renderer step named "DeferredSPAAO" to the value "Inactive" -
		// this has the effect that the fillrate eating HBAO post processing effect is deactivated. Please note, that
		// internally "just" the generic PixelLight RTTI is used. PixelLight itself doesn't offer such settings as
		// "low graphics quality" because the framework can't decide automatically for you what is considered "low quality"
		// within your application. We highly recommend to provide your end-user more abstract graphics settings as
		// seen in, for example, many video games out there.
		GetSceneRendererTool().SetPassAttribute("DeferredSPAAO", "Flags", "Inactive");
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::AbstractFrontend functions    ]
//[-------------------------------------------------------]
void Application60::OnUpdate()
{
	// One important word at the beginning: DON'T COPYCAT THIS!
	// The following is 'just' a simple demonstration how the scene graph 'can' be used. It's
	// definitely not good to update your scene nodes in the way you can see within this function.
	// Its quite to intricate, inflexible and not performant. Use for example a scene node modifier
	// added to your scene node (in this case 'the white light') for this job!

	// Call base implementation
	EngineApplication::OnUpdate();

	// Get the scene container with our 'concrete scene'
	SceneContainer *pSceneContainer = GetScene();
	if (pSceneContainer) {
		// Get the scene node with the name 'Light' (our 'white light')
		SceneNode *pLightSceneNode = pSceneContainer->GetByName("Light");
		if (pLightSceneNode) {
			// This variable is used for the light animation. Its just static you keep the implementation
			// for a good sample overview completely within this function.
			static float fLightTimer = 0.0f;

			// We set the current light position using the RTTI class interface. This is quite comfortable
			// and universal because you haven't to care about the concrete class type - just set the
			// variable values. For performance critical situations it's recommend to avoid using this RTTI
			// functions to set your variables and use the concrete provided class interfaces instead.
			pLightSceneNode->SetAttribute("Position", String::Format("%g %g %g", Math::Sin(fLightTimer),
				Math::Sin(fLightTimer)/2+2, -(Math::Cos(fLightTimer)+5)));

			// Update the light timer by using the time difference between the last and the current frame
			fLightTimer += Timing::GetInstance()->GetTimeDifference();
		}
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLEngine::EngineApplication functions ]
//[-------------------------------------------------------]
void Application60::OnCreateScene(SceneContainer &cContainer)
{
	// Create a camera scene node
	SceneNode *pCameraSceneNode = cContainer.Create("PLScene::SNCamera", "FreeCamera", "Position=\"1 2 -3\" Rotation=\"25 210 0\"");
	if (pCameraSceneNode && pCameraSceneNode->IsInstanceOf("PLScene::SNCamera")) {
		// Make this to our main scene camera
		SetCamera(reinterpret_cast<SNCamera*>(pCameraSceneNode));
	}

	// Create a scene node with the soldier mesh which can produce a shadow
	SceneNode *pSoldierSceneNode = cContainer.Create("PLScene::SNMesh", "Soldier", "Flags=\"CastShadow|ReceiveShadow\" Position=\"0.0 0.1 -5.0\" Scale=\"0.008 0.008 0.008\" Mesh=\"Data/Meshes/Soldier.mesh\"");
	if (pSoldierSceneNode) {
		// Add a scene node modifier which will constantly rotate the soldier
		pSoldierSceneNode->AddModifier("PLScene::SNMRotationLinearAnimation", "Velocity=\"0 10 0\"");

		// Add a scene node modifier which will playback the animation named "walk_0" letting the soldier walk
		pSoldierSceneNode->AddModifier("PLScene::SNMMeshAnimation", "Name=\"walk_0\"");

		// Add a scene node modifier which will animate the morph target named "blink" letting the soldier blink from time to time
		pSoldierSceneNode->AddModifier("PLScene::SNMMeshMorphBlink", "Name=\"blink\"");
	}

	// Create a light source scene node to illuminate the scene - this light can cast shadows
	cContainer.Create("PLScene::SNPointLight", "Light", "Flags=\"CastShadow|Flares|Corona\" Range=\"4\"");

	// Create the floor scene node
	cContainer.Create("PLScene::SNMesh", "Floor", "Flags=\"CastShadow|ReceiveShadow\" Position=\"0.0 0.0 -5.0\" Rotation=\"0.0 180.0 0.0\" Scale=\"4.0 0.1 4.0\" Mesh=\"Default\"");

	// Set scene container
	SetScene(&cContainer);
}

void Application60::OnCreateInputController()
{
	// Call base implementation
	EngineApplication::OnCreateInputController();

	// Get virtual input controller
	Controller *pController = reinterpret_cast<Controller*>(GetInputController());
	if (pController)
		pController->SignalOnControl.Connect(SlotOnControl);
}