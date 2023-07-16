#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Node_book
{
	char name[20];
	char subject[30];
	char author_name[40]; //surname first
	int accession_no;
	int year_of_publish; //eg:2001

	int available; //1 if available else 0

	struct Node_book *next;
	int hall_no, rack_no, row_no; //if not available then -1 is the location
	int extra_days;
} **head;

struct Node_member
{
	char member_name[20];
	char dept[20];
	char designation[20]; //"member or faculty
	int member_id;
	int no_of_books_issued;

	struct Node_member *next;
	struct Node_book *book;
} **head_faculty , **head_student ;

struct Node_book *Divide(struct Node_book *lptr);

struct Node_book *Merge(struct Node_book *lptr, struct Node_book *nptr);

struct Node_book *MergeSort(struct Node_book *lptr);
//insertion


void insert_book()
{

	struct Node_book *b = (struct Node_book *)malloc(sizeof(struct Node_book)); //	struct Node_book*b=(struct Node_book*)malloc(sizeof(struct Node_book))
	
	printf("Enter name of book\n");
	fflush(stdin);
	gets(b->name);
	printf("enter subject of book\n");
	gets(b->subject);
	printf("enter name of author\n");
	gets(b->author_name);
	printf("enter accession no.\n");
	scanf("%d", b->accession_no);
	printf("enter year of publish\n");
	scanf("%d", b->year_of_publish);

	//2 conditions are to be checked: book available earlier or not
	int key;
	printf("Enter 0 if this book wasnt available earlier or enter 1 if this is a new copy of already available book");
	scanf("%d", key);

	if (key == 0)
	{
		//now book is available since we are inserting
		b->available = 1;
		//for convienience lets add at 1st position only and then we will sort
		b->next = *head;
		*head = b;
		struct Node_book *temp;
		temp = *head;
		temp = MergeSort(temp);
		*head = temp;
	}

	else if (key == 1)
	{ //	struct Node_book*p=*head,*q=NULL;
		//if copy is already present then that means all books would be in sorted order only so we just have to keep it next to its copy
		struct Node_book *p = *head, *q = NULL;
		while (p->name != b->name)
		{
			q = p;
			p = p->next;
		}
		q->next = b;
		b->next = p;
		struct Node_book *temp;
		temp = *head;
		temp = MergeSort(temp);
		*head = temp;
	}
}

void create_faculty_member() //either faculty or student
{
	struct Node_member *m = (struct Node_member *)malloc(sizeof(struct Node_member)); //(struct Node_member*)malloc(sizeof(struct Node_member))
	printf("enter name of member\n");
	fflush(stdin);
	gets(m->member_name);
	printf("enter dept of member\n");
	gets(m->dept);

	strcpy(m->designation, "Faculty");

	printf("enter member's id:");
	scanf("%d", m->member_id);
	m->no_of_books_issued = 0;

	if (*head_faculty == NULL)
	{
		*head_faculty = m;
		m->next = NULL;
	}
	else
	{
		struct Node_member *p = *head_faculty, *q = NULL;
		while (p)
		{
			q = p;
			p = p->next;
		}
		q->next = m;
		m->next = NULL;
	}
}
void create_student_member() //no need to pass any parameter as we already made ptrs to head ptrs as global
{

	struct Node_member *m = (struct Node_member *)malloc(sizeof(struct Node_member));
	strcpy(m->designation, "Student");

	printf("enter name of member\n");
	fflush(stdin);
	gets(m->member_name);
	printf("enter dept of member\n");
	gets(m->dept);

	printf("enter student's member id:");
	scanf("%d", m->member_id);
	m->no_of_books_issued = 0;

	if (*head_student == NULL)
	{
		*head_student = m;
		m->next = NULL;
	}
	else
	{
		struct Node_member *p = *head_student, *q = NULL;
		while (p)
		{
			q = p;
			p = p->next;
		}
		q->next = m;
		m->next = NULL;
	}
}

void display_max_books_issued_faculty()
{

	if (*head_faculty == NULL)
	{
		printf("No faculty has enrolled in library\n");
	}
	else
	{
		struct Node_member *p = *head_faculty, *q = NULL;
		int max = (*head_faculty)->no_of_books_issued;

		while (p)
		{
			if (p->no_of_books_issued >= max)
			{
				max = p->no_of_books_issued;
			}
			p = p->next;
		}
		//more than 1 person might have max no. of books issued
		p = *head_faculty;
		while (p)
		{
			if (p->no_of_books_issued == max)
				printf("%s", p->member_name);
		}
	}
}

