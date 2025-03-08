#include <stdio.h>
#include <string.h>

#define MAX_USERS 1000
#define MAX_FAMILIES 100
#define MAX_EXPENSES 1000
#define SUCCESS 1
#define FAILURE 0

int userCount = 0;
int familyCount = 0;
int expenseCount = 0;

struct User {
    int user_id;
    char user_name[100];
    float income;
};

struct Family {
    int family_id;
    char family_name[50];
    int no_members;
    int family_members[4];
    float total_income;
    float total_expense;
};

struct Expense {
    int expense_id;
    int user_id;
    char category[15];
    float amount;
    int date;
};

struct User Users[MAX_USERS];
struct Family Families[MAX_FAMILIES];
struct Expense Expenses[MAX_EXPENSES];

void Initialize_DB_User(int size){
    int i;
    for (i=0 ; i < size ; i++){
        Users[i].user_id = 0;
        Users[i].user_name[0] = '\0';
        Users[i].income = 0;
    }
}

void Initialize_DB_Family(int size){
    int i;
    for (i=0 ; i < size ; i++){
        Families[i].family_id = 0;
        Families[i].family_name[0] = '\0';
        Families[i].no_members = 0;
        for (int j=0 ; j<4 ; j++){
            Families[i].family_members[j] = 0;
        }
        Families[i].total_income = 0;
        Families[i].total_expense = 0;
    }
}

void Initialize_DB_Expense(int size){
    int i;
    for (i=0 ; i < size ; i++){
        Expenses[i].expense_id = 0;
        Expenses[i].user_id = 0;
        Expenses[i].category[0] = '\0';
        Expenses[i].amount = 0;
        Expenses[i].date = 0;
    }
}

int AddUser(){
    int status_code = SUCCESS;
    if(userCount >= MAX_USERS){
        printf("the database the full \n");
        status_code = FAILURE;
    }
    else{
        struct User newuser;
        printf("Enter the user id: ");
        scanf("%d",&newuser.user_id);
        printf("Enter the user name: ");  
        scanf("%s",newuser.user_name);
        printf("Enter the user income: ");
        scanf("%f",&newuser.income);
        int i;
        int found = 0;
        while (i < userCount && newuser.user_id > Users[i].user_id){
            i++;
        }
        for (int j = userCount ; j > i; j--){
            Users[j] = Users[j-1];
        }
        Users[i].user_id = newuser.user_id;
        strcpy(Users[i].user_name,newuser.user_name);
        Users[i].income = newuser.income;
        printf("User added sussesfully\n");
        userCount++;

        int ch;
        printf("Do you wish to add User to an existing family or New Family (0/1): ");
        scanf("%d", &ch);
        if (ch == 0){
            int family_id;
            printf("Enter the Family ID: ");
            scanf("%d", &family_id);
            found = 0;
            for (int i=0 ; i < familyCount && !found; i++){
                if (Families[i].family_id == family_id){
                    found = 1;
                    if (Families[i].no_members < 4){
                        int flag = 0;
                        for (int j=0 ; j<4 && !flag; j++){
                            if (Families[i].family_members[j] == 0){
                                Families[i].family_members[j] = newuser.user_id;
                                flag = 1;
                            }
                        }
                        Families[i].no_members++;
                        Families[i].total_income += newuser.income;
                    }
                    else {
                        printf("The Family is Full \n");
                    }
                }
            }
        }
        else {
            struct Family newfamily;
            printf("enter the family id: ");
            scanf("%d",&newfamily.family_id);
            printf("enter the family name: ");
            scanf("%s",newfamily.family_name);
            int i = 0;
            int found = 0; 
            while (i < familyCount && newfamily.family_id > Families[i].family_id){
                i++;
            }
            for (int j = familyCount ; j > i; j--){
                Families[j] = Families[j-1];
            }
            Families[i].family_id = newfamily.family_id;
            strcpy(Families[i].family_name,newfamily.family_name);
            Families[i].no_members = 1;
            Families[i].family_members[0] = newuser.user_id;
            Families[i].total_income = newuser.income;
            familyCount++;
        }
    }
    return status_code;
}

