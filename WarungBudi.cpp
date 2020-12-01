#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

void clearScreen(){
	for(int i=0;i<30;i++){
		printf("\n");
	}
	return;
}

const char* detectOS() {
  #ifdef _WIN32
  return "Windows 32-bit";
  #elif _WIN64
  return "Windows 64-bit";
  #elif __APPLE__ || __MACH__
  return "Mac OSX";
  #elif __linux__
  return "Linux";
  #elif __FreeBSD__
  return "FreeBSD";
  #elif __unix || __unix__
  return "Unix";
  #else
  return "Other";
  #endif
}

struct dish {
	char name[100];
	int price;
    char priceStr[100];
	int quantity;
	struct dish *next, *prev;
}*head = NULL, *tail = NULL;

struct dish* createDish(char *name, int price, int qty){
	struct dish *node = (struct dish*)malloc(sizeof(struct dish));
    char Strprice[100];
    Strprice[0] = 'R';
    Strprice[1] = 'p';
    Strprice[2] = '.';
    Strprice[3] = '\0';
    char str[100];
    int temp = price, lenTemp = 1;
    while(temp>9){
        temp/=10;
        lenTemp++;
    }
    int temp1 = price;
    for(int i=lenTemp-1;i>0;i--){
        str[i] = (temp1%10)+'0';
        temp1/=10;
    }
    str[0] = temp1+'0';
    str[lenTemp] = '\0';
    //printf("%s\n", str);
    strcat(Strprice, str);
    Strprice[lenTemp+3] = '\0';
    //printf("%s\n", Strprice);
	strcpy(node->name, name);
	strcpy(node->priceStr, Strprice);
	node->price = price;
	node->quantity = qty;
	node->prev = NULL;
	node->next = NULL;
	return node;
}

void pushDish(char *name, int price, int qty){
	struct dish * node = createDish(name, price, qty);
	if(head==NULL){
		head = tail = node;
	}
	else if(strcmp(name,head->name)<0){
		node -> next = head;
		head -> prev = node;
		head = node;
	}
	else if(strcmp(name,tail->name)>0){
		node->prev = tail;
		tail->next = node;
		tail = node;
	}
	else{
		struct dish *curr = head;
		while(curr->next!=NULL && strcmp(curr->next->name,name)<0){
			curr = curr->next;
		}
		node->prev = curr;
		node->next = curr->next;
		curr->next->prev = node;
		curr->next = node;
	}
}

int popDish(char *name){
    struct dish *curr = head;
	if(head==NULL){
		return -1;
	}
	else if(head==tail){
		free(curr);
		head=tail=NULL;
	}
	else if(strcmp(name, head->name)==0){
		head = head->next;
		free(curr);
		head->prev = NULL;
	}
	else if(strcmp(name,tail->name)==0){
		free(tail);
		tail = tail->prev;
		tail->next = NULL;
	}
	else{
		while(curr->next!=NULL && strcmp(curr->next->name,name)!=0){
			curr = curr->next;
		}
		if(curr == tail){
			return -1;
		}
		free(curr->next);
		curr->next->next->prev = curr;
		curr->next = curr->next->next;
		return 1;
	}
}

struct customer{
	char name[100];
	struct dish *bookedHead;
    struct dish *bookedTail;
	struct customer *next;
}*table[26]={NULL};

unsigned long DJB2(char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;
  return hash % 26;
}

void pushBook(struct customer *cust, char *name, int qty, int price){
	struct dish * node = createDish(name, price, qty);
	if(cust->bookedHead==NULL){
		cust->bookedHead = cust->bookedTail = node;
	}
	else if(strcmp(name,cust->bookedHead->name)<0){
		node -> next = cust->bookedHead;
		cust->bookedHead -> prev = node;
		cust->bookedHead = node;
	}
	else if(strcmp(name,cust->bookedTail->name)>0){
		node->prev = cust->bookedTail;
		cust->bookedTail->next = node;
		cust->bookedTail = node;
	}
	else{
		struct dish *curr1 = cust->bookedHead;
		while(curr1->next!=NULL && strcmp(curr1->next->name,name)<0){
			curr1 = curr1->next;
		}
		node->prev = curr1;
		node->next = curr1->next;
		curr1->next->prev = node;
		curr1->next = node;
	}
}
void insert(char *str){
	struct customer *node = (struct customer*)malloc(sizeof(struct customer));
	strcpy(node->name, str);
	node->next = NULL;
	node->bookedHead = NULL;
	node->bookedTail = NULL;
	int index = DJB2(str);
	if(table[index]==NULL){
		table[index] = node;
	}
	else{
		struct customer *curr = table[index];
		while(curr->next!=NULL){
			curr = curr->next;
		}
		curr->next = node;
	}
}

