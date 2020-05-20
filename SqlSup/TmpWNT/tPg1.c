/*
 * testlibpq.c
 * 	Test the C version of LIBPQ, the POSTGRES frontend library.
 *
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <libpq-fe.h>


void test1(PGconn *, int, char **);
void test2(PGconn *);
void test3(PGconn *, int, char **);
void test4(PGconn *);
void  exit_nicely(PGconn*);


#ifdef DEBUG
FILE *debug; 
#endif /* DEBUG */


main(int argc, char **argv)
{
    PGconn* conn;
    PGresult* res;
    char *pghost, *pgport, *pgoptions, *pgtty;
    char* dbName;

  /* begin, by setting the parameters for a backend connection
     if the parameters are null, then the system will try to use
     reasonable defaults by looking up environment variables 
     or, failing that, using hardwired constants */

    pghost = NULL;  /* host name of the backend server */
    pgport = NULL;  /* port of the backend server */
    pgoptions = NULL; /* special options to start up the backend server */
    pgtty = NULL;     /* debugging tty for the backend server */
    dbName = "testHugo1";

  /* make a connection to the database */
    conn = PQsetdb(pghost, pgport, pgoptions, pgtty, dbName);

  /* check to see that the backend connection was successfully made */
    if (PQstatus(conn) == CONNECTION_BAD) {
	fprintf(stderr,"Connection to database '%s' failed.\n", dbName);
	fprintf(stderr,"%s",PQerrorMessage(conn));
	exit_nicely(conn);
    }

#ifdef DEBUG
    debug = fopen("/tmp/trace.out","w");  
    PQtrace(conn, debug);  
#endif /* DEBUG */

//    test1(conn, argc, argv);
//    test2(conn);
//    test3(conn, argc, argv);
   test4(conn);


    /* close the connection to the database and cleanup */
    PQfinish(conn);

#ifdef DEBUG
  fclose(debug); 
#endif /* DEBUG */

    exit(0);
}
  

void test1(PGconn *conn, int argc, char **argv)
{
    PGresult* res;
    int nFields;
    int i, j, k, gotData, clientId, loop;
    char query[250];

  /* start a transaction block */
    res = PQexec(conn,"BEGIN"); 
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
	fprintf(stderr,"BEGIN command failed\n");
	PQclear(res);
	exit_nicely(conn);
    }
  /* should PQclear PGresult whenever it is no longer needed to avoid
     memory leaks */
    PQclear(res); 

    for (k= 1; k < argc; k++) {
	for (loop= 0; loop < 10; loop++) {
	    for (clientId= 2; clientId < 42; clientId++) {
		sprintf(query, "DECLARE myportal CURSOR FOR select clientID, eFirstName, eLastName from %s where clientID = %d", argv[k], clientId);
		res = PQexec(conn, query);
		if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		    fprintf(stderr,"DECLARE CURSOR command failed\n");
		    PQclear(res);
		    exit_nicely(conn);
		}
		PQclear(res);
	
		gotData= 1;
//	    do {
		    res= PQexec(conn,"fetch 2 in myportal");
		    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
			gotData= 0;
		    }
		    else {
			if (PQntuples(res) != 0) {
			    nFields = PQnfields(res);
#if defined(BE_VERBOSE)
			    /* first, print out the attribute names */
			    for (i=0; i < nFields; i++) {
				printf("%-15s",PQfname(res,i));
			    }
			    printf("\n\n");
#endif
			    /* next, print out the instances */
			    for (i=0; i < PQntuples(res); i++) {
				for (j=0  ; j < nFields; j++) {
				    PQgetvalue(res, i, j);
//				    printf("%-15s", PQgetvalue(res, i, j));
				}
//				printf("\n");
			    }
			}
			else gotData= 0;
		    }
		    PQclear(res);
//	    } while (gotData);
	
		/* close the portal */
		res = PQexec(conn, "CLOSE myportal");
		PQclear(res);
	    }
	}
    }

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);
}


void test2(PGconn *conn)
{
    PGresult* res;
    int clientID;
    char query[512];

    srandom(test2);
    clientID= 1000 + random();
    sprintf(query, "insert into Client (clientID,clientType,loanID,eCompanyName,phoneBus1,phoneRes1,phoneFax1,regisAddress,postalAddress,resAddress,signAddress, eFirstName,eLastName,birthday) values (%d ,2,21,'Kuwait numbered corporation #4028639','868-7458','993-8881','996-4018',160,159,158,161,'Hugo','Woods', to_date('05-08-1940','DD-MM-YYYY'))",
	    clientID);
    
    res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
	fprintf(stderr,"DECLARE CURSOR command failed\n");
	PQclear(res);
	exit_nicely(conn);
    }
    PQclear(res);
	
}


void test3(PGconn *conn, int argc, char **argv)
{
    PGresult* res;
    int nFields;
    int i, j, k, gotData;
    char query[250];

    for (k= 1; k < argc; k++) {
	sprintf(query, "select clientID, eFirstName, eLastName from %s", argv[k]);
	res = PQexec(conn, query);
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
	    fprintf(stderr,"SELECT command failed\n");
	    PQclear(res);
	    exit_nicely(conn);
	}

	if (PQntuples(res) != 0) {
	    nFields = PQnfields(res);
#if defined(BE_VERBOSE)
	    /* first, print out the attribute names */
	    for (i=0; i < nFields; i++) {
		printf("%-15s",PQfname(res,i));
	    }
	    printf("\n\n");
#endif
	    /* next, print out the instances */
	    for (i=0; i < PQntuples(res); i++) {
		for (j=0  ; j < nFields; j++) {
		    printf("%-15s", PQgetvalue(res, i, j));
		}
		printf("\n");
	    }
	}
	PQclear(res);
    }
}


void test4(PGconn *conn)
{
    PGresult* res;
    float amount;
    unsigned int i;
    char aicID[30];
    char query[512];

    srandom(test2);
    for (i= 0; i < 500; i++) {
	amount= (float)((random() % 10000) +1)/100.0;
	sprintf(query, "insert into PerfTestDB (AIC, Amount) values ('AIC%d', %f)",
		random() % 2, amount);
	
	res = PQexec(conn, query);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
	    fprintf(stderr,"DECLARE CURSOR command failed\n");
	    PQclear(res);
	    exit_nicely(conn);
	}
    }
    PQclear(res);
	
}



void  exit_nicely(PGconn* conn)
{
  PQfinish(conn);
  exit(1);
}

