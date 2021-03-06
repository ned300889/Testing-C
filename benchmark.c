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

int main (int argc, char *argv[]) {

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
	FILE *fp;
	char path[1035];
	int i;
	int num[15];
	int sum = 0;
	int avg = 0;

	for ( i = 0 ; i <= 15; i++){ 
	
	fp = popen("pgbench benchmark -c 100 -T 3 -S -n | grep -i processed | awk '{printf $6}'", "r");
	if (fp == NULL ) {
		printf("Failed to run command, check user\n");
		exit(1);
	}
	while (fgets(path, sizeof(path)-1, fp) != NULL){
	num[i] = printf("%s", path);
		printf("\n");
		/*scanf("%s", &num[i]);*/
	}
	}

	for ( i = 0 ; i <=15; i++){
		sum = sum+num[i];
	}

	avg = sum/15;
	printf("Average of entered number is: %d", avg);
	

	
	pclose(fp);

	PQclear(res);
	PQfinish(conn);

	return 0;


}