void AddDish(){
	clearScreen();
	//getchar();
	char dishName[100];
    int price, quantity, addFlag = 1;
    do{
        addFlag = 1;
        printf(" Insert the name of the dish [lowercase letters]: ");
        scanf("%[^\n]", dishName);
        getchar();
        int len = strlen(dishName);
        for(int i=0;i<len;i++){
            if(dishName[i]!=' ' && !(dishName[i]>='a' && dishName[i]<='z')){
                addFlag = 0;
                break;
            }
        }
        struct dish *curr = head;
        while(curr!=NULL){
            if(strcmp(curr->name,dishName)==0){
                addFlag = 0;
                break;
            }
            curr = curr->next;
        }
    }while(addFlag==0);
    do{
        printf(" Insert the price of the dish [1000...50000]: ");
        scanf("%d", &price);
        getchar();
    }while(price<1000 || price>50000);
    do{
        printf(" Insert the quantity of the dish [1...999]: ");
        scanf("%d", &quantity);
        getchar();
    }while(quantity<=0 || quantity>=1000);
    pushDish(dishName, price, quantity);
    printf(" The dish has been added!\n");
    printf(" Please enter to continue..");
    getchar();
    return;
}

void RemoveDish(){
	clearScreen();
    struct dish *curr = head;
    if(curr == NULL){
        return;
    }
	printf("               Bude's Menu\n");
    printf(" ========================================= \n");
    printf("  No.        Name        Quantity   Price\n");
    int index = 1;
    while(curr!=NULL){
    	int len = strlen(curr->name), lenP = strlen(curr->priceStr);
		int length = 20-len;
		if(length%2!=0){
			length/=2;
			length+=1;
		}
		else{
			length/=2;
		}
		char str[100] = "";
        printf("  %2d. %*s%*s%03d      %*s\n", index, ((20-len)/2)+len, curr->name, length, str, curr->quantity, ((8-lenP)>>1)+lenP, curr->priceStr);
        curr = curr->next;
        index++;
    }
    printf(" ========================================= \n");
    char sellName[100];
    printf(" Insert dish name to be deleted: ");
    scanf("%[^\n]", sellName);
    getchar();
    int buang = popDish(sellName);
    if(buang == 1){
    	printf(" The dish has been removed!\n");
		printf(" Press enter to continue..");
		getchar();		
	}
    
    return;
}

void AddCustomer(){
	clearScreen();
	//getchar();
	char customerName[100];
	int add_flag = 1;
	do{
		add_flag = 1;
		printf(" Insert the customer name [Without space]: ");
		scanf("%[^\n]", customerName);
		getchar();
		int len = strlen(customerName);
		for(int i=0;i<len;i++){
			if(customerName[i]==' '){
				add_flag = 0;
				break;
			}
		}
	}while(add_flag == 0);
	insert(customerName);
	printf(" Customer has been added!\n");
	printf(" Press enter to continue..");
	getchar();
	return;
}

int search(char *cust){
	unsigned long i = DJB2(cust);
	if(table[i]!=NULL){
		struct customer * curr = table[i];
		while(curr!=NULL && strcmp(curr->name,cust)!=0){
			curr = curr->next;
		}
		if(curr!=NULL){
			return i;
		}
		else return -1;
	}	
	return -1;
}

void SearchCustomer(){
	clearScreen();
	char find[100];
	printf(" Insert the customer's name to be searched: ");
	scanf("%[^\n]", find);
	getchar();
	int found = search(find);
	if(found==-1){
		printf(" %s is not present\n", find);
	}
	else{
		printf(" %d. %s\n", found+1, find);
	}
	printf(" Press enter to continue..");
	getchar();
	return;
}

int crowded(){
	for(int i=0;i<26;i++){
		if(table[i]!=NULL){
			return 1;
		}
	}
	return -1;
}
void ViewWarteg(){
	clearScreen();
	int cek =crowded();
	if(cek == -1){
		return;
	}
	printf("Customer List\n");
	for(int i=0;i<26;i++){
		if(table[i]!=NULL){
			struct customer *temp1 = table[i];
			printf("%d. ", i);
			while(temp1!=NULL){
				printf("%s ", temp1->name);
				temp1 = temp1->next;
				if(temp1!=NULL){
					printf("-> ");
				}
			}
			printf("\n");
		}
	}
	printf("Press enter to continue..");
	getchar();
	return;
}