void display_max_books_issued_students()
{

	if (*head_student == NULL)
	{
		printf("No student has enrolled in library\n");
	}
	else
	{
		struct Node_member *p = *head_faculty, *q = NULL;
		int max = (*head_student)->no_of_books_issued;
		char a;
		while (p)
		{
			if (p->no_of_books_issued >= max)
			{
				max = p->no_of_books_issued;
			}
			p = p->next;
		}
		p = *head_student;
		printf("%s", p->member_name);
	}
}

void cal_fine()
{
	struct Node_member *p = *head_student;
	int maxfine = 0;
	//calculating maxfine
	while (p)
	{
		int curr_fine = 0;
		while (p->book)
		{
			if ((p->book)->extra_days != 0)
				curr_fine += 50 * ((p->book)->extra_days);
			p->book = p->book->next;
		}
		if (curr_fine >= maxfine)
			maxfine = curr_fine;
		p = p->next;
	}
	p = *head_student;
	//printing student with maxfine
	while (p)
	{
		int curr_fine = 0;
		while (p->book)
		{
			if (p->book->extra_days != 0)
				curr_fine += 50 * p->book->extra_days;
			p->book = p->book->next;
		}
		if (curr_fine == maxfine)
			printf("%s has maximum fine\n", p->member_name);
		p = p->next;
	}
	//faculty
	struct Node_member *q = *head_faculty;
	int maxfine2 = 0;
	//calculating maxfine
	while (q)
	{
		int curr_fine = 0;
		while (q->book)
		{
			if (q->book->extra_days != 0)
				curr_fine += 50 * q->book->extra_days;
			q->book = q->book->next;
		}
		if (curr_fine >= maxfine)
			maxfine2 = curr_fine;
		q = q->next;
	}
	//printing student with maxfine
	while (q)
	{
		int curr_fine = 0;
		while (q->book)
		{
			if (q->book->extra_days != 0)
				curr_fine += 50 * q->book->extra_days;
			q->book = q->book->next;
		}
		if (curr_fine == maxfine2)
			printf("%s has maximum fine\n", q->member_name);
		q = q->next;
	}
}

void issue(struct Node_member *m)
{
	printf("Are u a Student or faculty\n");
	fflush(stdin);
	char a[20];
	gets(a);
	strcpy(m->designation, a);

	if (strcmp(a,"Student")==0)
		
	{
		printf("How many issued books u have currently\n NOTE: U can never issue more than 2 books");
		scanf("%d", m->no_of_books_issued);

		if (m->no_of_books_issued == 2)
			printf("sorry u can't issue book");
		else
		{
			printf("please enter details of book to be issued\n ");
			struct Node_book *b = (struct Node_book *)malloc(sizeof(struct Node_book));
			printf("Enter name of book");
			fflush(stdin);
			gets(b->name);
			printf("enter subject of book\n");
			gets(b->subject);
			printf("enter name of author\n");
			gets(b->author_name);
			printf("enter accession no.(written on last page of book\n");
			scanf("%d", b->accession_no);
			printf("enter year of publish\n");
			scanf("%d", b->year_of_publish);

			printf("enter hall no");
			scanf("%d", b->hall_no);
			printf("enter rack no");
			scanf("%d", b->rack_no);
			printf("enter row no");
			scanf("%d", b->row_no);

			if (m->no_of_books_issued == 0)
			{
				m->book = b;
				m->no_of_books_issued++;
				m->book = m->book->next;
			}
			else if (m->no_of_books_issued == 1)
			{
				//set various details for the book already issued

				m->book = b;
				m->book->next = NULL;
			}
			printf("Book is successfully issued \n");
		}
		//now since we even have to display the days left for renewal of books
		int a;
		printf("Do u want to know the days left for renewal or return\n if yes press1 else 0");
		scanf("%d", a);
		if (a == 1)
		{
			printf("how many days back u issued the book\n");
			int x;
			scanf("%d", x);
			if (x <= 14)
			{
				printf("You have %d days left for renewal or return", 14 - x);
			}
			else
			{
				printf("you kept book for more than 14 days and hence you will be fined for %days\n", x - 14);
			}
		}
	}
	if (strcmp(a ,"Faculty")==0)
	{
		printf("How many issued books u have currently\n NOTE: U can never issue more than 4 books");
		
		scanf("%d", m->no_of_books_issued);

		if (m->no_of_books_issued == 4)
			printf("sorry u can't issue book");
		else
		{
			printf("please enter details of book to be issued\n ");
			struct Node_book *b = (struct Node_book *)malloc(sizeof(struct Node_book));
			printf("Enter name of book");
			fflush(stdin);
			gets(b->name);
			printf("enter subject of book\n");
			gets(b->subject);
			printf("enter name of author\n");
			gets(b->author_name);
			printf("enter accession no.(written on last page of book\n");
			scanf("%d", b->accession_no);
			printf("enter year of publish\n");
			scanf("%d", b->year_of_publish);

			printf("enter hall no");
			scanf("%d", b->hall_no);
			printf("enter rack no");
			scanf("%d", b->rack_no);
			printf("enter row no");
			scanf("%d", b->row_no);

			if (m->no_of_books_issued >= 0 && m->no_of_books_issued < 3)
			{
				m->book = b;
				m->no_of_books_issued++;
				m->book = m->book->next;
			}

			else if (m->no_of_books_issued == 3)
			{
				m->book = b;
				m->book->next = NULL;
				m->no_of_books_issued++;
			}
		}
		printf("Book issued successfully\n");
		//now since we even have to display the days left for renewal of books
		int a;
		printf("Do u want to know the days left for renewal or return\n if yes press1 else 0");
		scanf("%d", a);
		if (a == 1)
		{
			printf("how many days back u issued the book\n");
			int x;
			int days_left;
			scanf("%d", x);
			if (x <= 30)
			{

				days_left = 30 - x;
				printf("you only have %d days left for renewal\n", days_left);
			}
			else
			{

				printf("you will be imposed fine for %d days\n", x - 30);
			}
		}
	}
}

