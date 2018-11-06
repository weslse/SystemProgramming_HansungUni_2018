#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student
{
	int rollno;
	char name[50];
	int p_marks, c_marks;
	double per;
	char grade;
}st;

int fd;

void write_student()
{
	fd = open("student.dat", O_RDWR|O_CREAT, 0644);
	while ((read(fd, &st, sizeof(st))) > 0)
	{
		if (st.rollno == -100){
			lseek(fd, -(long)sizeof(st), SEEK_CUR);
			break;
		}
	}
	printf("\nPlease Enter The New Details of student \n");
	printf("\nEnter The roll number of student ");
	scanf(" %d", &st.rollno);
	getchar();
	printf("\nEnter The Name of student ");
	fgets(st.name, sizeof(st.name), stdin);
	st.name[strlen(st.name) - 1] = '\0';
	printf("\nEnter The marks in physics out of 100 : ");
	scanf(" %d", &st.p_marks);
	printf("\nEnter The marks in chemistry out of 100 : ");
	scanf(" %d", &st.c_marks);

	st.per = (st.p_marks + st.c_marks) / 2.0;
	if (st.per >= 60)
		st.grade = 'A';
	else if (st.per >=50 && st.per < 60)
		st.grade = 'B';
	else if (st.per >= 33 && st.per < 50)
		st.grade = 'C';
	else
		st.grade = 'F';

	//lseek(fd, 0, SEEK_END);
	write(fd, &st, sizeof(st));
	close(fd);
	printf("\n\nStudent Record Has Been Created. Press any key.... ");
	getchar();
	getchar();
}

void display_all()
{	
	system("clear");
	printf("\n\n\n\n\t\tDISPLAY ALL RECORD !!!\n\n");
	printf("===========================================================\n");
	printf("R.No.    Name     P   C   Ave    Grade\n");
	printf("===========================================================\n");

	fd = open("student.dat", O_RDONLY);
	if (fd == -1)
		return;
	
	while ((read(fd, &st, sizeof(st))) > 0)
	{
		if (st.rollno == -100)
			continue;
		printf("%-6d %-10s %-3d %-3d %-3.2f  %-1c\n",
		st.rollno, st.name, st.p_marks, st.c_marks, st.per, st.grade);
	}
	close(fd);
	getchar();
}

void display_sp(int n)
{
	int flag = 0;
	fd = open("student.dat", O_RDONLY);
	if (fd == -1)
		return;
	while ((read(fd, &st, sizeof(st))) > 0)
	{
		if (st.rollno == -100)
			break;
		if (st.rollno == n)
		{
			system("clear");
			printf("\nRoll number of student : %d", st.rollno);
			printf("\nName of student : %s", st.name);
			printf("\nMarks in Physics : %d", st.p_marks);
			printf("\nMarks in Chemistry : %d", st.c_marks);
			printf("\nPercentage of student is : %.2f", st.per);
			printf("\nGrade of student is : %c", st.grade);
			printf("\n\n");
			flag = 1;
		}
	}
	close(fd);
	if (flag == 0)
		printf("\n\nrecord not exist");
	getchar();
	getchar();

}

void modify_student()
{
	int no, found = 0;
	system("clear");
	printf("\n\n\tTo Modify ");
	printf("\n\n\tPlease Enter The roll number of student >> ");
	scanf(" %d", &no);
	fd = open("student.dat", O_RDWR);
	if (fd == -1)
		return;
	while ((read(fd, &st, sizeof(st))) > 0 && found == 0)
	{
		if(no == -100)
			break;
		if (st.rollno == no) {
			printf("\nRoll number of student : %d", st.rollno);
			printf("\nName of student : %s", st.name);
			printf("\nMarks in Physics : %d", st.p_marks);
			printf("\nMarks in Chemistry : %d", st.c_marks);
			printf("\nPercentage of student is : %.2f", st.per);
			printf("\nGrade of student is : %c", st.grade);
			printf("\nPlease Enter The New Details of student \n");
			printf("\nEnter The roll number of student ");
			scanf(" %d", &st.rollno);
			getchar();
			printf("\n\nEnter The Name of student ");
			fgets(st.name, sizeof(st.name), stdin);
			st.name[strlen(st.name) - 1] = '\0';
			printf("\nEnter The marks in physics out of 100 : ");
			scanf(" %d", &st.p_marks);
			printf("\nEnter The marks in chemistry out of 100 : ");
			scanf(" %d", &st.c_marks);

			st.per = (st.p_marks + st.c_marks) / 2.0;
			if (st.per >= 60)
				st.grade = 'A';
			else if (st.per >= 50 && st.per < 60)
				st.grade = 'B';
			else if (st.per >= 33 & st.per < 50)
				st.grade = 'C';
			else
				st.grade = 'F';
		
			lseek(fd, -(long)sizeof(st), SEEK_CUR);
			write(fd, &st, sizeof(st));
	
			printf("\n\n\t Record Updated");
			found = 1;
		}
	}
	close(fd);
	if (found == 0)
		printf("\n\n Record Not Found ");
	getchar();
}

void delete_student()
{
	int no;
	int found = 0;
	system("clear");
	printf("\n\n\n\tDelete Record");
	printf("\n\nPlease Enter The roll number of student You Want To Delete >>");
	scanf(" %d", &no);
	
	fd = open("student.dat", O_RDWR);
	if (fd == -1)
		return;	

	while((read(fd, &st, sizeof(st)) > 0) && found == 0 )
	{
		if(st.rollno == no)
		{
			st.rollno = -100;
			found = 1;
			lseek(fd, -(long)sizeof(st), SEEK_CUR);
			write(fd, &st, sizeof(st));
			printf("\n\n\tRecord Deleted .. ");
			break;
		}
	}

	close(fd);
	if (found == 0)
		printf("\n\n Record Not Found ");		
	getchar();
	getchar();
}

int main() 
{
	char ch;
	int num;
	do
	{
		system("clear");	
		printf("\n\n\t1.CREATE STUDENT RECORD");
		printf("\n\n\t2.DISPLAY ALL STUDENTS RECORDS");
		printf("\n\n\t3.SEARCH STUDENT RECORD");
		printf("\n\n\t4.MODIFY STUDENT RECORD");
		printf("\n\n\t5.DELETE STUDENT RECORD");
		printf("\n\n\t6.exit");
		printf("\n\n\tPlease Enter Your Choice (1-6) ");
		ch = getchar();
		getchar();

		switch(ch) 
		{
		case '1': system("clear");
			write_student();
			break;
		case '2': display_all();
			break;
		case '3': system("clear");
			printf("\n\n\tPlease Enter The roll number ");
			scanf(" %d", &num);
			display_sp(num);
			break;
		case '4': modify_student(); break;
		case '5': delete_student(); break;
		case '6': break;
		default: printf("\a");
		}
	} while (ch != '6');
}
