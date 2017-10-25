// $Log: FSMLoader.cpp,v $
// Revision 1.2  2002/08/20 14:48:41  vofka
// FiniteStateMachine-config.h removed from installing. Loading exported to the loader class. Saver introduced. Error codes exported to FSMErrorCodes.
//
// Revision 1.1  2002/07/22 17:20:59  vofka
// SimpleTransition, PushTransition, PopTransition added, not implemented yet
//
// Revision 1.2  2002/06/20 12:22:10  bobka
// LGPL license
//
// Revision 1.1  2002/05/03 23:21:56  bobka
// Loader externalisiert, aber der alte ist noch drin.
// Iterators for states und variables added.
// some minor bugs fixed.
//

#ifdef _MSC_VER
    #pragma warning( disable : 4786 ) // 'identifier' : identifier was truncated to 'number' characters in the debug information
//    #pragma warning( disable : 4503 ) // 'identifier' : decorated name length exceeded, name was truncated
//    #pragma warning( disable : 4250 ) // 'class1' : inherits 'class2::member' via dominance
    #pragma warning( disable : 4251 ) // 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
#endif
#ifdef __INTEL_COMPILER
    #pragma warning( disable : 985  ) // == C4786
#endif

#include "FSM.h"
#include "FiniteStateMachine-config.h"
#include "FSMErrorCodes.h"
#include "Tokenizer.h"

#include <cstring>

#ifdef HAVE_XERCES
#    include <util/PlatformUtils.hpp>
#    include <sax2/DefaultHandler.hpp>
#    include <sax2/Attributes.hpp>
#    include <sax2/XMLReaderFactory.hpp>
#endif