int CreateFamily(){
    int status_code = SUCCESS;
    if(familyCount >= MAX_FAMILIES){
        printf("Datbase is full \n");
        status_code = FAILURE;
    }
    else{
        struct Family newfamily;
        newfamily.total_income = 0;
        newfamily.total_expense = 0;
        int n;
        printf("enter the family id: ");
        scanf("%d",&newfamily.family_id);
        printf("enter the family name: ");
        scanf("%s",newfamily.family_name);
        printf("Enter the no of family members (Max 4, Min 1): ");
        scanf("%d" , &n);
        for (int i=0 ; i<4 ; i++){
            newfamily.family_members[i] = 0;
        }
        struct User members[4];
        printf("Enter the family members (Non-existent USERS ONLY) \n");
        for(int k=0; k<n; k++){
            printf("\nEnter the details of Member %d \n", k+1);
            printf("Enter the user id: ");
            scanf("%d", &members[k].user_id);
            printf("Enter Name: ");
            scanf("%s", members[k].user_name);
            printf("Enter the Income: ");
            scanf("%f", &members[k].income);
            int i = 0;
            int found = 0;
            while (i < userCount && members[k].user_id > Users[i].user_id){
                i++;
            }
            for (int j = userCount ; j > i; j--){
                Users[j] = Users[j-1];
            }
            Users[i].user_id = members[k].user_id;
            strcpy(Users[i].user_name,members[k].user_name);
            Users[i].income = members[k].income;
            printf("User added sussesfully\n");
            userCount++;
            newfamily.family_members[k] = members[k].user_id;
            newfamily.total_income += members[k].income;
        }
        int i=0;
        int found = 0; 
        while (i < familyCount && newfamily.family_id > Families[i].family_id){
            i++;
        }
        for (int j = familyCount ; j > i; j--){
            Families[j] = Families[j-1];
        }
        Families[i] = newfamily;
        printf("Family added sussesfully\n");
        familyCount++;
    }
    return status_code;
}

int AddExpense(){
    int status_code = SUCCESS;
    if(expenseCount >= MAX_EXPENSES){
        printf("database is full \n");
        status_code = FAILURE;
    }
    else{
        struct Expense newexpense;
        printf("Enter the expense id: ");
        scanf("%d",&newexpense.expense_id);
        printf("Enter the user id: ");
        scanf("%d",&newexpense.user_id);
        printf("Enter the expense category: ");
        scanf("%s",newexpense.category);
        printf("Enter the expense amount: ");
        scanf("%f",&newexpense.amount);
        printf("Enter the date of expense: ");
        scanf("%d",&newexpense.date);
        int i;
        int found = 0;
        for (i = 0; i < expenseCount && !found; i++) {
            if (newexpense.user_id < Expenses[i].user_id || 
                (newexpense.user_id == Expenses[i].user_id && newexpense.expense_id < Expenses[i].expense_id)) {
                found = 1;
                i--;
            }
        }
        // Shift the expenses to make room for the new expense
        for (int j = expenseCount; j > i; j--) {
            Expenses[j] = Expenses[j - 1];
        }
        Expenses[i].expense_id = newexpense.expense_id;
        Expenses[i].user_id = newexpense.user_id;
        strcpy(Expenses[i].category,newexpense.category);
        Expenses[i].amount = newexpense.amount;
        Expenses[i].date = newexpense.date;
        printf("expense added sussesfully\n");
        expenseCount++;
        found = 0;
        for (int j=0 ; j < familyCount && !found; j++){
            for (int k=0 ; k < Families[j].no_members && !found; k++){
                if (Families[j].family_members[k] == Expenses[i].user_id){
                    Families[j].total_expense += Expenses[i].amount;
                    found = 1;
                }
            }
        }
    }
    return status_code;
}

