#include <stdio.h>
#include <stdlib.h>
#define CHECKSUM_LENGTH 32
// unsigned int *ips;
struct ip
{
	unsigned int IP_ADDRESS;
	struct ip *next;
};
struct node
{
	struct node* right;
	struct ip * ips;
	unsigned int count;
	unsigned char CHECKSUM[32];
};
int SIZE = 1 << 24;
struct node * RAM[1 << 24] = {NULL};
char a[65] = {'\0'};
char ipp[100] = {'\0'};

unsigned int * get_ips(struct ip * node_ip)
{
	printf("\nGet IPS");
	int i = 0;
	unsigned int *ips;
	ips = (unsigned int *)malloc(4*100);
	while(node_ip != NULL)
	{
		ips[i] = node_ip->IP_ADDRESS;
		printf("\n%u  ", ips[i]);
		i += 1;
		node_ip = node_ip->next;
	}
	ips[i] = 0;
	return ips;
}

unsigned int * retIntPointer(){
	unsigned int * p;
	unsigned int value = 0;
	p = &value;
	return p;
}
// void ii()
// {
// 	// size of int is 4
// 	ips = (unsigned int *)malloc(4*100);
// }

void _print(unsigned int ip)
{
    unsigned char cip = (unsigned char)ip;
	unsigned char *b = &cip;
    printf(" | %u.%u.%u.%u",*(b+3),*(b+2),*(b+1),*b);
}

void _print_ips(unsigned int * IPS)
{
	while(*IPS)
		_print(*(IPS++));
}


unsigned char ci(char a)
{
	return a>96?a-87:a-48;
}
char ic(unsigned char a)
{
	return a>9?a+87:a+48;
}

char* _into_string(unsigned char *c)
{
	int i = 0;
	while(i++ < 32)
	{
		a[2*(i-1)] = ic((*c & 240) >> 4);
		a[2*i-1] = ic(*c & 15);
		c++;
	}
	return a;
}

unsigned int char_to_ip(char *a)
{

	//char a[20] = "255.255.255.255."; --->> 4294967295
	int i = 0;
	int c = 4;
	unsigned int ip = 0;
	unsigned char h= 0;
	while(c)
	{
		ip <<= 8;
		for(;a[i]!='.';i++)
		{
			h *= 10;
			h += a[i]-48;
		}
		ip += h;
		i++;
		h=0;
		c--;
	}
	return ip;
}

void sss()
{
	int h = 0;
	printf("\nDEBUG:");
	scanf("%d",&h);
}

void Insert_ips(struct node *N, char *IP_ADDRESS, int lim)
{
	printf("\nInserting Ips: ");
	int l = 0, i = 0;
	struct ip* new_ip = (struct ip*) malloc(sizeof(struct ip));
	while(*IP_ADDRESS != '*')
	{
		ipp[l] = *IP_ADDRESS;
		IP_ADDRESS++;
		l ++;
	}
	//"192.168.1.104."
	IP_ADDRESS++;
	printf("%s ", ipp);
	l=0;
	new_ip->IP_ADDRESS = char_to_ip(ipp);
	new_ip->next = NULL;
	N->ips = new_ip;
	struct ip *f=N->ips;
	i++;
	while(i < lim)
	{
		struct ip *ff = (struct ip*) malloc(sizeof(struct ip));
		while(*IP_ADDRESS != '*')
		{
			ipp[l] = *IP_ADDRESS;
			IP_ADDRESS++;
			l ++;
		}
		IP_ADDRESS++;
		l=0;
		printf("%s", ipp);
		ff->IP_ADDRESS = char_to_ip(ipp);
		ff->next = NULL;
		i++;
		f->next = ff;
		f = ff;
	}
}

// void Insert_ips1(struct node *N, char *ips[], int lim)
// {
// 	struct ip* new_ip = (struct ip*) malloc(sizeof(struct ip));
// 	new_ip->IP_ADDRESS = char_to_ip(ips[0]);
// 	int i = 0;
// 	new_ip->next = NULL;
// 	N->ips = new_ip;

// 	struct ip *f=N->ips;
// 	i++;
// 	while(i < lim)
// 	{
// 		struct ip *ff = (struct ip*) malloc(sizeof(struct ip));
// 		ff->IP_ADDRESS = char_to_ip(ips[i]);
// 		ff->next = NULL;
// 		i++;
// 		f->next = ff;
// 		f = ff;
// 	}
// }

void INIT_NODE(struct node *N, char *IP_ADDRESS, char *CHECKSUM, int lim)
{
	printf("\nInitializing Node\n");
	unsigned char i = 0;
	N->right = NULL;
	Insert_ips(N, IP_ADDRESS, lim);
	for(; i < 32 ; i++)
	{
		N->CHECKSUM[i] = ci(CHECKSUM[2*i]);
		N->CHECKSUM[i] <<= 4;
		N->CHECKSUM[i] |= ci(CHECKSUM[2*i+1]);
		// printf("%u ",N->CHECKSUM[i]);
	}
	N->count = 1;
}


// convert 256 bit checksum into 24 bit 
unsigned int _24_bit_checksum(char *CHECKSUM)
{
	unsigned int _24_bit = 0,k;
	unsigned char l = 0;
	int c = 0;
	while(c < 64)
	{
		if( c%2 == 0 && c%8 != 6)
		{
			k=0;
			_24_bit <<= 1;
			l = *CHECKSUM;
			l = l>96?l-87:l-48;
			while(l)
			{
				k += l & 1;
				l >>= 1;
			}
			_24_bit += k%2==0?1:0;
		}
		c++;
		CHECKSUM++;
	}
	return _24_bit;
}

int IS_CHECKSUM_EQUAL(unsigned char * A, unsigned char * B)
{
	int i = 0;
	for(i=0;i < 32;i++)
	{
		if(A[i] != B[i])
			break;
	}
	return i==32?0:1;
}

void _RAM_TEST()
{
	int i = 0,j=0,k=0;
	for(i=0;i<SIZE;i++)
	{
		if(RAM[i] == NULL)
		{
			j++;
		}
		else
		{
			k+=1;
		}
	}
	printf("\nThiss is result : %d %d %d\n",i,j,k);
}

// void init_ips()
// {
// 	int i = 0;
// 	for(i=0;i<100;i++)
// 		ips[i] = 0;
// }

//search ips of server where the file is saved
unsigned int * SEARCH_NODE(char *CHECKSUM)
{
	unsigned char a[32] = {'\0'};
	int i = 0;
	int INDEX_TO_RAM = _24_bit_checksum(CHECKSUM);
	// init_ips();

	for(; i < 32 ; i++)
	{
		a[i] = ci(CHECKSUM[2*i]);
		a[i] <<= 4;
		a[i] |= ci(CHECKSUM[2*i+1]);
	}
	if(RAM[INDEX_TO_RAM] != NULL)
	{
		struct node *ptr = RAM[INDEX_TO_RAM];
		if(IS_CHECKSUM_EQUAL(ptr->CHECKSUM, a)==0){
			return get_ips(ptr->ips);
		}
		else
			printf("\nIPS not found\n");
		while(ptr->right != NULL)
		{
			if(IS_CHECKSUM_EQUAL(ptr->CHECKSUM, a) == 0){
				
				return get_ips(ptr->ips);
			}
			else
				ptr = ptr->right;
		}
	}

	// return retIntPointer();
	return 0;
}

unsigned char DEC_COUNT(char *CHECKSUM)
{
	unsigned char a[32] = {'\0'};
	int i = 0;
	int INDEX_TO_RAM = _24_bit_checksum(CHECKSUM);

	for(; i < 32 ; i++)
	{
		a[i] = ci(CHECKSUM[2*i]);
		a[i] <<= 4;
		a[i] |= ci(CHECKSUM[2*i+1]);
	}
	if(RAM[INDEX_TO_RAM] != NULL)
	{
		struct node *ptr = RAM[INDEX_TO_RAM];
		if(IS_CHECKSUM_EQUAL(ptr->CHECKSUM, a)==0)
		{
			if(ptr->count == 1)
				RAM[INDEX_TO_RAM] = ptr->right;
			else
				ptr->count -= 1;
			return 1;
		}
		while(ptr->right->right != NULL)
		{
			if(IS_CHECKSUM_EQUAL(ptr->right->CHECKSUM, a) == 0)
			{
				if(ptr->right->count == 1)
					ptr->right = ptr->right->right;
				else
					ptr->right->count -= 1;
				return 1;
			}
			else
				ptr = ptr->right;
		}
		if(ptr->right->right == NULL)
		{
			//means last node
			if(IS_CHECKSUM_EQUAL(ptr->right->CHECKSUM, a) == 0)
			{
				if(ptr->right->count == 1)
					ptr->right = NULL;
				else
					ptr->right->count -= 1;
				return 1;
			}
			return 0;
		}
	}
	return 0;
}

int INC_COUNT(char *CHECKSUM)
{
	unsigned char a[32] = {'\0'};
	int i = 0;
	int INDEX_TO_RAM = _24_bit_checksum(CHECKSUM);

	for(; i < 32 ; i++)
	{
		a[i] = ci(CHECKSUM[2*i]);
		a[i] <<= 4;
		a[i] |= ci(CHECKSUM[2*i+1]);
	}
	if(RAM[INDEX_TO_RAM] != NULL)
	{
		struct node *ptr = RAM[INDEX_TO_RAM];
		if(IS_CHECKSUM_EQUAL(ptr->CHECKSUM, a)==0)
		{
			ptr->count += 1;
			return 1;
		}
		while(ptr->right != NULL)
		{
			if(IS_CHECKSUM_EQUAL(ptr->CHECKSUM, a) == 0)
			{
				ptr->count += 1;
				return 1;
			}
			else
				ptr = ptr->right;
		}
	}
	return 0;
}

