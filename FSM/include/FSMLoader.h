#if !defined(_FSMLoader_H_)
#define _FSMLoader_H_

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

// $Log: FSMLoader.h,v $
// Revision 1.2  2002/08/20 14:48:41  vofka
// FiniteStateMachine-config.h removed from installing. Loading exported to the loader class. Saver introduced. Error codes exported to FSMErrorCodes.
//
// Revision 1.1  2002/06/28 11:10:36  vofka
// first time on sourceforge.net cvs
//
// Revision 1.2  2002/06/20 12:22:10  bobka
// LGPL license
//
// Revision 1.1  2002/05/03 23:21:56  bobka
// Loader externalisiert, aber der alte ist noch drin.
// Iterators for states und variables added.
// some minor bugs fixed.
//



#include <cstdio>
#include "FiniteStateMachine.h"

/////////////////////////////////////////////////////////////////////////
namespace FSM {

class CTokenizer;

/**
 * Base class for loader.
 */
class CFSMLoader
{
    public:
        CFSMLoader(CFiniteStateMachine *fsm);
        virtual ~CFSMLoader();

    protected:
        CFiniteStateMachine* m_FSM;
};

/**
 * XML loader.
 */
class CFSMSimpleLoaderXML : public CFSMLoader
{
    public:
        CFSMSimpleLoaderXML(CFiniteStateMachine *fsm);
        virtual ~CFSMSimpleLoaderXML();

        // Input/Output FSM
        bool Load(const char *filename);
        bool Load(FILE *inFile);

    protected:
        bool LoadFSM();
        bool LoadVariable();
        bool LoadState();
        bool LoadParam(CFSMObject* obj);
        bool LoadEnterAction(CFSMState* state);
        bool LoadTransition(CFSMState* state);
        bool LoadLeaveAction(CFSMState* state);
        bool LoadTransAction(CFSMTransition* trans);
        bool LoadCondition(CFSMTransition* trans);

    private:
        CTokenizer* m_Tokenizer;
};

/**
 * Loader that uses xerces library.
 * @todo not implemented yet
 */
class CFSMLoaderXML : public CFSMLoader
{
    public:
        CFSMLoaderXML(CFiniteStateMachine *fsm);
        virtual ~CFSMLoaderXML();

        // Input/Output FSM
        bool Load(const char *filename);

    protected:

    private:
};

/**
 * Base class for savers
 */
class CFSMSaver
{
    public:
        CFSMSaver(const CFiniteStateMachine *fsm);
        virtual ~CFSMSaver();

        /**
         * Service for objects that are about to be saved.
         * The given parameter should be saved by the saver.
         */
        virtual bool SaveParameter(const char* name, const char* value) const =0;

    protected:
        /**
         * Let the object call SaveParameter for each of its parameter.
         */
        bool SaveParameters(const CFSMObject* obj);

        const CFiniteStateMachine* m_FSM;
};

/**
 * Saver, that saves the FSM in XML format
 */
class CFSMSaverXML : public CFSMSaver
{
    public:
        CFSMSaverXML(const CFiniteStateMachine *fsm);
        virtual ~CFSMSaverXML();

        // Output FSM
        bool Save(const char *filename);
        bool Save(FILE *outFile);

        // overloaded
        virtual bool SaveParameter(const char* name, const char* value);

    protected:
        virtual bool SaveState(const CFSMState* state);
        virtual bool SaveTransition(const CFSMTransition* trans);
        virtual bool SaveAction(const CFSMAction *action, const char* tag="Action");
        virtual bool SaveCondition(const CFSMCondition *cond);

    private:
        const CFiniteStateMachine* m_FSM;
        FILE* m_File;
};

}; // namespace FSM

#endif // !defined(_FSMLoader_H_)