int Update_or_delete_individual_Family_details(int updateordelete){
    int status_code = SUCCESS;
    int user_id;
    printf("enter the user id\n");
    scanf("%d",&user_id);
    if(user_id >= MAX_USERS){
        printf("user not found\n");
        status_code = FAILURE;
    }
    else{
        if(updateordelete==0){  // WILL UPDATE THE RECORD
            int i,flag=0,previous_income,new_income;
            for(i=0; i < userCount && !flag; i++){
                if(Users[i].user_id==user_id){
                    printf("enter the new user name\n");
                    scanf("%s",Users[i].user_name);
                    previous_income = Users[i].income;
                    printf("enter the new user income\n");
                    scanf("%f",&Users[i].income);
                    new_income = Users[i].income;
                    flag=1;
                }
            }

            for(int i=0; i<familyCount && flag; i++){
                for(int j=0;j<4;j++){
                    if(Families[i].family_members[j] == user_id){
                        Families[i].total_income += new_income - previous_income;
                    }
                }
            }
            if (flag == 0){
                printf("User not found \n");
                status_code = FAILURE;
            }  
        }
        else{   // WILL DELETE THE RECORD
            int i,flag=0,income,expense=0;
            for(i=0; i< userCount && !flag ; i++){
                if(Users[i].user_id == user_id){
                    income = Users[i].income;
                    for(int j=i; j < userCount-1; j++){
                        Users[j]=Users[j+1];
                    }
                    flag=1;
                    userCount--;
                }
            }
            for(i=0; i < expenseCount && flag; i++){
                if(Expenses[i].user_id == user_id){
                    expense += Expenses[i].amount;
                    for(int j=i; j < expenseCount - 1; j++){
                        Expenses[j]=Expenses[j+1];
                    }
                    expenseCount--;
                }
            }
            int flag1=0;
            for(i=0; i<familyCount && !flag1 && flag; i++){
                for(int j=0;j<4 && !flag1;j++){
                    if(Families[i].family_members[j]==user_id){
                        Families[i].family_members[j]=0;
                        Families[i].total_income -= income;
                        Families[i].total_expense -= expense;
                        Families[i].no_members--;
                        flag1=1;
                        i--;
                    } 
                }
            }
            int ifdelete=0;
            for(int k=0; k<4 && flag; k++){
                if(Families[i].family_members[k]==0){
                    ifdelete++;
                }
            }
            if(ifdelete==4){
                for(int j=i;j<familyCount-1;j++){
                    Families[j]=Families[j+1];
                }
                printf("The Family is also deleted \n");
                familyCount--;
            }
            if (!flag){
                status_code = FAILURE;
            }
        }
    }
    return status_code;
}

int Update_delete_expense(int updateordelete){
    int status_code = SUCCESS;
    int expense_id;
    printf("enter the expense id\n");
    scanf("%d",&expense_id);
    if(expense_id >= MAX_EXPENSES){
        printf("expense not found\n");
        status_code = FAILURE;
    }
    else{
        if(updateordelete==0){ // WILL UPDATE THE EXPENSE
            int i,flag=0,previous_expense,new_expense;
            for(i=0; i < expenseCount && !flag; i++){
                if(Expenses[i].expense_id==expense_id){
                    printf("enter the new expense category\n");
                    scanf("%s",Expenses[i].category);
                    previous_expense = Expenses[i].amount;
                    printf("enter the new expense amount\n");
                    scanf("%f",&Expenses[i].amount);
                    new_expense = Expenses[i].amount;
                    flag=1;
                }
            }
            for(int i=0;i<familyCount && flag;i++){
                for(int j=0;j<4;j++){
                    if(Families[i].family_members[j]==Expenses[i].user_id){
                        Families[i].total_expense+=new_expense-previous_expense; 
                    }
                } 
            }
            if (!flag){
                status_code = FAILURE;
            }
        }
        else{
            int i,flag=0,prev_expense,found,user_id;
            for(i=0;i<expenseCount && !flag ; i++){
                if(Expenses[i].expense_id == expense_id){
                    prev_expense = Expenses[i].amount;
                    user_id = Expenses[i].user_id;
                    for(int j=i; j<expenseCount-1; j++){
                        Expenses[j]=Expenses[j+1];
                    }
                    flag=1;
                    expenseCount--;
                }
            }
            found = 0;
            for(int i=0; i<familyCount && flag && !found; i++){
                for(int j=0; j<4 && !found; j++){
                    if(Families[i].family_members[j] == user_id){
                        Families[i].total_expense -= prev_expense;
                        Families[i].family_members[j] = 0;
                        found = 1;
                    }
                }
            }
            if (!flag){
                status_code = FAILURE;
            }
        }
    }
    return status_code;
}

