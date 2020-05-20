#ifndef _SQL_OPCODES_H_
#define _SQL_OPCODES_H_
/**************************************************
* File: sqlOpcodes.h.
* Desc: Definition of the SQL opcode tools.
* Module: AkraLog : SqlSup.
* Rev: 21 mai 1998 : REV 0 : Hugo DesRosiers : Creation..
**************************************************/

#include <akra/AkObject.h>

class SqlTokens : public AkObject {
  public:
    enum Tokens {
	immediateConstant= 0x0E0		// 16 instructions are used by this one.
	, queryExpr= 1				// Full query expression.
	, list						// List expression (followed by short count).
	, starIdentifier				// Reference to '*' identifier.
	, nullIdentifier				//  Reference to 'null' identifier.
	, moduleIdentifier			//  Reference to 'module' identifier.
	, identReference			// Reference to an identifer from pool (followed by short id).
	, containerReference		// Reference to an identifer that is container of following idents (followed by short id).
	, synonymExpr			// Relationship <expr> AS <identifier> (followed by <syn.> id, and then <expr>).
	, extSynonymExpr			// Relationship <expr> AS <identifier> '(' <derived column list> ')' (followed by <der. col. list>, <syn.> id, and <expr>).
	, nativeHolder				// Indicate a hole to be filled by the native values (followed by short hole #).
	, indicatorHolder			// Indicates a hole to be filled by the native values (followed by short hole #).
	, specialConstant			// Constant value, special kind defined by SQL (followed by byte id).
	, enumConstant			// Constant value, id of constant in enum list (followed by short id).
	, byteLiteral				// Constant value, byte integer.
	, shortILiteral				// Constant value, short (2 bytes) integer.
	, integerLiteral				// Constant value, natural (4 bytes) integer.
	, floatLiteral				// Constant value, floating point, small precision (4 bytes).
	, trueBoolLiteral
	, falseBoolLiteral
	, maybeBoolLiteral
	, charLiteral				// Constant value, character expression (followed by a short  ref ID).
	    // Operations.
	, orOp, andOp, notOp, isOp, isnotOp, parenthesisOp,
	equalOp, notEqualOp, ltOp, gtOp, leOp, geOp,
	betweenOp, notBetweenOp, inOp, notInOp,
	likeOp, notLikeOp, escapeOp,
	eqAllOp, eqSomeOp, eqAnyOp, neAllOp, neSomeOp, neAnyOp,
	ltAllOp, ltSomeOp, ltAnyOp, gtAllOp, gtSomeOp, gtAnyOp,
	leAllOp, leSomeOp, leAnyOp, geAllOp, geSomeOp, geAnyOp,
	existsOp, uniqueOp, overlapsOp, matchOp, matchUniqueOp,
	matchPartialOp, matchFullOp, matchUniPartialOp, matchUniFullOp,
	addOp, substractOp, multiplyOp, divideOp, posateOp, negateOp,
	userDefinedOp

	, functionCall				// Function call (either SQL or custom).

    };


    enum Modifiers {
	quantUnspec= 0, quantDistinct= 1, quantAll= 2,
    };

    enum SpecialConstants {
	defaultCte= 0, user= 1, current= 2, session= 3, system= 4, value= 5
	, fctCustom= 0, fctCountStar= 1, fctAvg= 2, fctMax= 3, fctMin= 4, fctSum= 5, fctCount= 6
	, ordNatural= 0, ordNext= 1, ordPrior= 2, ordFirst= 3, ordLast= 4, ordAbsolute= 5, ordRelative= 6
    };

  public:		// Class methods.
};


#endif		/* _SQL_OPCODES_H_ */
