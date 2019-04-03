#include <stdlib.h>
#include <libpq-fe.h>
#include <stdio.h>
#include <string.h>

void do_exit(PGconn *conn, PGresult *res) {
    
    fprintf(stderr, "%s\n", PQerrorMessage(conn));    

    PQclear(res);
    PQfinish(conn);    
    
    exit(1);
}

int main () {

	PGconn *conn = PQconnectdb("user=postgres dbname=postgres");

	if (PQstatus(conn) == CONNECTION_BAD) {
		fprintf(stderr, "Connection to database failed: %s\n",
				PQerrorMessage(conn));
	
		PQfinish(conn);
		exit(1);
	}
/* TODO 
 *
 * Move to a function for repeat calls
 *
 */

 	PGresult *res = PQexec(conn, "DROP DATABASE IF EXISTS benchmark");
	if (PQresultStatus(res) != PGRES_COMMAND_OK){
		do_exit(conn, res);
	}
	PQclear(res);

	res = PQexec(conn, "CREATE DATABASE benchmark");
		if (PQresultStatus(res) != PGRES_COMMAND_OK) {
			do_exit(conn, res);
		}

	printf("Creating Database Structure\n");

	printf("Filling Database with junk\n");

	system("pgbench -i -s 20 -p 5432 -d benchmark");
/* TODO 
 *
 * Create loop to run through benchmark 15 times and gather the average
 *
 */
	int i;

	for ( i = 0 ; i <= 15; i++){ 
	system("pgbench benchmark -c 100 -T 3 -S -n >> /tmp/results.txt");
	}

    char c[1000];
    FILE *fptr;

    if ((fptr = fopen("/tmp/results.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);         
    }

    // reads text until newline 
    fscanf(fptr,"%[^\n]", c);

    printf("Data from the file:\n%s", c);
    fclose(fptr);
    
    return 0;
	PQclear(res);
	PQfinish(conn);

	return 0;


}