int Get_total_expense(){
    int status_code = SUCCESS;
    int family_id;
    int found = 0;
    int expense;
    printf("enter the family id\n");
    scanf("%d",&family_id);
    for (int i=0 ; i < familyCount && !found ; i++){
        if (Families[i].family_id == family_id){
            found = 1;
            expense = Families[i].total_expense;
            printf("the total expense of the family is %d\n", expense);
            if(expense > Families[i].total_income){
                printf("the family is in debt\n");
            }
            else{
                printf("the family is in profit\n");
            }
        }
    }
    if (!found){
        printf("Family doesnt exist \n");
        status_code = FAILURE;
    }
    return status_code;
}

int Get_categorical_expense(char category[15]){
    int status_code = SUCCESS;
    int family_id;
    printf("enter the family id\n");
    scanf("%d",&family_id);
    int cat_expense = 0;

    // Defining a structure to store user_ids with expense 
    struct member_expense{
        int id;
        int amount;
    };
    struct member_expense member_Expenses[4];
    // Initializing the members' Expenses array 
    for (int i=0 ; i<4 ; i++){
        member_Expenses[i].id = 0;
        member_Expenses[i].amount = 0;
    }

    int found = 0;
    int found_member = 0;
    for (int i=0 ; i < familyCount && !found; i++){
        if (Families[i].family_id == family_id){
            found = 1;
            for (int j=0 ; j < expenseCount ; j++){
                if (strcmp(category,Expenses[j].category) == 0){
                    cat_expense += Expenses[j].amount;
                    found_member = 0;
                    for (int k=0 ; k < 4 && !found_member; k++){
                        if (Families[i].family_members[k] == Expenses[j].user_id){
                            member_Expenses[k].id = Families[i].family_members[k];
                            member_Expenses[k].amount += Expenses[j].amount;
                            found_member = 1;
                        }
                    }
                }
            }
        }
    }
    if (!found){
        status_code = FAILURE;
    }
    else {
        // Sorting in descending order
        struct member_expense temp;
        for (int i=0 ; i < 3 ; i++){
            for (int j=0 ; j < 3-i ; j++){
                if (member_Expenses[j].amount < member_Expenses[j+1].amount){
                    temp = member_Expenses[j];
                    member_Expenses[j] = member_Expenses[j+1];
                    member_Expenses[j+1] = temp;
                }
            }
        }
        printf("The total Category expense is : %d \n", cat_expense);
        printf("Member-wise contribution to category Expenses:- \n");
        for (int i=0 ; i<4 ; i++){
            if (member_Expenses[i].id != 0){
                printf("User ID: %d ", member_Expenses[i].id);
                printf("Expense contributed %d ", member_Expenses[i].amount);
            }
        }
    }
    return status_code;
}

