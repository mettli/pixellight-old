/*********************************************************\
 *  File: SNLoadScreenBase.cpp                           *
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
#include "PLScene/Scene/SceneContainer.h"
#include "PLScene/Scene/SceneNodes/SNLoadScreenBase.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNLoadScreenBase)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNLoadScreenBase::SNLoadScreenBase() :
	Flags(this),
	EventHandlerContainer   (&SNLoadScreenBase::OnContainer,	this),
	EventHandlerLoadProgress(&SNLoadScreenBase::OnLoadProgress, this),
	m_pContainer(nullptr),
	m_fLoadProgress(0.0f)
{
	// Overwrite the default setting of the flags
	SetFlags(GetFlags()|NoCulling);

	// Connect event handler
	SignalContainer.Connect(EventHandlerContainer);
}

/**
*  @brief
*    Destructor
*/
SNLoadScreenBase::~SNLoadScreenBase()
{
}

/**
*  @brief
*    Returns the current load progress
*/
float SNLoadScreenBase::GetLoadProgress() const
{
	return m_fLoadProgress;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node container changed
*/
void SNLoadScreenBase::OnContainer()
{
	// Disconnect event handler
	if (m_pContainer)
		m_pContainer->SignalLoadProgress.Disconnect(EventHandlerLoadProgress);

	// Connect event handler
	m_pContainer = GetContainer();
	if (m_pContainer)
		m_pContainer->SignalLoadProgress.Connect(EventHandlerLoadProgress);
}

/**
*  @brief
*    Called on load progress
*/
void SNLoadScreenBase::OnLoadProgress(float fLoadProgress)
{
	m_fLoadProgress = fLoadProgress;
}


//[-------------------------------------------------------]
//[ Public virtual SceneNode functions                    ]
//[-------------------------------------------------------]
void SNLoadScreenBase::DrawPost(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Show the load screen?
	if (m_fLoadProgress >= 0.0f && m_fLoadProgress <= 1.0f) {
		// Call base implementation
		SceneNode::DrawPost(cRenderer, pVisNode);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene