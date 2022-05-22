#include<stdio.h>
#include<stdlib.h>

char getval(char* arr,int end, int n, int offset)
{
	int idx = end - 1 - n;

	if (idx % 2 == 0)
		offset += 4;
	
	short val = (*(arr + idx / 2 - 1) * 256) | (255 & *(arr + idx / 2));
	val >>= offset;
	val &= 15;

return val;
}

void setval(char* arr, int end, int n, int offset, char val)
{
	int idx = end - 1 - n;

	if (idx % 2 == 0)
		offset += 4;

	short tmp = val << offset;
	short mask = (0xf << offset);

	char tmp0 = tmp & 255;
	char tmp1 = tmp >> 8;

	char mask0 = mask & 255;
	char mask1 = mask >> 8;

	*(arr + idx / 2) = (*(arr + idx / 2) & ~mask0) | (tmp0 & mask0);
	*(arr + idx / 2 - 1) = (*(arr + idx / 2 - 1) & ~mask1) | (tmp1 & mask1);
}

int main(void)
{
	int size = 1;
	int cnt = 1;
	char* p = malloc(size);
	*p = NULL;
	while (1)
	{
		char c = getchar();
		if (c > '9' || c < '0')
			break;

		if (cnt % 2 == 0)
			*(p + cnt / 2) = (c - '0') * 16;
		else
			*(p + cnt / 2) += (c - '0');

		cnt++;
		if (size * 2 <= cnt)
		{
			p = realloc(p, size * 2);
			if (p == NULL)
			{
				return -1;
			}
			size *= 2;
		}
		putchar(c);
	}
	putchar('\n');

	int iter = 0;
	int pos = 0;
	int end = 0;
	char c = -1;
	int i = 0;
	int allzero = 0;
	c = getval(p, cnt, end, iter);
	while (1)
	{
		putchar((c & 1) + '0');

		iter++;
		if (iter == 4)
		{
			iter = 0;
			end++;
		}

		allzero = 0;
		for (i = cnt - 2; i >= end; i--)
		{
			c = getval(p, cnt, i, iter);
			if (c >= 8)
			{
				c -= 3;
				setval(p, cnt, i, iter, c);
			}
			allzero |= c;
		}

		if (!allzero)
			break;
	}

	putchar('\n');
	return 0;
}
