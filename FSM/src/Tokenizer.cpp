// $Log: Tokenizer.cpp,v $
// Revision 1.3  2002/08/20 14:48:41  vofka
// FiniteStateMachine-config.h removed from installing. Loading exported to the loader class. Saver introduced. Error codes exported to FSMErrorCodes.
//
// Revision 1.2  2002/08/13 11:06:32  vofka
// automake support added
//
// Revision 1.1  2002/07/22 17:21:00  vofka
// SimpleTransition, PushTransition, PopTransition added, not implemented yet
//
// Revision 1.9  2002/06/20 12:22:11  bobka
// LGPL license
//
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
#include <cstdlib>
#include <utility>
namespace FSM {

    iErrorCode TOK_WRONG_EVENT("Wrong event \"%s\".");
    iErrorCode TOK_INSERT_ATTR_FAILED("Could not insert attribute %s=\"%s\".");
    iErrorCode TOK_UNEXPECTED_EOF("Unexpected end of file.");

//////////////////////////////////////////////////////////////////////
// Tokenizer state machine
//////////////////////////////////////////////////////////////////////
class CTOKConditionBlank :  public CFSMCondition
{
    public:
        friend class CFSMTransition;
        friend class CFiniteStateMachine;

        // Construction/Destruction
        static CFSMObject *Create() {return new CTOKConditionBlank;};
        virtual void DeleteThis() {delete this;};

        virtual bool CheckCondition(const char* event, void *a, void *b)
        {
            bool res = false;
            if (strcmp(event, "CHAR")==0)
            {
                char ch = *(char*)a;
                switch (ch)
                {
                case ' ':
                case '\n':
                case '\t':
                case '=':
                    res = true;
                    break;
                }
            }
            return res;
        }

    protected:

    private:
        CTOKConditionBlank() {};
        virtual ~CTOKConditionBlank() {};
};

class CTOKConditionGotChar :  public CFSMCondition
{
    public:
        friend class CFSMTransition;
        friend class CFiniteStateMachine;

        // Construction/Destruction
        static CFSMObject *Create() {return new CTOKConditionGotChar;};
        virtual void DeleteThis() {delete this;};

        // Get/Set
        void SetChar(char ch) {m_Value = ch;};

        virtual bool CheckCondition(const char* event, void *a, void *b)
        {
            bool res = false;
            if (strcmp(event, "CHAR")==0)
            {
                char ch = *(char*)a;
                res= (ch==m_Value);
            }
            return res;
        };

    protected:

    private:
        CTOKConditionGotChar() : m_Value('\0') {};
        virtual ~CTOKConditionGotChar() {};

        char m_Value;
};

class CTOKActionCopyToBuffer :  public CFSMAction
{
    public:
        friend class CFSMState;
        friend class CFSMTransition;
        friend class CFiniteStateMachine;

        // Construction/Destruction
        static CFSMObject *Create() {return new CTOKActionCopyToBuffer;};
        virtual void DeleteThis() {delete this;};

        // Get/Set
        void SetBuffer(char** buffer) {buffer_p = buffer;};

        // Overloaded
        virtual bool Execute(const char* event, void *a, void *b)
        {
            bool res = false;
            if (strcmp(event, "CHAR")==0)
            {
                char ch = *(char*)a;
                *(*buffer_p) = ch;
                ++(*buffer_p);
                *(*buffer_p)='\0';
                res = true;
            } else {
                iThrow(CRITICAL(TOK_WRONG_EVENT), event);
            }
            return res;

        }

    protected:

    private:
        CTOKActionCopyToBuffer() :buffer_p(NULL) {};
        virtual ~CTOKActionCopyToBuffer() {};

        char** buffer_p;
};

class CTOKActionSetTag :  public CFSMAction
{
    public:
        friend class CFSMState;
        friend class CFSMTransition;
        friend class CFiniteStateMachine;

        // Construction/Destruction
        static CFSMObject *Create() {return new CTOKActionSetTag;};
        virtual void DeleteThis() {delete this;};

        // Get/Set
        void SetBuffer(char** b) {buffer_p = b; buffer = *buffer_p;};
        void SetTokenizer(CTokenizer* tok) {tokenizer = tok;};