#    include <cstdlib>
#    include <cstdio>
namespace FSM {

CFSMLoader::CFSMLoader(CFiniteStateMachine *fsm)
:m_FSM(fsm)
{
}

CFSMLoader::~CFSMLoader()
{
}

/////////////////////////////////////////////////////////////

CFSMSimpleLoaderXML::CFSMSimpleLoaderXML(CFiniteStateMachine *fsm)
:CFSMLoader(fsm)
,m_Tokenizer(NULL)
{
}

CFSMSimpleLoaderXML::~CFSMSimpleLoaderXML()
{
}

bool CFSMSimpleLoaderXML::Load(const char *filename)
{
    bool res = false;
    FILE *inFile = fopen(filename, "rb");
    if (inFile)
    {
        res = Load(inFile);
        fclose(inFile);
        if (!res) iRethrow();
    } else {
        iThrow(CRITICAL(FSM_OPEN_FILE_FAILED), filename);
    }
    return res;
}

bool CFSMSimpleLoaderXML::Load(FILE *inFile)
{
    bool res = false;

    if(!m_FSM->Clear())
    {
        iRethrow();
        return false;
    }

    m_Tokenizer = new CTokenizer(inFile);

    if (!m_Tokenizer->GetTag())
    {
        delete m_Tokenizer;
        m_Tokenizer=NULL;
        iRethrow();
        return false;
    }

    if(strcmp(m_Tokenizer->GetTagName(), "?xml")==0)
    {
        if (!m_Tokenizer->GetTag())
        {
            delete m_Tokenizer;
            m_Tokenizer=NULL;
            iRethrow();
            return false;
        }
    }

    if(strcmp(m_Tokenizer->GetTagName(), "Finite-State-Machine")!=0)
    {
        delete m_Tokenizer;
        m_Tokenizer=NULL;
        iThrow(CRITICAL(FSM_TAG_NOT_FOUND), "Finite-State-Machine");
        return false;
    }

    res = LoadFSM();
    if (!res)
    {
        m_FSM->Clear();
    }

    delete m_Tokenizer;
    m_Tokenizer=NULL;
    return res;
}

bool CFSMSimpleLoaderXML::LoadFSM()
{
    const int bufferLen=1024;
    char buffer[bufferLen];

    if(m_Tokenizer->GetAttribute("InitialState", buffer, bufferLen))
    {
        m_FSM->SetInitialState(buffer);
    }

    if (!m_Tokenizer->IsTagClosed())
    {
        do
        {
            if (!m_Tokenizer->GetTag())
            {
                iRethrow();
                return false;
            }

            // Variable
            if (strcmp(m_Tokenizer->GetTagName(),"Variable")==0)
            {
                if (!LoadVariable())
                {
                    iRethrow();
                    return false;
                }
            } else

            // State
            if (strcmp(m_Tokenizer->GetTagName(),"State")==0)
            {
                if (!LoadState())
                {
                    iRethrow();
                    return false;
                }
            } else

            // EndOfTag
            if (strcmp(m_Tokenizer->GetTagName(),"/Finite-State-Machine")==0)
            {
                break;
            } else
            {
                iThrow(CRITICAL(FSM_UNKNOWN_TAG), m_Tokenizer->GetTagName(), "Finite-State-Machine");
                return false;
            }
        } while(true);
    }

    return true;
}

bool CFSMSimpleLoaderXML::LoadVariable()
{
    string name;
    const int bufferLen=1024;
    char buffer[bufferLen];
    int value = 0;

    if (!m_Tokenizer->GetAttribute("Name", buffer, bufferLen))
    {
        iThrow(CRITICAL(FSM_MISSED_ATTRIBUTE), "Name", "Variable");
        return false;
    }
    name=buffer;
    value=0;
    if(m_Tokenizer->GetAttribute("Value", buffer, bufferLen))
    {
        value = atoi(buffer);
    }

    if(!m_FSM->AddVariable(name.c_str(), value))
    {
        iRethrow();
        return false;
    }

    if (!m_Tokenizer->IsTagClosed())
    {
        if (!m_Tokenizer->GetTag())
        {
            iRethrow();
            return false;
        }

        if (strcmp(m_Tokenizer->GetTagName(),"/Variable")!=0)
        {
            iThrow(CRITICAL(FSM_TAG_NOT_FOUND), "/Variable");
            return false;
        }
    }

    return true;
}

bool CFSMSimpleLoaderXML::LoadState()
{
    CFSMState* state=NULL;
    string name;
    const int bufferLen=1024;
    char buffer[bufferLen];

    if (!m_Tokenizer->GetAttribute("Name", buffer, 1024))
    {
        iThrow(CRITICAL(FSM_MISSED_ATTRIBUTE), "Name", "State");
        return false;
    }
    name = buffer;

    if (m_Tokenizer->GetAttribute("Class", buffer, 1024))
    {
        state = m_FSM->AddState(name.c_str(), buffer);
    } else
    {
        state = m_FSM->AddState(name.c_str());
    }

    if (!state)
    {
        iRethrow();
        return false;
    }

    if (!m_Tokenizer->IsTagClosed())
    {
        do
        {
            if (!m_Tokenizer->GetTag())
            {
                iRethrow();
                return false;
            }

            // EnterAction
            if (strcmp(m_Tokenizer->GetTagName(),"EnterAction")==0)
            {
                if (!LoadEnterAction(state))
                {
                    iRethrow();
                    return false;
                }
            } else

            // Transition
            if (strcmp(m_Tokenizer->GetTagName(),"Transition")==0)
            {
                if (!LoadTransition(state))
                {
                    iRethrow();
                    return false;
                }
            } else

            // LeaveAction
            if (strcmp(m_Tokenizer->GetTagName(),"LeaveAction")==0)
            {
                if (!LoadLeaveAction(state))
                {
                    iRethrow();
                    return false;
                }
            } else

            // Param
            if (strcmp(m_Tokenizer->GetTagName(),"Param")==0)
            {
                if (!LoadParam(state))
                {
                    iRethrow();
                    return false;
                }
            } else

            // EndOfTag
            if (strcmp(m_Tokenizer->GetTagName(),"/State")==0)
            {
                break;
            } else

            {
                iThrow(CRITICAL(FSM_UNKNOWN_TAG), m_Tokenizer->GetTagName(), "State");
                return false;
            }

        } while(true);
    }

    return true;
}

bool CFSMSimpleLoaderXML::LoadParam(CFSMObject* obj)
{
    string name;
    const int bufferLen=1024;
    char buffer[bufferLen];

    if (!m_Tokenizer->GetAttribute("Name", buffer, bufferLen))
    {
        iThrow(CRITICAL(FSM_MISSED_ATTRIBUTE), "Name", "Param");
        return false;
    }
    name = buffer;

    if (!m_Tokenizer->GetAttribute("Value", buffer, bufferLen))
    {
        iThrow(CRITICAL(FSM_MISSED_ATTRIBUTE), "Value", "Param");
        return false;
    }

    if (!obj->SetParameter(name.c_str(), buffer))
    {
        iRethrow();
        return false;
    }

    if (!m_Tokenizer->IsTagClosed())
    {
        if (!m_Tokenizer->GetTag())
        {
            iRethrow();
            return false;
        }

        if (strcmp(m_Tokenizer->GetTagName(),"/Param")!=0)
        {
            iThrow(CRITICAL(FSM_TAG_NOT_FOUND), "/Param");
            return false;
        }
    }

    return true;
}

bool CFSMSimpleLoaderXML::LoadEnterAction(CFSMState* state)
{
    CFSMAction* action;
    const int bufferLen=1024;
    char buffer[bufferLen];
    string name;

    if (!m_Tokenizer->GetAttribute("Class", buffer, bufferLen))
    {
        iThrow(CRITICAL(FSM_MISSED_ATTRIBUTE), "Class", "Action");
    }
    action = state->AddEnterAction(buffer);

    if (!action)
    {
        iRethrow();
        return false;
    }

    if (!m_Tokenizer->IsTagClosed())
    {
        do
        {
            if (!m_Tokenizer->GetTag())
            {
                iRethrow();
                return false;
            }

            // Param
            if (strcmp(m_Tokenizer->GetTagName(),"Param")==0)
            {
                if (!LoadParam(action))
                {
                    iRethrow();
                    return false;
                }
            } else

            // EndOfTag
            if (strcmp(m_Tokenizer->GetTagName(),"/EnterAction")==0)
            {
                break;
            } else

            {
                iThrow(CRITICAL(FSM_UNKNOWN_TAG), m_Tokenizer->GetTagName(), "EnterAction");
                return false;
            }
        } while(true);
    }

    return true;
}

bool CFSMSimpleLoaderXML::LoadLeaveAction(CFSMState* state)
{
    CFSMAction* action;
    const int bufferLen=1024;
    char buffer[bufferLen];
    string name;

    if (!m_Tokenizer->GetAttribute("Class", buffer, bufferLen))
    {
        iThrow(CRITICAL(FSM_MISSED_ATTRIBUTE), "Class", "Action");
    }
    action = state->AddLeaveAction(buffer);

    if (!action)
    {
        iRethrow();
        return false;
    }

    if (!m_Tokenizer->IsTagClosed())
    {
        do
        {
            if (!m_Tokenizer->GetTag())
            {
                iRethrow();
                return false;
            }

            // Param
            if (strcmp(m_Tokenizer->GetTagName(),"Param")==0)
            {
                if (!LoadParam(action))
                {
                    iRethrow();
                    return false;
                }
            } else

            // EndOfTag
            if (strcmp(m_Tokenizer->GetTagName(),"/LeaveAction")==0)
            {
                break;
            } else

            {
                iThrow(CRITICAL(FSM_UNKNOWN_TAG), m_Tokenizer->GetTagName(), "LeaveAction");
                return false;
            }
        } while(true);
    }

    return true;
}

bool CFSMSimpleLoaderXML::LoadTransAction(CFSMTransition* trans)
{
    CFSMAction* action;
    const int bufferLen=1024;
    char buffer[bufferLen];
    string name;

    if (!m_Tokenizer->GetAttribute("Class", buffer, bufferLen))
    {
        iThrow(CRITICAL(FSM_MISSED_ATTRIBUTE), "Class", "Action");
    }
    action = trans->AddAction(buffer);

    if (!action)
    {
        iRethrow();
        return false;
    }

    if (!m_Tokenizer->IsTagClosed())
    {
        do
        {
            if (!m_Tokenizer->GetTag())
            {
                iRethrow();
                return false;
            }

            // Param
            if (strcmp(m_Tokenizer->GetTagName(),"Param")==0)
            {
                if (!LoadParam(action))
                {
                    iRethrow();
                    return false;
                }
            } else

            // EndOfTag
            if (strcmp(m_Tokenizer->GetTagName(),"/Action")==0)
            {
                break;
            } else

            {
                iThrow(CRITICAL(FSM_UNKNOWN_TAG), m_Tokenizer->GetTagName(), "Action");
                return false;
            }
        } while(true);
    }

    return true;
}

bool CFSMSimpleLoaderXML::LoadTransition(CFSMState* state)
{
    CFSMTransition* trans;
    const int bufferLen=1024;
    char buffer[bufferLen];
    string className = "SimpleTransition";

    if (m_Tokenizer->GetAttribute("Class", buffer, 1024))
    {
        className=buffer;
    }

    if (m_Tokenizer->GetAttribute("OnEvent", buffer, 1024))
    {
        trans = state->AddTransition(buffer, className.c_str());
    } else
    {
        trans = state->AddTransition(NULL, className.c_str());
    }

    if (!trans)
    {
        iRethrow();
        return false;
    }

    // down compatibility
    if (m_Tokenizer->GetAttribute("ToState", buffer, 1024))
    {
        if (!trans->SetParameter("ToState", buffer))
        {
            iRethrow();
            return false;
        }
    }

    if (!m_Tokenizer->IsTagClosed())
    {
        do
        {
            if (!m_Tokenizer->GetTag())
            {
                iRethrow();
                return false;
            }

            // Condition
            if (strcmp(m_Tokenizer->GetTagName(),"Condition")==0)
            {
                if (!LoadCondition(trans))
                {
                    iRethrow();
                    return false;
                }
            } else

            // Action
            if (strcmp(m_Tokenizer->GetTagName(),"Action")==0)
            {
                if (!LoadTransAction(trans))
                {
                    iRethrow();
                    return false;
                }
            } else

            // Param
            if (strcmp(m_Tokenizer->GetTagName(),"Param")==0)
            {
                if (!LoadParam(trans))
                {
                    iRethrow();
                    return false;
                }
            } else

            // EndOfTag
            if (strcmp(m_Tokenizer->GetTagName(),"/Transition")==0)
            {
                break;
            } else

            {
                iThrow(CRITICAL(FSM_UNKNOWN_TAG), m_Tokenizer->GetTagName(), "Transition");
                return false;
            }

        } while(true);
    }

    return true;
}

bool CFSMSimpleLoaderXML::LoadCondition(CFSMTransition* trans)
{
    CFSMCondition* condition;
    const int bufferLen=1024;
    char buffer[bufferLen];
    string name;

    if (m_Tokenizer->GetAttribute("Class", buffer, bufferLen))
    {
        condition = trans->AddCondition(buffer);
    } else
    {
        condition = trans->AddCondition();
    }

    if (!condition)
    {
        iRethrow();
        return false;
    }

    if (!m_Tokenizer->IsTagClosed())
    {
        do
        {
            if (!m_Tokenizer->GetTag())
            {
                iRethrow();
                return false;
            }

            // Param
            if (strcmp(m_Tokenizer->GetTagName(),"Param")==0)
            {
                if (!LoadParam(condition))
                {
                    iRethrow();
                    return false;
                }
            } else

            // EndOfTag
            if (strcmp(m_Tokenizer->GetTagName(),"/Condition")==0)
            {
                break;
            } else

            {
                iThrow(CRITICAL(FSM_UNKNOWN_TAG), m_Tokenizer->GetTagName(), "Condition");
                return false;
            }
        } while(true);
    }

    return true;
}


//////////////////////////////////////////////////////////
#ifdef HAVE_XERCES

class SAX2LoaderHandler : public DefaultHandler
{
public:
    SAX2LoaderHandler(CFiniteStateMachine *fsm);
    ~SAX2LoaderHandler();