void book_available()
{
	int KEY;
	printf("u want to search by title or author's name\n");
	printf("press 1 for title\npress 2 for author's name");
	scanf("%d", KEY);

	if (KEY == 1)
	{
		char search[30];
		printf("enter the title");
		fflush(stdin);
		gets(search);

		struct Node_book *p = *head;
		if (p == NULL)
			printf("Book not available");
		else
		{
			static int count = 0;
			while (p)
			{
				if (p->name == search)
				{
					count++;
					printf("book is available\n");

					printf("%s\n", p->name);
					printf("%s\n", p->subject);
					printf("%s\n", p->author_name);
					printf("%d\n", p->accession_no);
					printf("%d\n", p->year_of_publish);
					if (p->available == 1)
						printf("available\n");
					else
						printf("issued\n");
				}
				p = p->next;
			}
			if (count > 0)
				printf("%d copies are present\n", count);
			else
				printf("no copies are present\n");
		}
	}
	else if (KEY == 2)
	{
		char search[30];
		printf("enter the author's name\n");
		fflush(stdin);
		gets(search);

		struct Node_book *p = *head;
		if (p == NULL)
			printf("Book not available");
		else
		{
			int flag = 0;
			while (p)
			{
				if (p->author_name == search)
				{
					flag++;
					printf("%s\n", p->name);
				}
				p = p->next;
			}
			if (flag == 0)
				printf("book not found\n");
		}
	}
}

void sort_and_display_max_issued()
 {	
 	struct Node_member *p1=*head_faculty;
 	struct Node_member *p2=*head_student;
 	struct Node_member*p=(struct Node_member*)malloc(sizeof(struct Node_member));
 	 
	  struct Node_member*temp=p;
	  while(p1&&p2)
 	{
 		 if(p1->no_of_books_issued>p2->no_of_books_issued)
        {
            p->next=p1;
            p1=p1->next;
        }
        else if(p1->no_of_books_issued<p2->no_of_books_issued)
        {
            p->next=p2;
            p2=p2->next;
            
        }
        else
        {
         p->next=p1;//since p1 is professor
         p1=p1->next;
    
        }
        p=p->next;
 	}
	 while(p1)
    {
        p->next=p1;
        p1=p1->next;
        p=p->next;
    }
    while(p2)
    {
        p->next=p2;
        p2=p2->next;
        p=p->next;
    }
 		  temp=temp->next;
    	while(temp)
    	{
    		printf("%s ",temp->member_name);
    		temp=temp->next;
    	}
    printf("\n");
 }