        // Overloaded
        virtual bool Execute(const char* event, void *a, void *b)
        {
            tokenizer->tag = buffer;
            *buffer_p = buffer;
            buffer[0]='\0';
            return true;
        }

    protected:

    private:
        CTOKActionSetTag() :buffer(NULL), buffer_p(NULL), tokenizer(NULL) {};
        virtual ~CTOKActionSetTag() {};

        char* buffer;
        char** buffer_p;
        CTokenizer *tokenizer;
};

class CTOKActionSetAttribute :  public CFSMAction
{
    public:
        friend class CFSMState;
        friend class CFSMTransition;
        friend class CFiniteStateMachine;

        // Construction/Destruction
        static CFSMObject *Create() {return new CTOKActionSetAttribute;};
        virtual void DeleteThis() {delete this;};

        // Get/Set
        void SetNameBuffer(char** n) {name_p = n; name = *name_p;};
        void SetValueBuffer(char** v) {value_p = v; value = *value_p;};
        void SetTokenizer(CTokenizer* tok) {tokenizer = tok;};

        // Overloaded
        virtual bool Execute(const char* event, void *a, void *b)
        {
            TAM_VT v(name, value);
            const std::pair<Attrib_Map::iterator,bool>result = tokenizer->Attributes.insert(v);
            if(!result.second)
            {
                iThrow(CRITICAL(TOK_INSERT_ATTR_FAILED), name, value);
                return false;
            }
            name[0]='\0';
            value[0]='\0';
            *name_p = name;
            *value_p = value;
            return true;
        }

    protected:

    private:
        CTOKActionSetAttribute()
            :name(NULL)
            ,value(NULL)
            ,name_p(NULL)
            ,value_p(NULL)
            ,tokenizer(NULL)
        {};
        virtual ~CTOKActionSetAttribute() {};

        char* name;
        char* value;
        char** name_p;
        char** value_p;
        CTokenizer *tokenizer;
};

class CTOKActionSetTrue :  public CFSMAction
{
    public:
        friend class CFSMState;
        friend class CFSMTransition;
        friend class CFiniteStateMachine;

        // Construction/Destruction
        static CFSMObject *Create() {return new CTOKActionSetTrue;};
        virtual void DeleteThis() {delete this;};

        // Get/Set
        void SetVariable(bool *tagClosed) {value = tagClosed;};

        // Overloaded
        virtual bool Execute(const char* event, void *a, void *b)
        {
            *value = true;
            return true;
        }

    protected:

    private:
        CTOKActionSetTrue()
            :value(NULL)
        {};
        virtual ~CTOKActionSetTrue() {};

