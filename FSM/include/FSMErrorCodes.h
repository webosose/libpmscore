#ifndef _FSMErrorCodes_h_
#define _FSMErrorCodes_h_

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

// $Log: FSMErrorCodes.h,v $
// Revision 1.1  2002/08/20 14:48:41  vofka
// FiniteStateMachine-config.h removed from installing. Loading exported to the loader class. Saver introduced. Error codes exported to FSMErrorCodes.
//

#ifdef HAVE_LIBIERROR
    #include <iError.h>
    namespace FSM {
        using iERR::iError;
        using iERR::iErrorCode;
        using iERR::GENERIC;
    };
#else
    #define INFO(err) 1
    #define WARNING(err) 2
    #define CRITICAL(err) 3
    #define EXCEPTION(err) 4
    #define iHandleLastError()
    #define iRethrow()
    class iErrorCode {public: iErrorCode(const char *) {};};
    inline void iThrow(int, ...) {};
#endif


namespace FSM {
    extern iErrorCode FSM_UNKNOWN_CONDITION_VALUE;
    extern iErrorCode FSM_OPEN_FILE_FAILED;
    extern iErrorCode FSM_LOAD_STARTED;
    extern iErrorCode FSM_LOAD_NOT_IMPLEMENTED;
    extern iErrorCode FSM_TRANSITION_NOT_FOUND;
    extern iErrorCode FSM_ACTION_NOT_FOUND;
    extern iErrorCode FSM_CONDITION_NOT_FOUND;
    extern iErrorCode FSM_STATE_DEFINED_TWICE;
    extern iErrorCode FSM_STATE_NOT_FOUND;
    extern iErrorCode FSM_CANNOT_DELETE_CURRENT;
    extern iErrorCode FSM_VARAIBLE_DEFINED_TWICE;
    extern iErrorCode FSM_VARIABLE_NOT_FOUND;
    extern iErrorCode FSM_INITIAL_AFTER_START;
    extern iErrorCode FSM_ALREADY_STARTED;
    extern iErrorCode FSM_NOT_STARTED;
    extern iErrorCode FSM_CLEAR_STARTED;
    extern iErrorCode FSM_TAG_NOT_FOUND;
    extern iErrorCode FSM_MISSED_ATTRIBUTE;
    extern iErrorCode FSM_UNKNOWN_TAG;
    extern iErrorCode FSM_UNKNOWN_PARAM;
    extern iErrorCode FSM_UNKNOWN_CONDITION_VALUE;
    extern iErrorCode FSM_UNKNOWN_CONDITION;
    extern iErrorCode FSM_PARAM_NOT_SET;
    extern iErrorCode FSM_CLASS_REGISTERED_TWICE;
    extern iErrorCode FSM_CANNOT_CREATE_CLASS;
    extern iErrorCode FSM_PARAM_NOT_READ;
}; // namespace FSM

#endif // ifndef _FSMErrorCodes_h_