struct Node_book *Divide(struct Node_book *lptr)
{
	struct Node_book *slow, *fast, *prev;
	slow = fast = lptr;
	while (fast != NULL && fast->next != NULL)
	{
		prev = slow;
		slow = slow->next;
		fast = fast->next->next;
	}
	prev->next = NULL;
	return slow;
}
struct Node_book *Merge(struct Node_book *lptr, struct Node_book *nptr)
{
	struct Node_book *head, *ptr;
	if (strcmp(lptr->subject, nptr->subject) > 0)
	{
		head = nptr;
		nptr = nptr->next;
	}
	else if (strcmp(lptr->subject, nptr->subject) == 0)
	{
		if (strcmp(lptr->name, nptr->name) > 0)
		{
			head = nptr;
			nptr = nptr->next;
		}
		else if (strcmp(lptr->name, nptr->name) == 0)
		{
			if (strcmp(lptr->author_name, nptr->author_name) > 0)
			{
				head = nptr;
				nptr = nptr->next;
			}
			else if (strcmp(lptr->author_name, nptr->author_name) == 0)
			{
				if (lptr->year_of_publish > nptr->year_of_publish)
				{
					head = nptr;
					nptr = nptr->next;
				}
				else
				{
					head = lptr;
					lptr = lptr->next;
				}
			}
			else
			{
				head = lptr;
				lptr = lptr->next;
			}
		}
		else
		{
			head = lptr;
			lptr = lptr->next;
		}
	}
	else
	{
		head = lptr;
		lptr = lptr->next;
	}
	ptr = head;
	while (nptr != NULL && lptr != NULL)
	{
		if (strcmp(lptr->subject, nptr->subject) > 0)
		{
			ptr->next = nptr;
			nptr = nptr->next;
		}
		else if (strcmp(lptr->subject, nptr->subject) == 0)
		{
			if (strcmp(lptr->name, nptr->name) > 0)
			{
				ptr->next = nptr;
				nptr = nptr->next;
			}
			else if (strcmp(lptr->name, nptr->name) == 0)
			{
				if (strcmp(lptr->author_name, nptr->author_name) > 0)
				{
					ptr->next = nptr;
					nptr = nptr->next;
				}
				else if (strcmp(lptr->author_name, nptr->author_name) == 0)
				{
					if (lptr->year_of_publish > nptr->year_of_publish)
					{
						ptr->next = nptr;
						nptr = nptr->next;
					}
					else
					{
						ptr->next = lptr;
						lptr = lptr->next;
					}
				}
				else
				{
					ptr->next = lptr;
					lptr = lptr->next;
				}
			}
			else
			{
				ptr->next = lptr;
				lptr = lptr->next;
			}
		}
		else
		{
			ptr->next = lptr;
			lptr = lptr->next;
		}
		ptr = ptr->next;
	}
	if (lptr != NULL)
		ptr->next = lptr;
	else
		ptr->next = nptr;
	return head;
}
struct Node_book *MergeSort(struct Node_book *lptr)
{
	struct Node_book *nptr;
	if (lptr != NULL && lptr->next != NULL)
	{
		nptr = Divide(lptr);
		lptr = MergeSort(lptr);
		nptr = MergeSort(nptr);
		lptr = Merge(lptr, nptr);
	}
	return lptr;
}


int main()
{		head = NULL;
	head_faculty = NULL;
	head_student = NULL;
	
		printf("\n\n\t\t\t\t\tDSPD ASSIGNMENT 2 - LIBRARY SYSTEM\n\n");
   printf("\t\t\t\t\t-----------------------------------------\n");
   printf("\t\t\t\t\t|\t Name: Manglam Bhargava\t\t|\n");
   printf("\t\t\t\t\t|\t Roll-No: BT19CSE062\t\t|\n");
   printf("\t\t\t\t\t|\t Submission Date: 21-03-2021\t|\n");
   printf("\t\t\t\t\t-----------------------------------------\n\n\n\n");
					
		
	printf("1.Insert a book\n2.Create new faculty account\n3.Create new student account\n4.Display names of faculty who borrowed max books\n5.Display names of students who borrowed max books\n6.Display name of persons having maximum fines.\n7.Display names after sorting\n8.Exit\n");

	printf("choose the serial number you want to perform\n");

	while (1)
	{
		int choice;
		scanf("%d", &choice);
		if (choice == 1)
		{
			insert_book();
		}
		if (choice == 2)
		{
			create_faculty_member();
		}
		if (choice == 3)
		{
			create_student_member();
		}
		if (choice == 4)
		{
			display_max_books_issued_faculty();
		}
		if (choice == 5)
		{
			display_max_books_issued_students();
		}
		if (choice == 6)
		{
			cal_fine();
		}
		if (choice == 7)
		{
			sort_and_display_max_issued();
		}
		if (choice == 8)
		{
			break;
		}
	}
	return 0;
}
