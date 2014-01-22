/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtScript module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-ONLY$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** us via http://www.qt-project.org/.
**
** $QT_END_LICENSE$
**
****************************************************************************/

// This file was generated by qlalr - DO NOT EDIT!


//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

//
// This file is automatically generated from qscript.g.
// Changes will be lost.
//

#ifndef QSCRIPTPARSER_P_H
#define QSCRIPTPARSER_P_H

#include "qscriptgrammar_p.h"

#include "qscriptastfwd_p.h"

#include <stdlib.h>

QT_BEGIN_NAMESPACE

class QString;
class QScriptEnginePrivate;
class QScriptNameIdImpl;

class QScriptParser: protected QScriptGrammar
{
public:
    union Value {
      int ival;
      double dval;
      QScriptNameIdImpl *sval;
      QScript::AST::ArgumentList *ArgumentList;
      QScript::AST::CaseBlock *CaseBlock;
      QScript::AST::CaseClause *CaseClause;
      QScript::AST::CaseClauses *CaseClauses;
      QScript::AST::Catch *Catch;
      QScript::AST::DefaultClause *DefaultClause;
      QScript::AST::ElementList *ElementList;
      QScript::AST::Elision *Elision;
      QScript::AST::ExpressionNode *Expression;
      QScript::AST::Finally *Finally;
      QScript::AST::FormalParameterList *FormalParameterList;
      QScript::AST::FunctionBody *FunctionBody;
      QScript::AST::FunctionDeclaration *FunctionDeclaration;
      QScript::AST::Node *Node;
      QScript::AST::PropertyName *PropertyName;
      QScript::AST::PropertyNameAndValueList *PropertyNameAndValueList;
      QScript::AST::SourceElement *SourceElement;
      QScript::AST::SourceElements *SourceElements;
      QScript::AST::Statement *Statement;
      QScript::AST::StatementList *StatementList;
      QScript::AST::VariableDeclaration *VariableDeclaration;
      QScript::AST::VariableDeclarationList *VariableDeclarationList;
    };

    struct Location {
      int startLine;
      int startColumn;
      int endLine;
      int endColumn;
    };

public:
    QScriptParser();
    ~QScriptParser();

    bool parse(QScriptEnginePrivate *driver);

    inline QString errorMessage() const
    { return error_message; }
    inline int errorLineNumber() const
    { return error_lineno; }
    inline int errorColumnNumber() const
    { return error_column; }

protected:
    inline void reallocateStack();

    inline Value &sym(int index)
    { return sym_stack [tos + index - 1]; }

    inline Location &loc(int index)
    { return location_stack [tos + index - 2]; }

protected:
    int tos;
    int stack_size;
    Value *sym_stack;
    int *state_stack;
    Location *location_stack;
    QString error_message;
    int error_lineno;
    int error_column;
};

inline void QScriptParser::reallocateStack()
{
    if (! stack_size)
        stack_size = 128;
    else
        stack_size <<= 1;

    sym_stack = reinterpret_cast<Value*> (realloc(sym_stack, stack_size * sizeof(Value)));
    state_stack = reinterpret_cast<int*> (realloc(state_stack, stack_size * sizeof(int)));
    location_stack = reinterpret_cast<Location*> (realloc(location_stack, stack_size * sizeof(Location)));
}


#define Q_SCRIPT_REGEXPLITERAL_RULE1 7

#define Q_SCRIPT_REGEXPLITERAL_RULE2 8

QT_END_NAMESPACE

#endif // QSCRIPTPARSER_P_H