int Get_highest_expense_day(){
    int status_code = SUCCESS;
    int family_id;
    printf("enter the family id\n");
    scanf("%d",&family_id);
    int dailyExpenses[10];
    for(int i=0;i<10;i++){
        dailyExpenses[i]=0;
    }
    int found = 0;
    int found_member;
    for (int i=0; i < familyCount && !found; i++){
        if (Families[i].family_id == family_id){
            found = 1;
            for (int j=0; j < expenseCount ; j++){
                found_member = 0;
                for (int k=0 ; k<4 && !found_member; k++){
                    if (Expenses[j].user_id == Families[i].family_members[k]){
                        dailyExpenses[Expenses[j].date - 1] += Expenses[j].amount;
                        found_member = 1;
                    }
                }
            }
        }
    }
    if (!found){
        printf("Family not found \n");
        status_code = FAILURE;
    }
    else {
        int max=0;
        int max_date=0;
        for(int i=0;i<10;i++){
            if(dailyExpenses[i] > max){
                max = dailyExpenses[i];
                max_date = i+1;
            }
        }
        printf("the family %d has the highest expense on %d \n", family_id, max_date);
    }
    return status_code;
}

int Get_individual_expense(int user_id){
    int status_code = SUCCESS;
    int found = 0;
    int expense=0;
    int categoryExpenses[5];
    for (int i=0 ; i<5 ; i++){
        categoryExpenses[i] = 0;
    }
    char *expenseNames[5] = {"Utility", "Rent", "Grocery", "Stationary", "Leisure"};
    for(int i=0; i<expenseCount; i++){
        if (Expenses[i].user_id == user_id){
            found = 1;
            if(strcmp(Expenses[i].category, "Utility")==0){
                expense += Expenses[i].amount;
                categoryExpenses[0] += Expenses[i].amount;
            }
            else if(strcmp(Expenses[i].category, "Rent")==0){
                expense += Expenses[i].amount;
                categoryExpenses[1] += Expenses[i].amount;
            }
            else if(strcmp(Expenses[i].category, "Grocery")==0){
                expense += Expenses[i].amount;
                categoryExpenses[2] += Expenses[i].amount;
            }
            else if(strcmp(Expenses[i].category, "Stationary")==0){
                expense += Expenses[i].amount;
                categoryExpenses[3] += Expenses[i].amount;
            }
            else if(strcmp(Expenses[i].category, "Leisure")==0){
                expense += Expenses[i].amount;
                categoryExpenses[4] += Expenses[i].amount;
            }
        }
    }
    if (!found){
        status_code = FAILURE;
    }
    else {
        int i, j, temp;
        char *tempName;
        for (i = 0; i < 5 - 1; i++) {
            for (j = 0; j < 5 - i - 1; j++) {
                if (categoryExpenses[j] < categoryExpenses[j + 1]) {
                // Swap the Expenses
                    temp = categoryExpenses[j];
                    categoryExpenses[j] = categoryExpenses[j + 1];
                    categoryExpenses[j + 1] = temp;

                // Swap the corresponding expense names
                    tempName = expenseNames[j];
                    expenseNames[j] = expenseNames[j + 1];
                    expenseNames[j + 1] = tempName;
                }
            }
        }
        printf("Expense for user %d:\n", user_id);
        printf("Cartegory-wise expenses:- \n");
        for (i = 0; i < 5; i++){
            printf("%s: %d\n", expenseNames[i], categoryExpenses[i]);
        }
        printf("the total expense of the user is %d\n",expense);
    }
    return status_code;
}

void Display_Users(){
    printf("Total Users: %d \n", userCount);
    for (int i=0; i < userCount ; i++){
        printf("User ID: %d , Name : %s , Income : %f \n", Users[i].user_id, 
        Users[i].user_name, Users[i].income);
    }
}

