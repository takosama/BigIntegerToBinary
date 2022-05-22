#include<stdio.h>
#include<stdlib.h>

char _Rin_BCDtoBin_Converter_getval(char* arr, int end, int n, int offset)
{
	int idx = end - 1 - n;

	if (idx % 2 == 0)
		offset += 4;

	short val = (*(arr + idx / 2 - 1) * 256) | (255 & *(arr + idx / 2));
	val >>= offset;
	val &= 15;

	return val;
}

void _Rin_BCDtoBin_Converter_setval(char* arr, int end, int n, int offset, char val)
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

char* _Rin_BCDtoBin_Converter_Read_DECtoBCD(FILE* input,int* cnt)
{
	int size = 1;
	 *cnt = 1;

	char* p = malloc(size);
	*p = NULL;
	while (1)
	{
		char c = getchar();
		if (c > '9' || c < '0')
			break;

		if (*cnt % 2 == 0)
			*(p + *cnt / 2) = (c - '0') * 16;
		else
			*(p + *cnt / 2) += (c - '0');

		(* cnt)++;
		if (size * 2 <= cnt)
		{
			p = realloc(p, size * 2);
			if (p == NULL)
			{
				return -1;
			}
			size *= 2;
		}
	}
	return p;
}

typedef struct _Rin_BCDtoBin_Converter_State
{
	int cnt;
	int end;
	int iter;
	char* ptr;
}BCDtoBin_Converter_State;

typedef struct _Rin_BCDtoBin_Converter_Result
{
	int value;
	int isend;
}BCDtoBin_Converter_Result;


void _Rin_BCDtoBin_Converter_Init(BCDtoBin_Converter_State* s, BCDtoBin_Converter_Result* r, char* BCD_array, int cnt)
{
	s->end = 0;
	s->iter = 0;
	s->cnt = cnt;
	s->ptr = BCD_array;
	r->value= 1 & _Rin_BCDtoBin_Converter_getval(s->ptr, s->cnt, s->end, s->iter);
	r->isend = 0;
}


void _Rin_BCDtoBin_Converter_ComputeNext(BCDtoBin_Converter_State* s, BCDtoBin_Converter_Result* r)
{
	s->iter++;
	if (s->iter == 4)
	{
		s->iter = 0;
		s->end++;
	}

	r->isend = 0;
	char c = 0;
	for (int i = s->cnt - 2; i >= s->end; i--)
	{
		c = _Rin_BCDtoBin_Converter_getval(s->ptr, s->cnt, i, s->iter);
		if (c >= 8)
		{
			c -= 3;
			_Rin_BCDtoBin_Converter_setval(s->ptr, s->cnt, i, s->iter, c);
		}
		r->isend |= c;
	}
	r->isend = !r->isend;
	r->value = c&1;
}


struct  _Rin_BCDtoBin_Converter_State state;
struct  _Rin_BCDtoBin_Converter_Result result;

int main(void)
{
	int* pcnt = malloc(sizeof(int));
	char* p= _Rin_BCDtoBin_Converter_Read_DECtoBCD(stdin, pcnt);

	_Rin_BCDtoBin_Converter_Init(&state, &result, p, *pcnt);
	putchar(result.value+'0');

	while (1)
	{
		_Rin_BCDtoBin_Converter_ComputeNext(&state, &result);
		if (result.isend)
			break;
		putchar(result.value + '0');
	}
	putchar('\n');

	free(p);
	free(pcnt);
	return 0;
}
