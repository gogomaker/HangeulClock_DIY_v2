#include <stdio.h>

int main() 
{
	int num;
	
	scanf("%d", &num);
	int seatOne = num % 10;
	int seatTen = (num - seatOne) / 10;

	printf("%d / %d", seatTen, seatOne);
	
	return 0;
}