void Display_Families(){
    printf("Total Families: %d \n", familyCount);
    for (int i=0 ; i < familyCount ; i++){
        printf("\nFamily ID: %d , Family Name: %s \n", Families[i].family_id, 
        Families[i].family_name);
        printf("Family Member IDs : ");
        for (int j=0 ; j<4 ; j++){
            if (Families[i].family_members[j] != 0){
                printf("%d ", Families[i].family_members[j]);
            }
        }
        printf("\nTotal Income: %f , Total Expense: %f \n", 
        Families[i].total_income, Families[i].total_expense);
    }
}

void Display_Expenses(){
    printf("Total Expenses: %d \n", expenseCount);
    for (int i=0 ; i < expenseCount ; i++){
        printf("Expense ID: %d, User ID: %d, Category: %s, Amount: %f, Date: %d \n", 
        Expenses[i].expense_id, Expenses[i].user_id, Expenses[i].category, Expenses[i].amount, 
        Expenses[i].date);
    }
}

void CheckOperation(int status_code){
    if (status_code == FAILURE){
        printf("The Operation was a Failure \n");
    }
    else {
        printf("The operation was a Success \n");
    }
}

int main(){
    int flag = 0;
    int choice;
    int status_code;
    Initialize_DB_User(MAX_USERS);
    Initialize_DB_Family(MAX_FAMILIES);
    Initialize_DB_Expense(MAX_EXPENSES);
    while (!flag){
        printf("\n---------FAMILY EXPENSE TRACKING SYSTEM--------- \n");
        printf("OPTIONS AVAILABLE:- \n");
        printf("1. Add a User \n");
        printf("2. Add an expense \n");
        printf("3. Create a family \n");
        printf("4. Update individual details (will affect in family also) \n");
        printf("5. Delete individual details (will affect in family also) \n");
        printf("6. Update an expense \n");
        printf("7. Delete an expense \n");
        printf("8. Get total familiy expense \n");
        printf("9. Get categorical expense for a particular category and family \n");
        printf("10. Get highest expense day for a family \n");
        printf("11. Get individual expense (also shows category-wise expenses) \n");
        printf("12. Display User Database \n");
        printf("13. Display Family Database \n");
        printf("14. Display Expense Database \n");
        printf("15. Exit \n");
        printf("Enter a choice (1/2/3/4/5/6/7/8/9/10/11/12/13/14/15): ");
        scanf("%d", &choice);
        printf("--------------------------------------------------- \n");
        if (choice == 1){
            status_code = AddUser();
            CheckOperation(status_code);
        }
        else if (choice == 2){
            status_code = AddExpense();
            CheckOperation(status_code);
        }
        else if (choice == 3){
            status_code = CreateFamily();
            CheckOperation(status_code);
        }
        else if (choice == 4){
            status_code = Update_or_delete_individual_Family_details(0);
            CheckOperation(status_code);
        }
        else if (choice == 5){
            status_code = Update_or_delete_individual_Family_details(1);
            CheckOperation(status_code);
        }
        else if (choice == 6){
            status_code = Update_delete_expense(0);
            CheckOperation(status_code);
        }
        else if (choice == 7){
            status_code = Update_delete_expense(1);
            CheckOperation(status_code);
        }
        else if (choice == 8){
            status_code = Get_total_expense();
            CheckOperation(status_code);
        } 
        else if (choice == 9){
            char category[15];
            printf("Enter the expense category: ");
            scanf("%s",category);
            status_code = Get_categorical_expense(category);
            CheckOperation(status_code);
        }
        else if (choice == 10){
            status_code = Get_highest_expense_day();
            CheckOperation(status_code);
        }
        else if(choice == 11){
            int user_id;
            printf("Enter the user id: ");
            scanf("%d", &user_id);
            status_code = Get_individual_expense(user_id);
            CheckOperation(status_code);
        }
        else if (choice == 12){
            Display_Users();
        }
        else if (choice == 13){
            Display_Families();
        }
        else if (choice == 14){
            Display_Expenses();
        }
        else if (choice == 15){
            flag = 1;
        }
        else {
            printf("Choice is INVALID. Please re-enter choice \n");
        }
    }
    return 0;
}
