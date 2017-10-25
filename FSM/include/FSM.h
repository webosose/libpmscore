#if !defined(_FSM_H_)
#define _FSM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
 * The Finite State Machine (FSM) Library
 * Copyright (C) 2002 Siemens C-LAB Paderborn
 * All rights reserved.
 *
 * This program is  free  software;  you can redistribute it and/or modify it
 * under the terms of the  GNU Lesser General Public License  as published by
 * the  Free Software Foundation;  either version 2.1 of the License,  or (at
 * your option) any later version.
 *
 * This  program  is  distributed in  the  hope that it will  be useful,  but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or  FITNESS FOR A  PARTICULAR PURPOSE.  See the  GNU Lesser General Public
 * License for more details.
 *
 * You should  have received  a copy of the GNU Lesser General Public License
 * along with  this program;  if not, write to the  Free Software Foundation,
 * Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

// $Log: FSM.h,v $
// Revision 1.2  2002/08/20 14:48:41  vofka
// FiniteStateMachine-config.h removed from installing. Loading exported to the loader class. Saver introduced. Error codes exported to FSMErrorCodes.
//
// Revision 1.1  2002/06/28 11:10:36  vofka
// first time on sourceforge.net cvs
//
// Revision 1.4  2002/06/20 12:22:10  bobka
// LGPL license
//
// Revision 1.3  2002/05/03 23:21:56  bobka
// Loader externalisiert, aber der alte ist noch drin.
// Iterators for states und variables added.
// some minor bugs fixed.
//
// Revision 1.2  2001/11/23 09:27:57  bobka
// Load/Save funktioniert.
// FSM* im Fileformat entfernt.
// streams entfernt.
// Laeuft unter Win32 und unter WinCE (WinCE_STL Modul auschecken)
// static library unter WinCE funktioniert nicht, nur DLL.
//
// Revision 1.1  2001/11/15 10:37:31  bobka
// initial checkin
// not tested on linux!
//

#include "FiniteStateMachine.h"
#include "FSMAction.h"
#include "FSMCondition.h"
#include "FSMObject.h"
#include "FSMState.h"
#include "FSMTransition.h"
#include "FSMLoader.h"

#ifdef WIN32
    #ifdef FiniteStateMachine_EXPORTS
        #define FiniteStateMachine_API __declspec(dllexport)
    #else
        #define FiniteStateMachine_API __declspec(dllimport)
        #ifdef _DEBUG
            #pragma comment(lib, "FiniteStateMachine_d.lib")
        #else
            #pragma comment(lib, "FiniteStateMachine.lib")
        #endif
    #endif
    namespace FSM {
        class FiniteStateMachine_API CFSMObject;
        class FiniteStateMachine_API CFiniteStateMachine;
        class FiniteStateMachine_API CFSMAction;
        class FiniteStateMachine_API CFSMActionSetVariable;
        class FiniteStateMachine_API CFSMActionIncrVariable;
        class FiniteStateMachine_API CFSMCondition;
        class FiniteStateMachine_API CFSMConditionTestVariable;
        class FiniteStateMachine_API CFSMLoader;
        class FiniteStateMachine_API SAX2LoaderHandler;
        class FiniteStateMachine_API CFSMLoaderXML;
        class FiniteStateMachine_API CFSMSaverXML;
        class FiniteStateMachine_API CFSMState;
        class FiniteStateMachine_API CFSMTransition;
        class FiniteStateMachine_API CFSMSimpleTransition;
        class FiniteStateMachine_API CFSMPushTransition;
        class FiniteStateMachine_API CFSMPopTransition;
    } // namespace FSM
#endif

#endif // !defined(_FSM_H_)
