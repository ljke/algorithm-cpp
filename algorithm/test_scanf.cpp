#include <stdio.h>
#include <stdlib.h>
int main()
{
	int a = 0, b=1;
	scanf_s("a=%d", &a);
	scanf_s("%c", &b);
	printf("a=%d, b=%d\n", a, b);
	system("pause");
	return 0;
}