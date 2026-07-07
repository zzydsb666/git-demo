#include <stdio.h>
typedef struct Student{
    int id;
    char name[20];
}Stu;

int main(){
    Stu s1 = {202601, "学生赵张三"};
    printf("学号：%d，姓名：%s",s1.id,s1.name);
    return 0;
}