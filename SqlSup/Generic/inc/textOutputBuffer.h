#ifndef _TEXTOUTPUTBUFFER_H_
#define _TEXTOUTPUTBUFFER_H_
/**************************************************
* File: TextOutputBuffer.h.
* Desc: Definition of the TextOutputBuffer class.
* Module: AkraLog : SqlSupport.
* Rev: 27 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>
#include <akra/portableDefs.h>
#include "sqlOpcodes.h"

class SqlIdentifier;
class SqlConstant;


class TextOutputBuffer : public AkObject {
  public:
    enum SqlKeywords {		// Contain Sql92 and Sql3 keywords.
	kwAbsolute, kwAction, kwActor, kwAda, kwAdd, kwAdmin, kwAfter,
	kwAll, kwAlter, kwAnd, kwAny, kwAre, kwAs, kwAsc, kwAssertion,
	kwAsync, kwAt, kwAtomic, kwAuthorization, kwAvg, kwBefore,
	kwBegin, kwBetween, kwBin, kwBinary, kwBit, kwBit_length, kwBlob,
	kwBoolean, kwBoth, kwBreadth, kwBy, kwCall, kwCascade,
	kwCascaded, kwCase, kwCast, kwCatalog_name, kwChain,
	kwChar, kwCharacter, kwCharacteristics, kwCharacter_length,
	kwChar_length, kwCheck, kwClass, kwClass_origin, kwClob,
	kwClose, kwCoalesce, kwCobol, kwCollate, kwCollation,
	kwCollection, kwColumn, kwColumn_name, kwCommand_function,
	kwCommand_function_code, kwCommit, kwCommitted, kwCompletion,
	kwCondition_number, kwConnect, kwConnection, kwConnection_name,
	kwConstant, kwConstraint, kwConstraints, kwConstraint_catalog,
	kwConstraint_name, kwConstraint_schema, kwConstructor, kwConvert,
	kwCorresponding, kwCount, kwCreate, kwCross, kwCurrent,
	kwCurrent_date, kwCurrent_path, kwCurrent_time, kwCurrent_timestamp,
	kwCurrent_user, kwCursor, kwCursor_name, kwCycle, kwData, kwDate,
	kwDay, kwDec, kwDecimal, kwDeclare, kwDefault, kwDeferrable, kwDeferred,
	kwDelete, kwDepth, kwDesc, kwDestructor, kwDiagnostics, kwDictionary,
	kwDisconnect, kwDistinct, kwDo, kwDomain, kwDouble, kwDrop, kwEach,
	kwElement, kwElse, kwElseif, kwEnd, kwEquals, kwEscape, kwExcept,
	kwException, kwExecute, kwExists, kwExternal, kwExtract, kwFalse,
	kwFetch, kwFirst, kwFloat, kwFor, kwForeign, kwFortran, kwFound,
	kwFree, kwFrom, kwFull, kwFunction, kwGeneral, kwGet, kwGlobal,
	kwGrant, kwGroup, kwHandler, kwHash, kwHaving, kwHold, kwHour,
	kwIdentifier, kwIdentity, kwIf, kwIgnore, kwImmediate, kwIn, kwIndicator,
	kwInfix, kwInitially, kwInner, kwInout, kwInsensitive, kwInsert, kwInstance,
	kwInstead, kwInt, kwInteger, kwIntersect, kwInterval, kwInto, kwIs,
	kwIsolation, kwJoin, kwKeep, kwKey, kwLanguage, kwLarge, kwLast,
	kwLeading, kwLeave, kwLeft, kwLess, kwLevel, kwLike, kwLimit, kwList,
	kwLocal, kwLocator, kwLoop, kwLower, kwMatch, kwMax,
	kwMessage_length, kwMessage_octet_length, kwMessage_text,
	kwMin, kwMinute, kwModify, kwModule, kwMonth, kwMore, kwMove,
	kwMultiset, kwMumps, kwName, kwNames, kwNational, kwNatural,
	kwNchar, kwNclob, kwNew, kwNewline, kwNew_table, kwNext, kwNo,
	kwNone, kwNot, kwNull, kwNullif, kwNumber, kwNumeric, kwObject,
	kwOctet_length, kwOf, kwOff, kwOid, kwOld, kwOld_table, kwOn,
	kwOnly, kwOpen, kwOperators, kwOption, kwOr, kwOrder, kwOut,
	kwOuter, kwOverlaps, kwOverlay, kwPad, kwParameter, kwPartial,
	kwPascal, kwPath, kwPendant, kwPlacing, kwPli, kwPosition, kwPostfix,
	kwPrecision, kwPrefix, kwPreorder, kwPreserve, kwPrimary, kwPrior,
	kwPrivate, kwPrivileges, kwProcedure, kwProtected, kwPublic,
	kwRead, kwReal, kwRecursive, kwRedo, kwReferences, kwReferencing,
	kwRelative, kwRelease, kwRepeatable, kwResignal, kwRestrict, kwReturn,
	kwReturned_sqlstate, kwReturns, kwRevoke, kwRight, kwRole, kwRollback,
	kwRoutine, kwRoutine_catalog, kwRoutine_name, kwRoutine_schema, kwRow,
	kwRows, kwRow_count, kwSavepoint, kwSchema, kwSchema_name, kwScroll,
	kwSearch, kwSecond, kwSelect, kwSensitive, kwSequence, kwSerializable,
	kwServer_name, kwSession, kwSession_user, kwSet, kwSignal, kwSimilar, kwSize,
	kwSmallint, kwSome, kwSpace, kwSpecific, kwSpecific_name, kwSql,
	kwSqlcode, kwSqlexception, kwSqlstate, kwSqlwarning, kwStart, kwState,
	kwStatement, kwStyle, kwSubclass_origin, kwSubstring, kwSum,
	kwSystem_user, kwTable, kwTable_name, kwTemplate, kwTemporary,
	kwTest, kwThan, kwThen, kwThere, kwTime, kwTimestamp, kwTimezone_hour,
	kwTimezone_minute, kwTo, kwTrailing, kwTransaction,
	kwTransactions_committed, kwTransactions_rolled_back,
	kwTransaction_active, kwTranslate, kwTranslation, kwTreat,
	kwTrigger, kwTrigger_catalog, kwTrigger_name, kwTrigger_schema,
	kwTrim, kwTrue, kwType, kwUncommitted, kwUnder, kwUndo, kwUnion,
	kwUnique, kwUnknown, kwUpdatable, kwUpdate, kwUpper, kwUsage, kwUser,
	kwUsing, kwValue, kwValues, kwVarchar, kwVariant, kwVarying, kwView,
	kwVisible, kwWait, kwWhen, kwWhere, kwWhile, kwWith, kwWork, kwWrite,
	kwYear, kwZone
	, spkDoubleDot, spkDoubleColon, spkColonEqual, spkSmallerEqual, spkNotEqual,
	 spkEqualGreater, spkGreaterEqual, spkDoubleBar
	, nbrKeywords
    };
    enum States {
	initial, keyword, identifier, constant, listSep, openPar, closePar, singleChar
    };

  protected:		// Class variables.
    static char *keywords[nbrKeywords];	// TMP: We use a table, with trailing '\0'.
    static unsigned short offsets[nbrKeywords];
    static unsigned short lengths[nbrKeywords];

  protected:		// Instance variables.
    States state;
    unsigned int currentIndex;
    char *currentText;		// TMP: non-chained text buffer.

  public:			// Instance methods.
    TextOutputBuffer(void);
    virtual ~TextOutputBuffer(void);
    virtual void reset(void);
    virtual void addKeyword(SqlKeywords aKeyword);
    virtual void addIdentifier(SqlIdentifier *anIdent);
    virtual void addStringConstant(SqlConstant *aStrCte);
    virtual void addIntConstant(int value);
    virtual void addRealConstant(float value);
    virtual void addListSeparator(void);
    virtual void addParenthesis(bool closedFlag);
    virtual void addSingleOp(char anOpChar);
    virtual char *getCompactOutput(void);
    virtual void addOperator(SqlTokens::Tokens anOp);
    virtual void addText(char *someText);
    virtual void addStringConstant(char *someText, unsigned int textLength);
};


#endif		/* _TEXTOUTPUTBUFFER_H_ */
