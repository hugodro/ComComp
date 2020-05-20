#ifndef _SQL3SUPPORT_H_
#define _SQL3SUPPORT_H_
/**************************************************
* File: sql3Support.h.
* Desc: Contains the definitions for using SQL3 statements.
* Module: AkraLog : JavaComp.
* Rev: 13 mai 1997 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/


// SQL subtypes.
#define SCHEMA_SQL				(1 << 8)
#define DATA_SQL				(2 << 8)
#define TRANSACTION_SQL		(3 << 8)
#define CONTROL_SQL			(4 << 8)
#define CONNECTION_SQL		(5 << 8)
#define SESSION_SQL				(6 << 8)
#define DIAGNOSTIC_SQL			(7 << 8)

// SQL Schema sub-subtypes.
#define DEFINITION_SCHEMA
#define ALTER_SCHEMA
#define DROP_SCHEMA
#define DOM_DEF_SCHEMA
#define DROP_DOM_SCHEMA
#define TABLE_DEF_SCHEMA
#define DROP_TABLE_SCHEMA
#define VIEW_DEF_SCHEMA
#define DROP_VIEW_SCHEMA
#define ASSERT_DEF_SCHEMA
#define DROP_ASSERT_SCHEMA
#define TABLE_ALTER_SCHEMA
#define DOM_ALTER_SCHEMA
#define GRANT_SCHEMA
#define REVOKE_SCHEMA
#define CHARSET_DEF_SCHEMA
#define CHARSET_DROP_SCHEMA
#define COLLATION_DEF_SCHEMA
#define COLLATION_DROP_SCHEMA
#define TRANSL_DEF_SCHEMA
#define TRANSL_DROP_SCHEMA
#define TRIGGER_DEF_SCHEMA
#define TRIGGER_DROP_SCHEMA
#define NLCLASS_DEF_SCHEMA
#define NLCLASS_DROP_SCHEMA
#define ADT_DEF_SCHEMA
#define TYPETEMPL_DEF_SCHEMA
#define TYPETEMPL_DROP_SCHEMA
#define ADT_DROP_SCHEMA
#define DISTINCTYPE_DEF_SCHEMA
#define OPER_DEF_SCHEMA
/* ATTN: ANSI-Only.
#define DFLTROLE_DEF_SCHEMA
#define DFLTROLE_DROP_SCHEMA
*/
#define ROLE_DEF_SCHEMA
#define ROLE_GRANT_SCHEMA
#define ROLE_REVOKE_SCHEMA
#define ROLE_DROP_SCHEMA
#define MODULE_SCHEMA

// SQL Data sub-subtypes.
// ATTN-980525 [HD]: The token IDs are not the optimized values, just ordinal position
//	in the sql-3 standard document.
#define TMPTBL_DECL_DATA		32
#define TMPADT_DECL_DATA		33
#define TMPVIEW_DECL_DATA		34
#define CURSOR_DECL_DATA		35
#define OPEN_DATA				36
#define CLOSE_DATA				37
#define FETCH_DATA				38
#define SELECT_SNG_DATA		39
#define FREELOCATOR_DATA		40
// These are considered a sub-sub-subsection (data change).
#define INSERT_DATA				41
#define DELETE_SEARCH_DATA	42
#define DELETE_POS_DATA		43
#define UPDATE_SEARCH_DATA	44
#define UPDATE_POS_DATA		45

// SQL Transaction sub-subtypes.
#define START_TRANSACT
#define  SET_TRANSACT
#define  SET_CONSTRAINT_TRANSACT
#define  COMMIT_TRANSACT
#define  ROLLBACK_TRANSACT
#define  TEST_COMPL_TRANSACT
#define  SAVEPT_TRANSACT
#define  RELEASE_SAVEPT_TRANSACT
#define  _TRANSACT

// SQL Connection sub-subtypes.
#define CONNECT_CONNCT
#define SET_CONNCT
#define DISCONNECT_CONNCT

// SQL Session sub-subtypes.
#define SET_CHAR_SESSION
#define SET_AUTH_SESSION
#define SET_ROLE_SESSION
#define SET_LOCALTM_SESSION

// SQL Diagnostic sub-subtypes.
#define GET_DIAGNOSTIC

// Non-official statements.
#define SELECT_MULT_DATA	46

#endif		/* _SQL3SUPPORT_H_ */