unsigned int GET_COUNT(char *CHECKSUM)
{
	unsigned char a[32] = {'\0'};
	int i = 0;
	int INDEX_TO_RAM = _24_bit_checksum(CHECKSUM);

	for(; i < 32 ; i++)
	{
		a[i] = ci(CHECKSUM[2*i]);
		a[i] <<= 4;
		a[i] |= ci(CHECKSUM[2*i+1]);
	}
	if(RAM[INDEX_TO_RAM] != NULL)
	{
		struct node *ptr = RAM[INDEX_TO_RAM];
		if(IS_CHECKSUM_EQUAL(ptr->CHECKSUM, a)==0)
			return ptr->count;
		while(ptr->right != NULL)
		{
			if(IS_CHECKSUM_EQUAL(ptr->CHECKSUM, a) == 0)
				return ptr->count;
			else
				ptr = ptr->right;
		}
	}
	return 0;
}

unsigned int * INSERT_NODE(char *IP_ADDRESS, char *CHECKSUM, int lim)
{
	printf("\n%s %s %d",IP_ADDRESS, CHECKSUM, lim);
	// init_ips();
	unsigned int INDEX_TO_RAM = _24_bit_checksum(CHECKSUM);
	struct node *N = (struct node *)malloc(sizeof(struct node));
	INIT_NODE(N, IP_ADDRESS, CHECKSUM, lim);
	
	// Check if node is already present at head 
	// if yes add node to the right if no add to head in hashtable RAM[]
	if(RAM[INDEX_TO_RAM] == NULL)
	{
		printf("\nips: %u",*get_ips(N->ips));
		printf("\nRAM is EMPTY @ %d",INDEX_TO_RAM);
		RAM[INDEX_TO_RAM] = N;
		printf("\nHead Node \"%s\" is inserted into RAM",_into_string(N->CHECKSUM));
	}
	else
	{
		printf("\nRAM IS NOT NULL @ %d",INDEX_TO_RAM);
		struct node *ptr = RAM[INDEX_TO_RAM];
		if(IS_CHECKSUM_EQUAL(ptr->CHECKSUM, N->CHECKSUM)==0)
		{
			ptr->count += 1;
			return get_ips(ptr->ips);//ptr->ips->IP_ADDRESS;
		}
		while(ptr->right != NULL)
		{
			if(IS_CHECKSUM_EQUAL(ptr->CHECKSUM, N->CHECKSUM) == 0)
			{
				ptr->count += 1;
				return get_ips(ptr->ips);//ptr->ips->IP_ADDRESS;
			}
			else
				ptr = ptr->right;
		}
		ptr->right = N;
	}
	
	return 0;
	// return retIntPointer();
}



void print_node(int indx)
{
	if(RAM[indx] == 0)
		printf("\nThis Index is Empty .");
	else
	{
		struct node* temp = RAM[indx];
		printf("\n");
		while(temp!=NULL)
		{
			printf("--%s ,",_into_string(temp->CHECKSUM));
			struct ip *pp = temp->ips;
			while(pp != NULL)
			{
				_print(pp->IP_ADDRESS);

				pp = pp->next;
			}
			temp=temp->right;
		}
	}
}

int main() {
	//ii();
	
	char ips[1000] = "192.168.1.1.*192.56.10.10.*192.168.56.51.*";
	// char checksum[70] = "1234567812345678123456781234567812345678123456781234567812345678";
	// char ips[1000] = "192.168.1.102.*192.168.1.103.*192.168.1.104.*";
	// char checksum[70] = "841f449431f3cf84c2e4d458ae9f4dac28ce0908427eda55d579dba0bbed0af7";
	char checksum[70] = "5917b04920f8664d692ac4c8dcd397cfa4522fc1a8eb7727e3403dd150db1296";
	int ipCount = 3;

	printf("\nInserting a checksum...");
	INSERT_NODE(ips,checksum,ipCount);
	
	int i =0;
	i = INC_COUNT(checksum);
	printf("\nIncreased count status is %d", i);
	i = DEC_COUNT(checksum);
	printf("\nDecreased count status is %d", i);
	i = GET_COUNT(checksum);
	printf("\nCurrent count is %d\n", i);

	unsigned int *b = SEARCH_NODE(checksum);
	printf("\nPrinting ips for: %u\n", *b);
	_print_ips(b);
	printf("\n\nEnd of ips\n");

	return 0;
}