/**************************************************
* File: TextOutputBuffer.cc.
* Desc: Implementation of the TextOutputBuffer class.
* Module: AkraLog : SqlSupport.
* Rev: 27 mai 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <stdio.h>
#if defined(_WIN32)
#include <string.h>
#endif
#include "execFrame.h"	// For SqlIdentifier & SqlConstant.
#include "textOutputBuffer.h"


char *TextOutputBuffer::keywords[TextOutputBuffer::nbrKeywords]= {
    "ABSOLUTE", "ACTION", "ACTOR", "ADA", "ADD", "ADMIN", "AFTER", "ALL",
    "ALTER", "AND", "ANY", "ARE", "AS", "ASC", "ASSERTION", "ASYNC", "AT",
    "ATOMIC", "AUTHORIZATION", "AVG", "BEFORE", "BEGIN", "BETWEEN", "BIN",
    "BINARY", "BIT", "BIT_LENGTH", "BLOB", "BOOLEAN", "BOTH", "BREADTH",
    "BY", "CALL", "CASCADE", "CASCADED", "CASE", "CAST", "CATALOG_NAME",
    "CHAIN", "CHAR", "CHARACTER", "CHARACTERISTICS",
    "CHARACTER_LENGTH", "CHAR_LENGTH", "CHECK", "CLASS",
    "CLASS_ORIGIN", "CLOB", "CLOSE", "COALESCE", "COBOL", "COLLATE",
    "COLLATION", "COLLECTION", "COLUMN", "COLUMN_NAME",
    "COMMAND_FUNCTION", "COMMAND_FUNCTION_CODE", "COMMIT",
    "COMMITTED", "COMPLETION", "CONDITION_NUMBER", "CONNECT",
    "CONNECTION", "CONNECTION_NAME", "CONSTANT", "CONSTRAINT",
    "CONSTRAINTS", "CONSTRAINT_CATALOG", "CONSTRAINT_NAME",
    "CONSTRAINT_SCHEMA", "CONSTRUCTOR", "CONVERT", "CORRESPONDING",
    "COUNT", "CREATE", "CROSS", "CURRENT", "CURRENT_DATE",
    "CURRENT_PATH", "CURRENT_TIME", "CURRENT_TIMESTAMP",
    "CURRENT_USER", "CURSOR", "CURSOR_NAME", "CYCLE", "DATA", "DATE",
    "DAY", "DEC", "DECIMAL", "DECLARE", "DEFAULT", "DEFERRABLE",
    "DEFERRED", "DELETE", "DEPTH", "DESC", "DESTRUCTOR", "DIAGNOSTICS",
    "DICTIONARY", "DISCONNECT", "DISTINCT", "DO", "DOMAIN", "DOUBLE",
    "DROP", "EACH", "ELEMENT", "ELSE", "ELSEIF", "END", "EQUALS", "ESCAPE",
    "EXCEPT", "EXCEPTION", "EXECUTE", "EXISTS", "EXTERNAL", "EXTRACT",
    "FALSE", "FETCH", "FIRST", "FLOAT", "FOR", "FOREIGN", "FORTRAN",
    "FOUND", "FREE", "FROM", "FULL", "FUNCTION", "GENERAL", "GET",
    "GLOBAL", "GRANT", "GROUP", "HANDLER", "HASH", "HAVING", "HOLD",
    "HOUR", "IDENTIFIER", "IDENTITY", "IF", "IGNORE", "IMMEDIATE", "IN",
    "INDICATOR", "INFIX", "INITIALLY", "INNER", "INOUT", "INSENSITIVE", "INSERT",
    "INSTANCE", "INSTEAD", "INT", "INTEGER", "INTERSECT", "INTERVAL", "INTO",
    "IS", "ISOLATION", "JOIN", "KEEP", "KEY", "LANGUAGE", "LARGE", "LAST",
    "LEADING", "LEAVE", "LEFT", "LESS", "LEVEL", "LIKE", "LIMIT", "LIST", "LOCAL",
    "LOCATOR", "LOOP", "LOWER", "MATCH", "MAX", "MESSAGE_LENGTH",
    "MESSAGE_OCTET_LENGTH", "MESSAGE_TEXT", "MIN", "MINUTE", "MODIFY",
    "MODULE", "MONTH", "MORE", "MOVE", "MULTISET", "MUMPS", "NAME",
    "NAMES", "NATIONAL", "NATURAL", "NCHAR", "NCLOB", "NEW", "NEWLINE",
    "NEW_TABLE", "NEXT", "NO", "NONE", "NOT", "NULL", "NULLIF", "NUMBER",
    "NUMERIC", "OBJECT", "OCTET_LENGTH", "OF", "OFF", "OID", "OLD",
    "OLD_TABLE", "ON", "ONLY", "OPEN", "OPERATORS", "OPTION", "OR",
    "ORDER", "OUT", "OUTER", "OVERLAPS", "OVERLAY", "PAD", "PARAMETER",
    "PARTIAL", "PASCAL", "PATH", "PENDANT", "PLACING", "PLI", "POSITION",
    "POSTFIX", "PRECISION", "PREFIX", "PREORDER", "PRESERVE", "PRIMARY",
    "PRIOR", "PRIVATE", "PRIVILEGES", "PROCEDURE", "PROTECTED", "PUBLIC",
    "READ", "REAL", "RECURSIVE", "REDO", "REFERENCES", "REFERENCING",
    "RELATIVE", "RELEASE", "REPEATABLE", "RESIGNAL", "RESTRICT", "RETURN",
    "RETURNED_SQLSTATE", "RETURNS", "REVOKE", "RIGHT", "ROLE",
    "ROLLBACK", "ROUTINE", "ROUTINE_CATALOG", "ROUTINE_NAME",
    "ROUTINE_SCHEMA", "ROW", "ROWS", "ROW_COUNT", "SAVEPOINT",
    "SCHEMA", "SCHEMA_NAME", "SCROLL", "SEARCH", "SECOND", "SELECT",
    "SENSITIVE", "SEQUENCE", "SERIALIZABLE", "SERVER_NAME", "SESSION",
    "SESSION_USER", "SET", "SIGNAL", "SIMILAR", "SIZE", "SMALLINT", "SOME",
    "SPACE", "SPECIFIC", "SPECIFIC_NAME", "SQL", "SQLCODE",
    "SQLEXCEPTION", "SQLSTATE", "SQLWARNING", "START", "STATE",
    "STATEMENT", "STYLE", "SUBCLASS_ORIGIN", "SUBSTRING", "SUM",
    "SYSTEM_USER", "TABLE", "TABLE_NAME", "TEMPLATE", "TEMPORARY",
    "TEST", "THAN", "THEN", "THERE", "TIME", "TIMESTAMP", "TIMEZONE_HOUR",
    "TIMEZONE_MINUTE", "TO", "TRAILING", "TRANSACTION",
    "TRANSACTIONS_COMMITTED", "TRANSACTIONS_ROLLED_BACK",
    "TRANSACTION_ACTIVE", "TRANSLATE", "TRANSLATION", "TREAT",
    "TRIGGER", "TRIGGER_CATALOG", "TRIGGER_NAME", "TRIGGER_SCHEMA",
    "TRIM", "TRUE", "TYPE", "UNCOMMITTED", "UNDER", "UNDO", "UNION",
    "UNIQUE", "UNKNOWN", "UPDATABLE", "UPDATE", "UPPER", "USAGE", "USER",
    "USING", "VALUE", "VALUES", "VARCHAR", "VARIANT", "VARYING", "VIEW",
    "VISIBLE", "WAIT", "WHEN", "WHERE", "WHILE", "WITH", "WORK", "WRITE",
    "YEAR", "ZONE"
    , "..", "::", ":=", "<=", "<>", "=>", ">=", "||"
};

unsigned short TextOutputBuffer::offsets[nbrKeywords]= {
    0, 8, 14, 19, 22, 25, 30, 35, 38, 43, 46, 49, 52, 54, 57, 66, 71, 73, 79, 92, 95, 101,
    106, 113, 116, 122, 125, 135, 139, 146, 150, 157, 159, 163, 170, 178, 182, 186,
    198, 203, 207, 216, 231, 247, 258, 263, 268, 280, 284, 289, 297, 302, 309, 318,
    328, 334, 345, 361, 382, 388, 397, 407, 423, 430, 440, 455, 463, 473, 484, 502,
    517, 534, 545, 552, 565, 570, 576, 581, 588, 600, 612, 624, 641, 653, 659, 670,
    675, 679, 683, 686, 689, 696, 703, 710, 720, 728, 734, 739, 743, 753, 764, 774,
    784, 792, 794, 800, 806, 810, 814, 821, 825, 831, 834, 840, 846, 852, 861, 868,
    874, 882, 889, 894, 899, 904, 909, 912, 919, 926, 931, 935, 939, 943, 951, 958,
    961, 967, 972, 977, 984, 988, 994, 998, 1002, 1012, 1020, 1022, 1028, 1037,
    1039, 1048, 1053, 1062, 1067, 1072, 1083, 1089, 1097, 1104, 1107, 1114, 1123,
    1131, 1135, 1137, 1146, 1150, 1154, 1157, 1165, 1170, 1174, 1181, 1186, 1190,
    1194, 1199, 1203, 1208, 1212, 1217, 1224, 1228, 1233, 1238, 1241, 1255, 1275,
    1287, 1290, 1296, 1302, 1308, 1313, 1317, 1321, 1329, 1334, 1338, 1343, 1351,
    1358, 1363, 1368, 1371, 1378, 1387, 1391, 1393, 1397, 1400, 1404, 1410, 1416,
    1423, 1429, 1441, 1443, 1446, 1449, 1452, 1461, 1463, 1467, 1471, 1480, 1486,
    1488, 1493, 1496, 1501, 1509, 1516, 1519, 1528, 1535, 1541, 1545, 1552, 1559,
    1562, 1570, 1577, 1586, 1592, 1600, 1608, 1615, 1620, 1627, 1637, 1646, 1655,
    1661, 1665, 1669, 1678, 1682, 1692, 1703, 1711, 1718, 1728, 1736, 1744, 1750,
    1767, 1774, 1780, 1785, 1789, 1797, 1804, 1819, 1831, 1845, 1848, 1852, 1861,
    1870, 1876, 1887, 1893, 1899, 1905, 1911, 1920, 1928, 1940, 1951, 1958, 1970,
    1973, 1979, 1986, 1990, 1998, 2002, 2007, 2015, 2028, 2031, 2038, 2050, 2058,
    2068, 2073, 2078, 2087, 2092, 2107, 2116, 2119, 2130, 2135, 2145, 2153, 2162,
    2166, 2170, 2174, 2179, 2183, 2192, 2205, 2220, 2222, 2230, 2241, 2263, 2287,
    2305, 2314, 2325, 2330, 2337, 2352, 2364, 2378, 2382, 2386, 2390, 2401, 2406,
    2410, 2415, 2421, 2428, 2437, 2443, 2448, 2453, 2457, 2462, 2467, 2473, 2480,
    2487, 2494, 2498, 2505, 2509, 2513, 2518, 2523, 2527, 2531, 2536
    , 2540 , 2542, 2544, 2546, 2548, 2550, 2552, 2554
};

unsigned short TextOutputBuffer::lengths[nbrKeywords]= {
    8, 6, 5, 3, 3, 5, 5, 3, 5, 3, 3, 3, 2, 3, 9, 5, 2, 6, 13, 3, 6, 5, 7, 3, 6, 3, 10, 4, 7, 4, 7, 2, 4, 7, 8,
    4, 4, 12, 5, 4, 9, 15, 16, 11, 5, 5, 12, 4, 5, 8, 5, 7, 9, 10, 6, 11, 16, 21, 6, 9, 10, 16, 7, 10, 15,
    8, 10, 11, 18, 15, 17, 11, 7, 13, 5, 6, 5, 7, 12, 12, 12, 17, 12, 6, 11, 5, 4, 4, 3, 3, 7, 7, 7, 10,
    8, 6, 5, 4, 10, 11, 10, 10, 8, 2, 6, 6, 4, 4, 7, 4, 6, 3, 6, 6, 6, 9, 7, 6, 8, 7, 5, 5, 5, 5, 3, 7, 7, 5,
    4, 4, 4, 8, 7, 3, 6, 5, 5, 7, 4, 6, 4, 4, 10, 8, 2, 6, 9, 2, 9, 5, 9, 5, 5, 11, 6, 8, 7, 3, 7, 9, 8, 4, 2,
    9, 4, 4, 3, 8, 5, 4, 7, 5, 4, 4, 5, 4, 5, 4, 5, 7, 4, 5, 5, 3, 14, 20, 12, 3, 6, 6, 6, 5, 4, 4, 8, 5, 4,
    5, 8, 7, 5, 5, 3, 7, 9, 4, 2, 4, 3, 4, 6, 6, 7, 6, 12, 2, 3, 3, 3, 9, 2, 4, 4, 9, 6, 2, 5, 3, 5, 8, 7, 3,
    9, 7, 6, 4, 7, 7, 3, 8, 7, 9, 6, 8, 8, 7, 5, 7, 10, 9, 9, 6, 4, 4, 9, 4, 10, 11, 8, 7, 10, 8, 8, 6, 17,
    7, 6, 5, 4, 8, 7, 15, 12, 14, 3, 4, 9, 9, 6, 11, 6, 6, 6, 6, 9, 8, 12, 11, 7, 12, 3, 6, 7, 4, 8, 4, 5,
    8, 13, 3, 7, 12, 8, 10, 5, 5, 9, 5, 15, 9, 3, 11, 5, 10, 8, 9, 4, 4, 4, 5, 4, 9, 13, 15, 2, 8, 11, 22,
    24, 18, 9, 11, 5, 7, 15, 12, 14, 4, 4, 4, 11, 5, 4, 5, 6, 7, 9, 6, 5, 5, 4, 5, 5, 6, 7, 7, 7, 4, 7, 4,
    4, 5, 5, 4, 4, 5, 4, 4
    , 2, 2, 2, 2, 2, 2, 2, 2
};


TextOutputBuffer::TextOutputBuffer(void)
{
    currentText= new char[1024];	// TMP: no use of chained text buffers.
    state= initial;
    currentIndex= 0;
}


TextOutputBuffer::~TextOutputBuffer(void)
{
    delete currentText;
}


void TextOutputBuffer::reset(void)
{
    currentIndex= 0;
    state= initial;
}


void TextOutputBuffer::addKeyword(SqlKeywords aKeyword)
{
    if ((state == keyword) || (state == identifier) || (state == constant) || (state == closePar)) {
	currentText[currentIndex++]= ' ';
    }
    memcpy(&currentText[currentIndex], keywords[aKeyword], lengths[aKeyword]);
    currentIndex+= lengths[aKeyword];
    state= keyword;
}


void TextOutputBuffer::addIdentifier(SqlIdentifier *anIdent)
{
    if ((state == keyword) || (state == identifier) || (state == constant) || (state == closePar)) {
	currentText[currentIndex++]= ' ';
    }
    memcpy(&currentText[currentIndex], anIdent->value, anIdent->charLength);
    currentIndex+= anIdent->charLength;
    state= identifier;
}


void TextOutputBuffer::addStringConstant(SqlConstant *aStrCte)
{
    if ((state == keyword) || (state == identifier) || (state == constant)) {
	currentText[currentIndex++]= ' ';
    }
    currentText[currentIndex++]= '\'';
    memcpy(&currentText[currentIndex], aStrCte->value, aStrCte->charLength);
    currentIndex+= aStrCte->charLength;
    currentText[currentIndex++]= '\'';
    state= constant;
}


void TextOutputBuffer::addIntConstant(int value)
{
    if ((state == keyword) || (state == identifier) || (state == constant)) {
	currentText[currentIndex++]= ' ';
    }
    sprintf(&currentText[currentIndex], "%d", value);
    currentIndex+= strlen(&currentText[currentIndex]);
    state= constant;
}


void TextOutputBuffer::addRealConstant(float value)
{
    if ((state == keyword) || (state == identifier) || (state == constant)) {
	currentText[currentIndex++]= ' ';
    }
    sprintf(&currentText[currentIndex], "%f", value);
    currentIndex+= strlen(&currentText[currentIndex]);
    state= constant;
}


void TextOutputBuffer::addListSeparator(void)
{
    currentText[currentIndex++]= ',';
    currentText[currentIndex++]= ' ';
    state= listSep;
}


void TextOutputBuffer::addParenthesis(bool closedFlag)
{
    if (closedFlag) {
	currentText[currentIndex++]= ')';
	state= closePar;
    }
    else {
	currentText[currentIndex++]= '(';
	state= openPar;
    }
}


void TextOutputBuffer::addSingleOp(char anOpChar)
{
    currentText[currentIndex++]= anOpChar;
	state= singleChar;
}


char *TextOutputBuffer::getCompactOutput(void)
{
    currentText[currentIndex]= '\0';
    return currentText;
}


void TextOutputBuffer::addOperator(JCSqlTokens::Tokens anOp)
{
    switch(anOp) {
	case JCSqlTokens::orOp:
	    addKeyword(kwOr);
	    break;
	case JCSqlTokens::andOp:
	    addKeyword(kwAnd);
	    break;
	case JCSqlTokens::notOp:
	    addKeyword(kwNot);
	    break;
	case JCSqlTokens::isOp:
	    addKeyword(kwIs);
	    break;
	case JCSqlTokens::isnotOp:
	    addKeyword(kwIs);
	    addKeyword(kwNot);
	    break;
	case JCSqlTokens::equalOp:
	    addSingleOp('=');
	    break;
	case JCSqlTokens::notEqualOp:
	    addKeyword(spkNotEqual);
	    break;
	case JCSqlTokens::ltOp:
	    addSingleOp('<');
	    break;
	case JCSqlTokens::gtOp:
	    addSingleOp('>');
	    break;
	case JCSqlTokens::leOp:
	    addKeyword(spkSmallerEqual);
	    break;
	case JCSqlTokens::geOp:
	    addKeyword(spkGreaterEqual);
	    break;
	case JCSqlTokens::betweenOp:
	    addKeyword(kwBetween);
	    break;
	case JCSqlTokens::notBetweenOp:
	    addKeyword(kwNot);
	    addKeyword(kwBetween);
	    break;
	case JCSqlTokens::inOp:
	    addKeyword(kwIn);
	    break;
	case JCSqlTokens::notInOp:
	    addKeyword(kwNot);
	    addKeyword(kwIn);
	    break;
	case JCSqlTokens::likeOp:
	    addKeyword(kwLike);
	    break;
	case JCSqlTokens::notLikeOp:
	    addKeyword(kwNot);
	    addKeyword(kwLike);
	    break;
	case JCSqlTokens::escapeOp:
	    addKeyword(kwEscape);
	    break;
	case JCSqlTokens::eqAllOp:
	    addSingleOp('=');
	    addKeyword(kwAll);
	    break;
	case JCSqlTokens::eqSomeOp:
	    addSingleOp('=');
	    addKeyword(kwSome);
	    break;
	case JCSqlTokens::eqAnyOp:
	    addSingleOp('=');
	    addKeyword(kwAny);
	    break;
	case JCSqlTokens::neAllOp:
	    addKeyword(spkNotEqual);
	    addKeyword(kwAll);
	    break;
	case JCSqlTokens::neSomeOp:
	    addKeyword(spkNotEqual);
	    addKeyword(kwSome);
	    break;
	case JCSqlTokens::neAnyOp:
	    addKeyword(spkNotEqual);
	    addKeyword(kwAny);
	    break;
	case JCSqlTokens::ltAllOp:
	    addSingleOp('<');
	    addKeyword(kwAll);
	    break;
	case JCSqlTokens::ltSomeOp:
	    addSingleOp('<');
	    addKeyword(kwSome);
	    break;
	case JCSqlTokens::ltAnyOp:
	    addSingleOp('<');
	    addKeyword(kwAny);
	    break;
	case JCSqlTokens::gtAllOp:
	    addSingleOp('>');
	    addKeyword(kwAll);
	    break;
	case JCSqlTokens::gtSomeOp:
	    addSingleOp('>');
	    addKeyword(kwSome);
	    break;
	case JCSqlTokens::gtAnyOp:
	    addSingleOp('>');
	    addKeyword(kwAny);
	    break;
	case JCSqlTokens::leAllOp:
	    addKeyword(spkSmallerEqual);
	    addKeyword(kwAll);
	    break;
	case JCSqlTokens::leSomeOp:
	    addKeyword(spkSmallerEqual);
	    addKeyword(kwSome);
	    break;
	case JCSqlTokens::leAnyOp:
	    addKeyword(spkSmallerEqual);
	    addKeyword(kwAny);
	    break;
	case JCSqlTokens::geAllOp:
	    addKeyword(spkGreaterEqual);
	    addKeyword(kwAll);
	    break;
	case JCSqlTokens::geSomeOp:
	    addKeyword(spkGreaterEqual);
	    addKeyword(kwSome);
	    break;
	case JCSqlTokens::geAnyOp:
	    addKeyword(spkGreaterEqual);
	    addKeyword(kwAny);
	    break;
	case JCSqlTokens::existsOp:
	    addKeyword(kwExists);
	    break;
	case JCSqlTokens::uniqueOp:
	    addKeyword(kwUnique);
	    break;
	case JCSqlTokens::overlapsOp:
	    addKeyword(kwOverlaps);
	    break;
	case JCSqlTokens::matchOp:
	    addKeyword(kwMatch);
	    break;
	case JCSqlTokens::matchUniqueOp:
	    addKeyword(kwMatch);
	    addKeyword(kwUnique);
	    break;
	case JCSqlTokens::matchPartialOp:
	    addKeyword(kwMatch);
	    addKeyword(kwPartial);
	    break;
	case JCSqlTokens::matchFullOp:
	    addKeyword(kwMatch);
	    addKeyword(kwFull);
	    break;
	case JCSqlTokens::matchUniPartialOp:
	    addKeyword(kwMatch);
	    addKeyword(kwUnique);
	    addKeyword(kwPartial);
	    break;
	case JCSqlTokens::matchUniFullOp:
	    addKeyword(kwMatch);
	    addKeyword(kwUnique);
	    addKeyword(kwFull);
	    break;
	case JCSqlTokens::addOp:
	    addSingleOp('+');
	    break;
	case JCSqlTokens::substractOp:
	    addSingleOp('-');
	    break;
	case JCSqlTokens::multiplyOp:
	    addSingleOp('*');
	    break;
	case JCSqlTokens::divideOp:
	    addSingleOp('/');
	    break;
	case JCSqlTokens::posateOp:
	    addSingleOp('+');
	    break;
	case JCSqlTokens::negateOp:
	    addSingleOp('-');
	    break;
	default:
	    // TODO: Deal with unknown operator.
	    break;
    }
}


void TextOutputBuffer::addText(char *someText)
{
    if ((state == keyword) || (state == identifier) || (state == constant)) {
	currentText[currentIndex++]= ' ';
    }
    strcpy(&currentText[currentIndex], someText);
    currentIndex+= strlen(someText);
    state= constant;
}


void TextOutputBuffer::addStringConstant(char *someText, unsigned int textLength)
{
    currentText[currentIndex++]= '\'';
    memcpy(&currentText[currentIndex], someText, textLength);
    currentIndex+= textLength;
    currentText[currentIndex++]= '\'';
}

