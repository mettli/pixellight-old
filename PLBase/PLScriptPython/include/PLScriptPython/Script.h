/*********************************************************\
 *  File: Script.h                                       *
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


#ifndef __PLSCRIPTPYTHON_SCRIPT_H__
#define __PLSCRIPTPYTHON_SCRIPT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLScriptPython/Python.h"
#include <PLScript/Script.h>
#include "PLScriptPython/PLScriptPython.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScriptPython {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Python (http://www.python.org/) script implementation
*/
class Script : public PLScript::Script {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLSCRIPTPYTHON_RTTI_EXPORT, Script, "PLScriptPython", PLScript::Script, "Python (http://www.python.org/) script implementation")
		pl_properties
			pl_property("Language", "Python")
			pl_property("Formats",  "py,PY")
		pl_properties_end
		pl_constructor_0(DefaultConstructor, "Default constructor", "")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLSCRIPTPYTHON_API Script();

		/**
		*  @brief
		*    Destructor
		*/
		PLSCRIPTPYTHON_API virtual ~Script();


	//[-------------------------------------------------------]
	//[ Public virtual PLScript::Script functions             ]
	//[-------------------------------------------------------]
	public:
		PLSCRIPTPYTHON_API virtual bool AddDynamicFunction(const PLGeneral::String &sFunction, const PLCore::DynFunc &cDynFunc, const PLGeneral::String &sNamespace = "");
		PLSCRIPTPYTHON_API virtual bool RemoveAllDynamicFunctions();
		PLSCRIPTPYTHON_API virtual PLGeneral::String GetSourceCode() const;
		PLSCRIPTPYTHON_API virtual bool SetSourceCode(const PLGeneral::String &sSourceCode);
		PLSCRIPTPYTHON_API virtual bool BeginCall(const PLGeneral::String &sFunctionName, const PLGeneral::String &sFunctionSignature);
		PLSCRIPTPYTHON_API virtual void PushArgument(int nValue);
		PLSCRIPTPYTHON_API virtual void PushArgument(PLGeneral::uint8 nValue);
		PLSCRIPTPYTHON_API virtual void PushArgument(PLGeneral::uint16 nValue);
		PLSCRIPTPYTHON_API virtual void PushArgument(PLGeneral::uint32 nValue);
		PLSCRIPTPYTHON_API virtual void PushArgument(float fValue);
		PLSCRIPTPYTHON_API virtual void PushArgument(double fValue);
		PLSCRIPTPYTHON_API virtual bool EndCall();
		PLSCRIPTPYTHON_API virtual void GetReturn(int &nValue);
		PLSCRIPTPYTHON_API virtual void GetReturn(PLGeneral::uint8 &nValue);
		PLSCRIPTPYTHON_API virtual void GetReturn(PLGeneral::uint16 &nValue);
		PLSCRIPTPYTHON_API virtual void GetReturn(PLGeneral::uint32 &nValue);
		PLSCRIPTPYTHON_API virtual void GetReturn(float &fValue);
		PLSCRIPTPYTHON_API virtual void GetReturn(double &fValue);


	//[-------------------------------------------------------]
	//[ Private static Python callback functions              ]
	//[-------------------------------------------------------]
	private:
		/*
		*  @brief
		*    Python function callback
		*
		*  @param[in] pPythonSelf
		*    Python self object
		*  @param[in] pPythonArguments
		*    Python tuple object representing all arguments
		*
		*  @return
		*    Python result object, null pointer if an exception was set
		*/
		static PyObject *PythonFunctionCallback(PyObject *pPythonSelf, PyObject *pPythonArguments);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Script(const Script &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Script &operator =(const Script &cSource);

		/**
		*  @brief
		*    Returns a string containing the Python error description
		*
		*  @return
		*    A string containing the Python error description, empty string if there
		*    wasn't an error or there was an error in receiving the error description
		*/
		PLGeneral::String GetPythonErrorDescription() const;

		/**
		*  @brief
		*    Write a string into the log
		*
		*  @param[in] nLogLevel
		*    Log level
		*  @param[in] sText
		*    Text which should be written into the log
		*  @param[in] sErrorDescription
		*    Error description
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @remarks
		*    The text is written to the log only if the current
		*    log level is greater or equal to the specified value.
		*    This method is an extension of "PLGeneral::Log::Output()"
		*    which also adds the name of the script to the given
		*    text and the "sErrorDescription" content.
		*/
		bool LogOutputWithErrorDescription(PLGeneral::uint8 nLogLevel, const PLGeneral::String &sText, const PLGeneral::String &sErrorDescription);

		/**
		*  @brief
		*    Clears the script
		*/
		void Clear();

		/**
		*  @brief
		*    Increases the number of arguments
		*/
		void IncreaseNumOfArguments();

		/**
		*  @brief
		*    Adds a Python function
		*
		*  @param[in] pPythonDictionary
		*    Python dictionary were to add the function to
		*  @param[in] sFunction
		*    Name of the function
		*  @param[in] pPythonFunction
		*    Python function object
		*  @param[in] sNamespace
		*    Namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool AddPythonFunction(PyObject *pPythonDictionary, const PLGeneral::String &sFunction, PyObject *pPythonFunction, const PLGeneral::String &sNamespace) const;


	//[-------------------------------------------------------]
	//[ Private structures                                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    A dynamic function
		*/
		struct DynamicFunction {
			PLGeneral::String  sFunction;	/**< Function name used inside the script to call the dynamic function */
			PLCore::DynFunc   *pDynFunc;	/**< Dynamic function to be called, always valid, destroy when done */
			PLGeneral::String  sNamespace;	/**< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on) */
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLGeneral::String					m_sSourceCode;				/**< Script source code */
		PyObject						   *m_pPythonModule;			/**< Python module, can be a null pointer (borrowed reference, don't use Py_DECREF on it) */
		PyObject						   *m_pPythonDictionary;		/**< Python dictionary of the module, can be a null pointer (borrowed reference, don't use Py_DECREF on it) */
		PyObject						   *m_pPythonFunction;			/**< Current Python function, can be a null pointer (borrowed reference, don't use Py_DECREF on it) */
		PyObject						   *m_pPythonTuple;				/**< Python tuple, used during function call, can be a null pointer (own reference, use Py_DECREF on it) */
		PLGeneral::uint32					m_nCurrentArgument;			/**< Current argument, used during function call */
		PyObject						   *m_pPythonFunctionResult;	/**< Python function result, used during function call, can be a null pointer (own reference, use Py_DECREF on it) */
		PLGeneral::Array<DynamicFunction*>  m_lstDynamicFunctions;		/**< List of dynamic functions */
		PyMethodDef						   *m_pPythonTableOfFunctions;	/**< Python table of functions, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptPython


#endif // __PLSCRIPTPYTHON_SCRIPT_H__