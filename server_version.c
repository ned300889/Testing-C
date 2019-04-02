#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

void do_exit(PGconn *conn){

	PQfinish(conn);
	exit(1);
}	

int main() {

	PGconn *conn = PQconnectdb("user=postgres dbname=trial password=test ");

	if (PQstatus(conn) == CONNECTION_BAD) {
		fprintf(stderr, "Connection to database failed: %s\n",
			PQerrorMessage(conn));
		do_exit(conn);
	}

	char *user = PQuser(conn);
	char *db_name = PQdb(conn);
	char *pswd = PQpass(conn);
	
	int ver = PQserverVersion(conn);

	printf("Server version: %d\n", ver);

	PQfinish(conn);

	return 0;
}