        bool* value;
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTokenizer::CTokenizer(FILE* f)
:inFile(f)
{
    name_p = name;
    value_p = value;
    tagname_p = tagname;

    CFSMTransition *trans;
    CTOKConditionGotChar *condGotChar;
    CTOKActionCopyToBuffer *actionCopyToBuffer;
    CTOKActionSetTag *actionSetTag;
    CTOKActionSetAttribute *actionSetAttribute;
    CTOKActionSetTrue *actionSetTrue;

    fsm = new CFiniteStateMachine;

    fsm->Register("Blank", CTOKConditionBlank::Create);
    fsm->Register("GotChar", CTOKConditionGotChar::Create);
    fsm->Register("CopyToBuffer", CTOKActionCopyToBuffer::Create);
    fsm->Register("SetTag", CTOKActionSetTag::Create);
    fsm->Register("SetAttribute", CTOKActionSetAttribute::Create);
    fsm->Register("SetTrue", CTOKActionSetTrue::Create);


    // the start state
    state = fsm->AddState("SEEK_TAG");
        // '<' -> CHECK_COMMENT
        trans = state->AddSimpleTransition("CHECK_COMMENT", "CHAR");
            condGotChar = (CTOKConditionGotChar*)trans->AddCondition("GotChar");
            condGotChar->SetChar('<');

    // '<' read, check for "<!"
    state = fsm->AddState("CHECK_COMMENT");
        // '!' -> SKIPPING_COMMENT
        trans = state->AddSimpleTransition("SKIPPING_COMMENT", "CHAR");
            condGotChar = (CTOKConditionGotChar*)trans->AddCondition("GotChar");
            condGotChar->SetChar('!');
        // else -> READING_TAG
        trans = state->AddSimpleTransition("READING_TAG", "CHAR");
            actionCopyToBuffer = (CTOKActionCopyToBuffer*)trans->AddAction("CopyToBuffer");
            actionCopyToBuffer->SetBuffer(&tagname_p);

    // copy to buffer "tagname"
    state = fsm->AddState("READING_TAG");
        // '<' -> FINISHED
        trans = state->AddSimpleTransition("FINISHED", "CHAR");
            condGotChar = (CTOKConditionGotChar*)trans->AddCondition("GotChar");
            condGotChar->SetChar('>');
            actionSetTag = (CTOKActionSetTag*)trans->AddAction("SetTag");
            actionSetTag->SetTokenizer(this);
            actionSetTag->SetBuffer(&tagname_p);
        // ' ' -> SEEK_ATTR
        trans = state->AddSimpleTransition("SEEK_ATTR", "CHAR");
            trans->AddCondition("Blank");
            actionSetTag = (CTOKActionSetTag*)trans->AddAction("SetTag");
            actionSetTag->SetTokenizer(this);
            actionSetTag->SetBuffer(&tagname_p);
        // else continue
        trans = state->AddSimpleTransition("READING_TAG", "CHAR");
            actionCopyToBuffer = (CTOKActionCopyToBuffer*)trans->AddAction("CopyToBuffer");
            actionCopyToBuffer->SetBuffer(&tagname_p);

    // skip blanks
    state = fsm->AddState("SEEK_ATTR");
        // '>' -> FINISHED
        trans = state->AddSimpleTransition("FINISHED", "CHAR");
            condGotChar = (CTOKConditionGotChar*)trans->AddCondition("GotChar");
            condGotChar->SetChar('>');
        // '/' -> CHECK_END_TAG
        trans = state->AddSimpleTransition("CHECK_END_TAG", "CHAR");
            condGotChar = (CTOKConditionGotChar*)trans->AddCondition("GotChar");
            condGotChar->SetChar('/');
        // ' ' continue
        trans = state->AddSimpleTransition("SEEK_ATTR", "CHAR");
            trans->AddCondition("Blank");
        // else -> READING_ATTR
        trans = state->AddSimpleTransition("READING_ATTR", "CHAR");
            actionCopyToBuffer = (CTOKActionCopyToBuffer*)trans->AddAction("CopyToBuffer");
            actionCopyToBuffer->SetBuffer(&name_p);

    // copy to buffer "name"
    state = fsm->AddState("READING_ATTR");
        // '>' -> FINISHED
        trans = state->AddSimpleTransition("FINISHED", "CHAR");
            condGotChar = (CTOKConditionGotChar*)trans->AddCondition("GotChar");
            condGotChar->SetChar('>');
        // ' ' -> SEEK_VALUE
        trans = state->AddSimpleTransition("SEEK_VALUE", "CHAR");
            trans->AddCondition("Blank");
        // else continue
        trans = state->AddSimpleTransition("READING_ATTR", "CHAR");
            actionCopyToBuffer = (CTOKActionCopyToBuffer*)trans->AddAction("CopyToBuffer");
            actionCopyToBuffer->SetBuffer(&name_p);

    // skip blanks, seek for \" ('=' is a blank)
    state = fsm->AddState("SEEK_VALUE");
        // '>' -> FINISHED
        trans = state->AddSimpleTransition("FINISHED", "CHAR");
            condGotChar = (CTOKConditionGotChar*)trans->AddCondition("GotChar");
            condGotChar->SetChar('>');
        // ' ' -> continue
        trans = state->AddSimpleTransition("SEEK_VALUE", "CHAR");
            trans->AddCondition("Blank");
        // " -> READING_VALUE
        trans = state->AddSimpleTransition("READING_VALUE", "CHAR");
            condGotChar = (CTOKConditionGotChar*)trans->AddCondition("GotChar");
            condGotChar->SetChar('\"');

    // copy to buffer "value"
    state = fsm->AddState("READING_VALUE");
        // " -> SEEK_ATTR
        trans = state->AddSimpleTransition("SEEK_ATTR", "CHAR");
            condGotChar = (CTOKConditionGotChar*)trans->AddCondition("GotChar");
            condGotChar->SetChar('\"');
            actionSetAttribute = (CTOKActionSetAttribute*)trans->AddAction("SetAttribute");
            actionSetAttribute->SetNameBuffer(&name_p);
            actionSetAttribute->SetValueBuffer(&value_p);
            actionSetAttribute->SetTokenizer(this);
        // else continue
        trans = state->AddSimpleTransition("READING_VALUE", "CHAR");
            actionCopyToBuffer = (CTOKActionCopyToBuffer*)trans->AddAction("CopyToBuffer");
            actionCopyToBuffer->SetBuffer(&value_p);

    // "<!" read, seek end of comment
    state = fsm->AddState("SKIPPING_COMMENT");
        // '-' -> SKIPPING_COMMENT2
        trans = state->AddSimpleTransition("SKIPPING_COMMENT2", "CHAR");
            condGotChar = (CTOKConditionGotChar*)trans->AddCondition("GotChar");
            condGotChar->SetChar('-');
        // else continue

    // one or more '-' read
    state = fsm->AddState("SKIPPING_COMMENT2");
        // '>' -> SEEK_TAG
        trans = state->AddSimpleTransition("SEEK_TAG", "CHAR");
            condGotChar = (CTOKConditionGotChar*)trans->AddCondition("GotChar");
            condGotChar->SetChar('>');
        // '-' continue
        trans = state->AddSimpleTransition("SKIPPING_COMMENT2", "CHAR");
            condGotChar = (CTOKConditionGotChar*)trans->AddCondition("GotChar");
            condGotChar->SetChar('-');
        // else -> SKIPPING_COMMENT
        trans = state->AddSimpleTransition("SKIPPING_COMMENT", "CHAR");

    // '/' read
    state = fsm->AddState("CHECK_END_TAG");
        // '>' -> FINISHED
        trans = state->AddSimpleTransition("FINISHED", "CHAR");
            condGotChar = (CTOKConditionGotChar*)trans->AddCondition("GotChar");
            condGotChar->SetChar('>');
            actionSetTrue = (CTOKActionSetTrue*)trans->AddAction("SetTrue");
            actionSetTrue->SetVariable(&tagClosed);
        // ' ' -> SEEK_ATTR
        trans = state->AddSimpleTransition("SEEK_ATTR", "CHAR");
            trans->AddCondition("Blank");
        // else -> READING_TAG
        trans = state->AddSimpleTransition("READING_ATTR", "CHAR");
            actionCopyToBuffer = (CTOKActionCopyToBuffer*)trans->AddAction("CopyToBuffer");
            actionCopyToBuffer->SetBuffer(&name_p);

    // the end state
    state = fsm->AddState("FINISHED");

    // setup
    fsm->SetInitialState("SEEK_TAG");
}

CTokenizer::~CTokenizer()
{
    delete fsm;
}

bool CTokenizer::GetTag()
{
    bool res = true;

    name_p = name;
    value_p = value;
    tagname_p = tagname;

    *name_p = '\0';
    *value_p = '\0';
    *tagname_p = '\0';
    Attributes.clear();
    tag="";
    tagClosed=false;

    char ch;

    if (!fsm->Start())
    {
        iRethrow();
        return false;
    }
    while(fsm->GetCurrentState() != state)
    {
        ch = fgetc(inFile);
        if (feof(inFile))
        {
            iThrow(CRITICAL(TOK_UNEXPECTED_EOF));
            break;
        }
        res = fsm->ProcessEvent("CHAR", (void*)&ch);
        if (!res)
        {
            iRethrow();
            break;
        }
    }
    fsm->Stop();

    return res;
}

bool CTokenizer::GetAttribute(char *AttribName, char *retValue, int len)
{
    bool res = false;
    retValue[0]='\0';

    Attrib_Map::const_iterator i = Attributes.find(AttribName);
    if(!(i==Attributes.end()))
    {
        int l = (*i).second.size();
        if (l>len-1) l = len-1;
        strncpy(retValue, (*i).second.c_str(), l+1);
        res = true;
    } else
    {
        // Attribute not found
    }
    return res;
}

const char*
CTokenizer::GetTagName()
{
    return tag.c_str();
};

bool
CTokenizer::IsTagClosed()
{
    return tagClosed;
};

/////////////////////////////////////////////////
}; // namespace FSM