void Order(){
	clearScreen();
	int cek =crowded();
	if(cek == -1){
		printf(" There is no customer!\n");
		printf(" Press enter to continue..");
		getchar();
		return;
	}
	char name[100];
	int find;
	do{
		printf(" Insert the customer's name: ");
		scanf("%[^\n]", name);
		getchar();
		find = search(name);
	}while(find==-1);
	unsigned long search = DJB2(name);
	if(table[search]!=NULL){
		struct customer * curr = table[search];
		while(curr!=NULL && strcmp(curr->name,name)!=0){
			curr = curr->next;
		}
		if(curr!=NULL){
			if(head==NULL){
				printf(" There are no food available!\n");
				printf(" Press enter to continue..");
				getchar();
				return;
			}
			int pesanan;
			printf(" Insert the amount of dish: ");
			scanf("%d", &pesanan);
			getchar();
			for(int i=1;i<=pesanan;i++){
				int quantity, flag = 1;
				char menu[100];
				do{
					flag = 1;
					printf(" [%d] Insert the dish's name and quantity: ", i);
					scanf("%[^x]x%d", menu, &quantity);
					getchar();
					int lenCurr = strlen(menu);
					menu[lenCurr-1] = '\0';
					//printf("%s\n", menu);
					struct dish *temp = head;
					while(temp!=NULL){
						if(strcmp(temp->name, menu)==0){
							break;
						}
						temp = temp->next;
					}
					if(temp==NULL){
						flag = 0;
					}
					else if(quantity>temp->quantity){
						flag = 0;
					}
					else{
						pushBook(curr, menu, quantity, quantity*(temp->price));
						temp->quantity -= quantity;
						if(temp->quantity == 0){
							int buang = popDish(temp->name);
						}
					}
				}while(flag ==0);
			}
			printf(" Order success!\n");
			printf(" Press enter to continue..");
			getchar();
		}
		else return;
	}	
	else return;
	
}

void Payment(){
	clearScreen();
	int cek =crowded();
	if(cek == -1){
		return;
	}
	int index=0;
	do{
		printf("Insert Customer Index: ");
		scanf("%d", &index);
		getchar();
	}while(table[index]==NULL);
	struct customer *payNow = table[index];
	printf("%s\n", payNow->name);
	int i = 1, total = 0;
	struct dish* ordered = payNow->bookedHead;
	while(ordered!=NULL){
		printf("[%d] %s x %d\n", i, ordered->name, ordered->quantity);
		total+=ordered->price;
		ordered = ordered->next;
	}
	printf("Total = Rp.%d\n", total);
	struct customer *throws = table[index];
	table[index] = table[index]->next;
	throws = NULL;
	free(throws);
	printf("Press enter to continue..");
	getchar();
}

void ExitPage(){
	printf("Please expand the terminal to full screen!\n");
	printf("Press enter to continue..");
	getchar();
	char screen[255][255];
	FILE *fp = fopen("splash-screen.txt", "r");
	int index = 0;
	while(fscanf(fp, "%[^\n]\n",screen[index])!=EOF){
		index++;
	}
	fclose(fp);
	for(int i=0;i<index;i++){
		printf("%s\n", screen[i]);
	}
}
int main(){
    char choose;
    do{
        do{
        	time_t t;
    		time(&t);
            clearScreen();
            printf(" System: %s\n", detectOS());
            printf(" %s\n",ctime(&t));
            printf(" 1. Add Dish\n");
            printf(" 2. Remove Dish\n");
            printf(" 3. Add Customer\n");
            printf(" 4. Search Customer\n");
            printf(" 5. View Warteg\n");
            printf(" 6. Order\n");
            printf(" 7. Payment\n");
            printf(" 8. Exit Warteg\n");
            printf(" >> ");
            choose = getchar();
            getchar();
        }while(choose-'0'<1 || choose-'0'>8);
        switch(choose-'0'){
            case 1:
                AddDish();
                break;
            case 2:
                RemoveDish();
                break;
            case 3:
                AddCustomer();
                break;
            case 4:
                SearchCustomer();
                break;
            case 5:
                ViewWarteg();
                break;
            case 6:
                Order();
                break;
            case 7:
                Payment();
                break;
            case 8:
				ExitPage();
                break;
        }
    }while(choose-'0'!=8);
    return 0;
}