    void startElement(    const   XMLCh* const    uri,
                        const   XMLCh* const    localname,
                        const   XMLCh* const    qname,
                        const   Attributes&        attributes);


    void endElement( const XMLCh* const uri,
                     const XMLCh* const localname,
                     const XMLCh* const qname);
private:
    CFiniteStateMachine* m_FSM;
};


//////////////////////////////////////////////////////
SAX2LoaderHandler::SAX2LoaderHandler(CFiniteStateMachine *fsm)
:m_FSM(fsm)
{
}

SAX2LoaderHandler::~SAX2LoaderHandler()
{
}

void SAX2LoaderHandler::startElement(const   XMLCh* const    uri,
                                    const   XMLCh* const    localname,
                                    const   XMLCh* const    qname,
                                    const   Attributes&        attributes)
{
//    cerr << "startElement" << endl;
//    cerr << (const char*)uri << ":" << (const char*)localname << endl;
    cerr << "<" << XMLString::transcode(qname);

    unsigned int len = attributes.getLength();
    for (unsigned int index = 0; index < len; index++)
    {
//        cerr << (const char*)attributes.getURI(index) << ":" << (const char*)attributes.getLocalName(index) << endl;
        cerr << " " << XMLString::transcode(attributes.getQName(index)) << "=\"";
        cerr << XMLString::transcode(attributes.getValue(index)) << "\"";
    }
    cerr << ">" << endl;
}

void SAX2LoaderHandler::endElement(const XMLCh* const uri,
                                 const XMLCh* const localname,
                                 const XMLCh* const qname)
{
//    cerr << (const char*)uri << ":" << (const char*)localname << endl;
    cerr << "</" << XMLString::transcode(qname) << ">" << endl;
}

#endif

//////////////////////////////////////////////////////
CFSMLoaderXML::CFSMLoaderXML(CFiniteStateMachine *fsm)
:CFSMLoader(fsm)
{
}

CFSMLoaderXML::~CFSMLoaderXML()
{
}

bool CFSMLoaderXML::Load(const char *filename)
{
    bool res = false;

#ifdef HAVE_XERCES

    if(!m_FSM->Clear())
    {
        iRethrow();
        return false;
    }

    try
    {
         XMLPlatformUtils::Initialize();
    }

    catch (const XMLException& toCatch)
    {
         cerr << "Error during initialization! :\n" << toCatch.getMessage() << endl;
         return false;
    }

    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();

    int errorCount = 0;
    try
    {
        SAX2LoaderHandler handler(m_FSM);
        parser->setContentHandler(&handler);
        parser->setErrorHandler(&handler);
        parser->parse(filename);
        errorCount = parser->getErrorCount();
        if (errorCount==0) res = true;
    } catch (const XMLException& toCatch)
    {
        cerr << "An error occured" << endl
             << "  Error: " << toCatch.getMessage() << endl;
        XMLPlatformUtils::Terminate();
        return false;
    }

    //
    //  Delete the parser itself.  Must be done prior to calling Terminate, below.
    //
    delete parser;

    // And call the termination method
    XMLPlatformUtils::Terminate();

#else
    iThrow(CRITICAL(FSM_LOAD_NOT_IMPLEMENTED));
#endif

    return res;
}

///////////////////////////////////////////////////////////////////////////
CFSMSaver::CFSMSaver(const CFiniteStateMachine *fsm)
:m_FSM(fsm)
{
}

CFSMSaver::~CFSMSaver()
{
}

bool CFSMSaver::SaveParameters(const CFSMObject* obj)
{
    bool res = obj->SaveParameters(this);
    if (!res) {iRethrow();}
    return true;
}

/////////////////////////////////////////////////////////////
CFSMSaverXML::CFSMSaverXML(const CFiniteStateMachine *fsm)
:CFSMSaver(fsm)
,m_File(NULL)
{
}

CFSMSaverXML::~CFSMSaverXML()
{
}

bool CFSMSaverXML::Save(const char *filename)
{
    bool res = false;
    FILE *outFile = fopen(filename, "wb");
    if(outFile)
    {
        fprintf(outFile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        if (Save(outFile))
        {
            res = true;
        } else
        {
            iRethrow();
        }
        fflush(outFile);
        fclose(outFile);

    } else
    {
        iThrow(CRITICAL(FSM_OPEN_FILE_FAILED), filename);
    }

    return res;
}


bool CFSMSaverXML::Save(FILE *outFile)
{
    m_File=outFile;
    fprintf(m_File, "<Finite-State-Machine Version=\"0.3\" ");
    fprintf(m_File, "InitialState=\"%s\">\n", m_FSM->GetInitialState());

    for(Var_Map::const_iterator j = m_FSM->GetVarsBegin(); j != m_FSM->GetVarsEnd() ; ++j)
    {
        fprintf(m_File, "\t<Variable Name=\"%s", (*j).first.c_str());
        fprintf(m_File, "\" Value=\"%d\"> ", (*j).second);
        fprintf(m_File, "</Variable>\n");
    }

    for(State_Map::const_iterator i = m_FSM->GetStatesBegin() ; i != m_FSM->GetStatesEnd() ; ++i)
    {
        if(!SaveState((*i).second))
        {
            iRethrow();
            return false;
        }
    }
    fprintf(m_File, "</Finite-State-Machine>\n");
    m_File=NULL;
    return true;
}

bool CFSMSaverXML::SaveState(const CFSMState* state)
{
    fprintf(m_File, "\t<State Name=\"%s\"", state->GetName());
    if (strcmp(state->GetNameOfClass(), "State")!=0)
    {
        fprintf(m_File, " Class=\"%s\"", state->GetNameOfClass());
    }
    fprintf(m_File, ">\n");
    if (!SaveParameters(state)) {iRethrow(); return false;}
    Action_List::const_iterator i;
    for(i = state->GetEnterActionsBegin() ; i != state->GetEnterActionsEnd() ; ++i)
    {
        SaveAction((*i), "EnterAction");
    }
    for(Transition_List::const_iterator j = state->GetTransitionsBegin() ; j != state->GetTransitionsEnd() ; ++j)
    {
        SaveTransition((*j));
    }
    for(i = state->GetLeaveActionsBegin() ; i != state->GetLeaveActionsEnd() ; ++i)
    {
        SaveAction((*i), "LeaveAction");
    }
    fprintf(m_File, "\t</State>\n");
    return true;
}

bool CFSMSaverXML::SaveTransition(const CFSMTransition* trans)
{
    fprintf(m_File, "\t\t<Transition");
    if (strcmp(trans->GetOnEvent(), "ANY")!=0)
    {
        fprintf(m_File, " OnEvent=\"%s\"", trans->GetOnEvent());
    }
    if (strcmp(trans->GetNameOfClass(), "Transition")!=0)
    {
        fprintf(m_File, " Class=\"%s\"", trans->GetNameOfClass());
    }
    fprintf(m_File, ">\n");
    if (!SaveParameters(trans)) {iRethrow(); return false;}
    for(Condition_List::const_iterator i = trans->GetConditionsBegin() ; i != trans->GetConditionsEnd() ; ++i)
    {
        if (!SaveCondition((*i)))
        {
            iRethrow();
            return false;
        }
    }
    for(Action_List::const_iterator j = trans->GetActionsBegin() ; j != trans->GetActionsEnd() ; ++j)
    {
        if (!SaveAction((*j), "Action"))
        {
            iRethrow();
            return false;
        }
    }
    fprintf(m_File, "\t\t</Transition>\n");
    return true;
}

bool CFSMSaverXML::SaveAction(const CFSMAction* action, const char* tag)
{
    fprintf(m_File, "\t\t\t<%s Class=\"%s\">\n", tag, action->GetNameOfClass());
    if (!SaveParameters(action)) {iRethrow(); return false;}
    fprintf(m_File, "\t\t\t</%s>\n", tag);
    return true;
}

bool CFSMSaverXML::SaveCondition(const CFSMCondition* cond)
{
    fprintf(m_File, "\t\t\t<Condition");
    if (strcmp(cond->GetNameOfClass(), "TestVariable")!=0)
    {
        fprintf(m_File, " Class=\"%s\"", cond->GetNameOfClass());
    }
    fprintf(m_File, ">\n");
    if (!SaveParameters(cond)) {iRethrow(); return false;}
    fprintf(m_File, "\t\t\t</Condition>\n");
    return true;
}

bool CFSMSaverXML::SaveParameter(const char* name, const char* value)
{
    fprintf(m_File, "\t\t\t\t<Param Name=\"%s\" Value=\"%s\"> </Param>\n", name, value);
    return true;
}

/////////////////////////////////////////////////
} // namespace FSM
