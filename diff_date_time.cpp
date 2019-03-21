#include <stdio.h>
#include <ctype.h> // tolower()
#include <time.h>


void readDate(struct tm &date){
	scanf(
		"%d %d %d %d %d",
		&date.tm_year, &date.tm_mon, &date.tm_mday,
		&date.tm_hour, &date.tm_min, &date.tm_sec
	);
}

int main()
{
	time_t end_date;
	time(&end_date); // update 'end_date' variable to current time
	double seconds;

	struct tm init_date;
	char ans;

	printf("Do you want to calculate final date: (y,n): ");
	scanf("%c", &ans);

	printf("Initial date (year month day hour minute second): ");
	readDate(init_date);


	if(char(tolower(ans)) == 'y'){
		struct tm *end_date_ = localtime(&end_date);
		printf("Final date (year month day hour minute second): ");
		readDate(*end_date_);
		end_date = mktime(end_date_);
	}

	seconds = difftime(end_date,mktime(&init_date));

  	printf ("Difference between two dates: %.f seconds.\n", seconds);
	return 0;
}