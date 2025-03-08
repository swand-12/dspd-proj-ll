// Prototype - 1 DSPD-II Assignment 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int status_code;
typedef enum{FALSE, TRUE} boolean;

typedef struct date_tag{
	int day;
	int month;
	int year;
} Date;

typedef struct user{
	int id;
	char name[50];
	float income;
	struct user *next;
} User;

typedef struct expense {
	int expense_id;
	int user_id;
	float expense_amount;
	char category[10];
	Date date;
	struct expense *next;
} Expense;

typedef struct family{
	int family_id;
	char family_name[50];
	User* members[4];
    int No_members;
	float family_income;
	float family_expense;
	struct family *next;
} Family;

status_code displayUsers(User *userList){
    status_code sc = 0;
    if(!userList){
        sc = 2;
    }
    else{
        printf("<<<< Printing the CURRENT USER DATABASE >>>>> \n");
        while(userList && !sc){
            printf("\nUser Id : %d \n",userList->id);
            printf("User Name : %s \n",userList->name);
            printf("User income : %f \n",userList->income);
            userList = userList -> next;
        }
    }
    return sc;
}

status_code displayFamily(Family *familyList){
    status_code sc = 0;
    if(!familyList){
        sc = 3;
    }
    else{
        printf("<<<< Printing the CURRENT FAMILY DATABASE >>>>> \n");
        while(familyList && !sc){
            printf("\nFamily id : %d \n", familyList -> family_id);
            printf("Family Name : %s \n", familyList -> family_name);
            printf("Family Number of Members : %d\n", familyList -> No_members);
            printf("Family Income : %f \n", familyList -> family_income);
            printf("Family Expense : %f \n", familyList -> family_expense);
            familyList = familyList -> next;
        }
    }
    return sc;
}

void displayDate(Date date){
    printf("%d/%d/%d \n", date.day, date.month, date.year);
}

status_code displayExpenses(Expense *expenseList){
    status_code sc = 0;
    if(!expenseList){
        sc = 4;
    }
    else{
        printf("<<<< Printing the CURRENT EXPENSE DATABASE >>>>> \n");
        while(expenseList && !sc){
            printf("\nExpense Id : %d \n",expenseList->expense_id);
            printf("Expense User Id : %d \n",expenseList->user_id);
            printf("Expense Amount : %f \n",expenseList->expense_amount);
            printf("Expense Date : ");
            displayDate(expenseList -> date);
            printf("Expense Category : %s \n",expenseList->category);
            expenseList = expenseList -> next;
        }
    }
    return sc;
}

void status_code_checker(status_code sc){
    if(sc == 0){
        printf("The Operation was a success \n");
    }
    else if(sc == 1){
        printf("Memory Allocation failed \n");
    }
    else if(sc == 2){
        printf("User List is Empty \n");
    }
    else if(sc == 3){
        printf("Family List is Empty \n");
    }
    else if(sc == 4){
        printf("Expenses List is Empty \n");
    }
    else if(sc == 5){
        printf("Family is Full \n");
    }
    else {
        printf("The Operation failed / User Ended Execution \n");
    }
}

status_code CreateUserNode(User *userList, User **userNode){
    status_code sc = 0;
    User *newuser = (User*)malloc(sizeof(User));
    if(!newuser){
        sc = 1;
    }
    else{
        printf("Enter Name of the user: ");
        scanf("%49s", newuser->name);  // Reads a string safely
        printf("Enter the income of the user: ");
        scanf("%f", &newuser->income);
        newuser->next = NULL;
        User *userptr;
        int loop_remover = 0;
        int found;
        int choice = 1;

        while(!loop_remover && choice){
            found = 0;
            printf("Enter the ID of the user: ");
            scanf("%d", &newuser->id);
            userptr = userList;
            while(userptr && !found){
                if(userptr -> id == newuser -> id){
                    found = 1;
                }
                else{
                    userptr = userptr -> next;
                }
            }
            if(found){
                printf("You have entered an already existing USER-ID \n");
                printf("Do You Wish to Proceed Further (0-NO/1-YES): ");
                scanf("%d", &choice);
                if(!choice){
                    sc = 6;
                    loop_remover = 1;
                }
            }
            else{
                loop_remover = 1;
            }
        }
        if(sc){
            free(newuser);
        }
        else{
            *userNode = newuser;
           
        }
    }
    
    return sc;
}

status_code CreateFamilyNode(Family *familyList, Family **familyNode){
    status_code sc = 0;
    Family *newfamily = (Family*)malloc(sizeof(Family));
    if(!newfamily){
        sc = 1;
    }
    else{
        printf("Enter Name of the family: ");
        scanf("%49s", newfamily->family_name);  // Reads a string safely
        newfamily->next = NULL;
        Family *familyptr;
        int loop_remover = 0;
        int found;
        int choice = 1;

        while(!loop_remover && choice){
            found = 0;
            printf("Enter the ID of the family: ");
            scanf("%d", &newfamily->family_id);
            familyptr = familyList;
            while(familyptr && !found){
                if(familyptr -> family_id == newfamily -> family_id){
                    found = 1;
                }
                else{
                    familyptr = familyptr -> next;
                }
            }
            if(found){
                printf("You have entered an already existing USER-ID \n");
                printf("Do You Wish to Proceed Further (0-NO/1-YES): ");
                scanf("%d", &choice);
                if(!choice){
                    sc = 6;
                    loop_remover = 1;
                }
            }
            else{
                loop_remover = 1;
            }
        }
        if(sc){
            free(newfamily);
        }
        else{
            newfamily->No_members = 0;
            for(int i=0 ; i<4 ; i++){
                newfamily->members[i] = NULL;
            }
            newfamily->family_income = 0;
            newfamily->family_expense = 0;
            *familyNode = newfamily;
        }
    }
    return sc;
}

status_code CreateExpenseNode(Expense *expenseList, User *userList, Expense **expenseNode){
    status_code sc = 0;
    Expense *newexpense = (Expense*)malloc(sizeof(Expense));
    if(!newexpense){
        sc = 1;
    }
    else{
     Expense *expenseptr = expenseList;
     User *userptr = userList;
     if(userptr==NULL){
        sc=2;
      }
      else{      
        int loop_remover = 0;
        int found;
        int choice = 1;
        
        // Validating User-ID
        while(!loop_remover && choice){
            found = 0;
            printf("Enter the ID of the user: ");
            scanf("%d", &newexpense->user_id);
            userptr = userList;
            while(userptr && !found){
                if(userptr -> id == newexpense -> user_id){
                    found = 1;
                }
                else{
                    userptr = userptr -> next;
                }
            }
            if(!found){
                printf("You have entered a non-existing USER-ID \n");
                printf("Do You Wish to Proceed Further (0-NO/1-YES): ");
                scanf("%d", &choice);
                if(!choice){
                    sc = 6;
                    loop_remover = 1;
                }
            }
            else{
                loop_remover = 1;
            }
        }
        if(sc){
            free(newexpense);
        }
        else{
            found=0;
            // Validating Expense ID
            loop_remover = 0;
            choice = 1;
            while(!loop_remover ){
                found = 0;
                printf("Enter the ID of the Expense: ");
                scanf("%d", &(newexpense->expense_id));
                expenseptr = expenseList;
                while(expenseptr && !found){
                    if(expenseptr -> expense_id == newexpense -> expense_id){
                        found = 1;
                    }
                    else{
                        expenseptr = expenseptr -> next;
                    }
                }
                if(found){
                    printf("You have entered an already existing expense-ID \n");
                    printf("Do You Wish to Proceed Further (0-NO/1-YES): ");
                    scanf("%d", &choice);
                    if(!choice){
                        sc = 6;
                        loop_remover = 1;
                    }
                }
                else{
                    loop_remover = 1;
                }
            }
            if(sc){
                free(newexpense);
            }
            else{
                printf("Enter the Expense amount: ");
                scanf("%f", &(newexpense->expense_amount));
                // Input expense category
                printf("Choose the Expense Category:\n");
                printf("(1) Travel\n(2) Leisure\n(3) Rent\n(4) Food\n(5) Other Expense\n Enter here: ");
                int categoryChoice;
                scanf("%d", &categoryChoice);

                switch (categoryChoice) {
                    case 1:
                        strcpy(newexpense->category, "Travel");
                        break;
                    case 2:
                        strcpy(newexpense->category, "Leisure");
                        break;
                    case 3:
                        strcpy(newexpense->category, "Rent");
                        break;
                    case 4:
                        strcpy(newexpense->category, "Food");
                        break;
                    default:
                        strcpy(newexpense->category, "Other");
                        break;
                }

                // Input expense date
                printf("Enter the date of the Expense in dd mm yyyy format: ");
                scanf("%d %d %d", &(newexpense->date.day), &(newexpense->date.month), &(newexpense->date.year)); 
                newexpense->next = NULL;
                *expenseNode = newexpense;
            }
            }
        }
    }
    return sc;
}

User* findUser(User* userList){
    User* ret_val;
    User* userptr;
    int user_id;
    if(userList == NULL){
        ret_val = NULL;
    }
    else{
        int choice = 1;
        int loop_remover = 0;
        while(!loop_remover && choice){
            int found = 0;
            printf("Enter the User-ID to find : ");
            scanf("%d", &user_id);
            userptr = userList;
            while(userptr && !found){
                if(userptr -> id == user_id){
                    found = 1;
                }
                else{
                    userptr = userptr -> next;
                }
            }
            if(!found){
                printf("The User-ID entered doesnt exist \n");
                printf("Do you wish to proceed further (0-No/1-Yes) : ");
                scanf("%d", &choice);
                if(!choice){
                    ret_val = NULL;

                }
            }
            else{
                ret_val = userptr;
                loop_remover = 1;
            }
        }
    }
    return ret_val;
}

Family* findFamily(Family* familyList){
    Family *ret_val;
    Family *familyptr;
    int family_id;
    if(familyList == NULL){
        ret_val = NULL;
    }
    else{
        int choice = 1;
        int loop_remover = 0;
        while(!loop_remover && choice){
            int found = 0;
            printf("Enter the Family-ID to find : ");
            scanf("%d", &family_id);
            familyptr = familyList;
            while(familyptr && !found){
                if(familyptr -> family_id == family_id){
                    found = 1;
                }
                else{
                    familyptr = familyptr -> next;
                }
            }
            if(!found){
                printf("The User-ID entered doesnt exist \n");
                printf("Do you wish to proceed further (0-No/1-Yes) : ");
                scanf("%d", &choice);
                if(!choice){
                    ret_val = NULL;

                }
            }
            else{
                ret_val = familyptr;
                loop_remover = 1;
            }
        }
    }
    return ret_val;
}

Expense* findExpense(Expense* expenseList){
    Expense* ret_val;
    Expense* expenseptr = expenseList;
    int expense_id;
    if(expenseList == NULL){
        ret_val = NULL;
    }
    else{
        int choice = 1;
        int loop_remover = 0;
        while(!loop_remover && choice){
            int found = 0;
            printf("Enter the Expense-ID to find : ");
            scanf("%d", &expense_id);
            expenseptr = expenseList;
            while(expenseptr && !found){
                if(expenseptr -> expense_id == expense_id){
                    found = 1;
                }
                else{
                    expenseptr = expenseptr -> next;
                }
            }
            if(!found){
                printf("The User-ID entered doesnt exist \n");
                printf("Do you wish to proceed further (0-No/1-Yes) : ");
                scanf("%d", &choice);
                if(!choice){
                    ret_val = NULL;

                }
            }
            else{
                ret_val = expenseptr;
                loop_remover = 1;
            }
        }
    }
    return ret_val;
}

status_code AddUser(User **userList, Family **familyList, User *newuser){
    status_code sc = 0;
    User* userptr = *userList;
    Family* familyptr = *familyList;
    int isNewFamily;
    printf("Do you want to enter a existing family (Enter 0) or Create a new Family (Enter 1): ");
    scanf("%d", &isNewFamily);
    if(!isNewFamily && !familyptr){
        printf("The Family List is Empty. You may create a new family \n");
        isNewFamily = 1;
    }
    if(isNewFamily){
        Family* newfamily;
        sc = CreateFamilyNode(*familyList, &newfamily);
        if(sc){
            free(newuser);
        }
        else{
            newfamily->members[0] = newuser;
            newfamily->family_income = newuser->income;
            newfamily->next = NULL;	
            newfamily->family_expense = 0;
            newfamily->No_members = 1;
            familyptr = *familyList;
            if(familyptr==NULL || familyptr->family_id > newfamily->family_id){
                newfamily->next = familyptr;
                familyptr = newfamily;
                *familyList = familyptr;
            }
            else{
                while(familyptr->next != NULL && (familyptr -> next) -> family_id < newfamily->family_id){
                    familyptr = familyptr -> next;
                }
                newfamily -> next = familyptr->next;
                familyptr -> next = newfamily;
            }
            printf("New Family Having ID : %d added successfully \n", newfamily -> family_id);
        }
    }
    else{
        familyptr = findFamily(*familyList);
        if(!familyptr){
            sc = 6;
        }
        if(!sc){
            if(familyptr -> No_members == 4){
                sc = 5;
            }
            else{
                //now familyptr points on the selected family
                familyptr->family_income += newuser->income;
                familyptr->members[(familyptr->No_members)++] = newuser;
            }
        }
    }
    if(!sc){
        userptr = *userList;
        // Insert user in sorted order
        if (*userList == NULL || (*userList)->id > newuser->id) {
            newuser->next = *userList;
            *userList = newuser;
        } 
        else {
            User *trail = *userList;
            while (trail->next != NULL && trail->next->id < newuser->id) {
                trail = trail->next;
            }
            newuser->next = trail->next;
            trail->next = newuser;
        }
        printf("The User having ID : %d is entered successfully \n",newuser->id);
    }
    else{
        free(newuser);
    }
    return sc;
}    

status_code AddExpense(Expense** expenseList, Family *familyList, Expense *newexpense){


    status_code sc = 0;

    newexpense -> next = NULL;
    Expense *expenseptr = *expenseList;
    Family *familyptr = familyList;
    
    Expense *current = *expenseList;
    Expense *previous = NULL;
    int flag1=0;
    // Find the correct position to insert the new expense
    while (current != NULL && !(flag1)) {
        if (current->user_id > newexpense->user_id ||
            (current->user_id == newexpense->user_id && current->expense_id > newexpense->expense_id)) {
            flag1=1;
        }
        else{
        previous = current;
        current = current->next;
        }
    }
    if (previous == NULL) {
        // Insert at the beginning
        newexpense->next = *expenseList;
        *expenseList = newexpense;
    } 
    else {
        // Insert in the middle or at the end
        newexpense->next = current;
        previous->next = newexpense;
    }
    printf("The Expense record having ID : %d inserted \n", newexpense -> expense_id);

        // Updating Family having user id
    familyptr = familyList;
    int flag2=0;
    while (familyptr != NULL &&!(flag2)) {
        for (int i = 0; i < familyptr->No_members && !(flag2); i++) {
            if (familyptr->members[i]->id == newexpense->user_id) {
                familyptr->family_expense += newexpense->expense_amount;
                flag2=1;
                printf("Family Having ID : %d updated its expenses \n", familyptr->family_id);
            }
        }
        familyptr = familyptr->next;
    }
    
    return sc;
}

status_code updateUser(User *userList, Family *familyList, User* user){
    status_code sc = 0;
    if(user != NULL){
        int prev_income = user -> income;
        Family* familyptr = familyList;
        printf("Enter new Name of the User: ");
        scanf("%49s", user -> name);
        printf("Enter new income of the User: ");
        scanf("%f", &(user -> income));
        int new_income = user -> income;
        printf("User having ID : %d updated \n", user -> id);
        // Searching in Family and updating income
        int found  = 0;
        while(familyptr != NULL && !found){
            for(int i=0 ; i < familyptr -> No_members && !found ; i++){
                if(familyptr -> members[i] -> id == user -> id){
                    familyptr -> family_income = (familyptr -> family_income) - prev_income + new_income;
                    found = 0;
                    printf("Income of Family having ID : %d updated \n", familyptr -> family_id);
                }
            }
            familyptr = familyptr -> next;
        }
    }
    else{
        sc = 6;
    }
    return sc;
}

status_code updateFamily(Family* familyList, Family* family){
    status_code sc = 0;
    if(familyList == NULL){
        sc = 3;
    }
    else if(family == NULL){
        sc = 6;
    }
    else{
        printf("Enter the new name of the family: ");
        scanf("%49s", family -> family_name);
        printf("Updated Details for family having ID : %d \n", family -> family_id);
    }
    return sc;
}

status_code deleteUser(User **userList, Family **familyList, Expense **expenseList, User *user){
    status_code sc = 0;
    if(user == NULL){
        sc = 6;
    }
    else if(*userList == NULL){
        sc = 2;
    }
    else{
        int id = user -> id;
        int totalexpense = 0;
        Expense* expenseptr = *expenseList;
        User* userptr;
        Family* familyptr=*familyList;

        // Deleting User Expense Records
        Expense* eptr = expenseptr;
        Expense* trail = NULL;

        while (eptr != NULL) {
            if (eptr->user_id == id) {
                Expense* temp = eptr;
                if (trail == NULL) {
                    // First expense matches
                    expenseptr = eptr->next;
                } 
                else {
                    // Expense in the middle or end
                    trail->next = eptr->next;
                }
                eptr = eptr->next;
                totalexpense += temp->expense_amount;
                printf("Expense record having ID : %d deleted \n", temp->expense_id);
                free(temp);
            } 
            else {
                trail = eptr;
                eptr = eptr->next;
            }
        }
        *expenseList = expenseptr;

        // Updating / Deleting Family Record
        Family* fptr = familyptr;
        int flag = 0;
        while (fptr != NULL && !flag) {
            for (int i = 0; i < fptr->No_members && !flag; i++) {
                if (fptr->members[i]->id == id) {
                    flag = 1;
                }
            }
            if (!flag) {
                fptr = fptr->next;
            }
        }

        if (fptr != NULL) {
            if (fptr->No_members == 1) {
                // Delete the entire family
                if (familyptr == fptr) {
                    // Family is at the first position
                    familyptr = familyptr->next;
                    *familyList=familyptr;
                   
                } 
                else {
                    // Family is in the middle or end
                    Family* trail = familyptr;
                    while (trail->next != fptr) {
                        trail = trail->next;
                    }
                    trail->next = fptr->next;
                }
                printf("Family Having ID : %d deleted \n", fptr -> family_id);
                free(fptr);
            } 
            else {
                // Remove the user from the family
                flag = 0;
                for (int i = 0; i < fptr->No_members && !flag; i++) {
                    if (fptr->members[i]->id == id) {
                        // Shift members to the left
                        int j;
                        for (j = i; j < fptr->No_members - 1; j++) {
                            fptr->members[j] = fptr->members[j + 1];
                        }
                        fptr->No_members--;
                        fptr->members[j] = NULL;
                        flag = 1;
                    }
                }
                fptr->family_expense -= totalexpense;
                fptr->family_income -= user -> income;
                printf("Family Having ID : %d updated \n", fptr -> family_id);
            }
        }

        // Deleting the User
        userptr = *userList;
            if (user == userptr) {
            // User is at the first position
            userptr = userptr->next;
        } 
        else {
            // User is in the middle or end
            User* trail = userptr;
            while (trail->next != user) {
                trail = trail->next;
            }
            trail->next = user->next;
        }
        printf("User having ID : %d deleted \n", user -> id);
        free(user);
        *userList = userptr;
    }
    return sc;
}

status_code deleteFamily(User **userList, Family **familyList, Expense **expenseList, Family *family){
    status_code sc = 0;
    if(family == NULL){
        sc = 6;
    }
    else if(*familyList == NULL){
        sc = 2;
    }
    else{
        int no_members = family -> No_members;
        for(int i=0 ; i<no_members && family ->members[0] != NULL; i++){
            sc = deleteUser(userList, familyList, expenseList, family -> members[0]);
        }
    }
    return sc;
}

status_code updateExpense(Expense* expenseList, Family* familyList, Expense* expense){
    status_code sc = 0;
    if(expenseList == NULL){
        sc = 4;
    }
    else if(expense == NULL){
        sc = 6;
    }
    else{
        Expense* expenseptr = expenseList;
        Family* familyptr = familyList;
        Expense* temp = expenseptr;
        int found;
        while(temp && temp -> expense_id != expense -> expense_id){
            temp = temp -> next;
        }
        int user_id = temp -> user_id;
        float prevExpense = temp -> expense_amount;

        // Updating the Expense record
        printf("Enter the New Expense Amount: ");
        scanf("%f",&temp -> expense_amount);
        int category;
        printf("Choose the New Expense Category : \n(1) for Travel \n (2) for Leisure \n (3) for Rent \n (4) for Food \n (5) for Others \n  Enter here: ");
        scanf("%d",&category);
        if(category == 1){
            strcpy(temp->category,"Travel");
        }
        else if(category == 2){
            strcpy(temp->category,"Leisure");
        }
        else if(category == 3){
            strcpy(temp->category,"Rent");
        }
        else if(category == 4){
            strcpy(temp->category,"Food");
        }
        else{
            strcpy(temp->category,"Others");
        }
        printf("Enter the New date of The Expense In dd mm yyyy Format: ");
        scanf("%d%d%d",&temp->date.day,&temp->date.month,&temp->date.year);
        printf("Expense Record Changed for ID : %d \n", temp -> expense_id);

        // Updating the expense in Family list
        found = 0;
        while(familyptr && !found){
            for(int i = 0; i < familyptr -> No_members && !found; i++){
                if(familyptr->members[i]->id == user_id){
                    found = 1;
                    familyptr -> family_expense = familyptr -> family_expense + (temp->expense_amount - prevExpense);
                    printf("Family Details updated for ID : %d \n", familyptr -> family_id);
                }
            }
            familyptr = familyptr -> next;
        }
    }
    return sc;
}

status_code deleteExpense(Expense** expenseList, Family* familyList, Expense* expense){
    status_code sc = 0;
    if(expenseList == NULL){
        sc = 4;
    }
    else if(expense == NULL){
        sc = 6;
    }
    else{
        Expense* expenseptr = *expenseList;
        Family* familyptr = familyList;
        Expense* temp;
        int flag;
        float expenseAmount = expense -> expense_amount;
        int user_id = expense -> user_id;
        
        // Deleting Expense from the expense record
        if(expenseptr -> expense_id == expense->expense_id){
            //1st position
            temp = expenseptr;
            expenseptr = expenseptr->next;
            user_id = temp->user_id;
            printf("Expense Record having ID : %d deleted \n", expense -> expense_id);
            free(temp);
            *expenseList = expenseptr;

        }
        else{
            //anywhere else
            while(expenseptr != NULL && expenseptr -> expense_id != expense -> expense_id){
                temp = expenseptr;
                expenseptr = expenseptr->next;
            }
            temp -> next = expenseptr->next;
            user_id = expenseptr->user_id;
            printf("Expense Record having ID : %d deleted \n", expense -> expense_id);
            free(expenseptr);
        }
        
        // Updating the Family details (change in expense)
        flag = 0;
        while(familyptr != NULL && !(flag)){
            for(int i = 0; i < familyptr -> No_members && !flag; i++){
                if(familyptr -> members[i] -> id == user_id){
                    flag = 1;
                }
            }
            if(flag==1){
                familyptr -> family_expense -= expenseAmount;
                printf("Updated Record for Family having ID : %d \n", familyptr -> family_id);
            }
            else{
                familyptr = familyptr->next;
            }
        }
    }
    return sc;
}

status_code Get_total_expense(Family* familyList, Family* family, Expense* expenseList, int month, int year){
    status_code sc = 0;
    if(family == NULL){
        sc = 6;
    }
    else if(familyList == NULL){
        sc = 3;
    }
    else{
        Expense* expenseptr = expenseList;
        int total_expense = 0;
        int found;
        while(expenseptr != NULL){
            found = 0;
            for(int i = 0; i < family -> No_members && !found ; i++){
                if(family -> members[i] -> id == expenseptr -> user_id){
                    if(expenseptr -> date.month == month && expenseptr -> date.year == year){
                        total_expense += expenseptr -> expense_amount;
                    }
                    found = 1;
                }
            }
            expenseptr = expenseptr -> next;
        }
        printf("Total Monthly expense for the family in Month : %d, Year : %d is %d \n", month,year,total_expense);
        if(total_expense > family -> family_income){
            printf("The Family (ID : %d) is in debt \n", family -> family_id);
            printf("The family is in debt of Rs: %f \n", total_expense - (family -> family_income));
        }
        else{
            printf("The Family (ID : %d) is well-off \n", family -> family_id);
            printf("The family is saving Rs: %f \n", (family -> family_income) - total_expense);
        }
    }
    return sc;
}

status_code Get_categorcial_expense(Expense* expenseList, Family* family, int category){
    status_code sc = 0;
    if(family == NULL){
        sc = 6;
    }
    else{
        char category_string[15];
        float total_cat_expense = 0.0;
        // Convert Category to string
        switch(category){
            case 1:
                strcpy(category_string, "Travel");
                break;
            case 2:
                strcpy(category_string, "Leisure");
                break;
            case 3:
                strcpy(category_string, "Rent");
                break;
            case 4:
                strcpy(category_string, "Food");
                break;
            default:
                strcpy(category_string, "Other");
                break;
        }

        // Defining a structure to store user_ids with expense 
        struct member_expense{
            int id;
            float amount;
        };

        struct member_expense member_Expenses[4];
        // Initializing the members' Expenses array 
        for (int i=0 ; i<4 ; i++){
            member_Expenses[i].id = 0;
            member_Expenses[i].amount = 0.0;
        }
        for(int i=0 ; i<family -> No_members ; i++){
            member_Expenses[i].id = family ->members[i] ->id;
        }

        Expense* expenseptr = expenseList;
        int found;
        while(expenseptr != NULL){
            found = 0;
            for(int i=0; i< family->No_members && !found; i++){
                if(strcmp(category_string, expenseptr -> category) == 0 && member_Expenses[i].id == expenseptr -> user_id){
                    total_cat_expense += expenseptr -> expense_amount;
                    member_Expenses[i].amount += expenseptr -> expense_amount;
                    found = 1;
                }
            }
            expenseptr = expenseptr -> next;
        }

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
        printf("The total Category expense for Family (ID : %d) is : %f \n", family->family_id, total_cat_expense);
        printf("Member-wise contribution to category Expenses:- \n");
        for (int i=0 ; i<4 ; i++){
            if (member_Expenses[i].id != 0){
                printf("User ID: %d ", member_Expenses[i].id);
                printf("Expense contributed %f \n", member_Expenses[i].amount);
            }
        }
    }
    return sc;
}

status_code Get_highest_expense_day(Family* family, Expense* expenseList, int month, int year){
    status_code sc = 0;
    if(family == NULL){
        sc = 6;
    }
    else if(expenseList == NULL){
        sc = 4;
    }
    else{
        float expenseArray[31] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        Expense* expenseptr = expenseList;
        int found;
        while(expenseptr != NULL){
            found = 0;
            for(int i=0; i < family -> No_members && !found; i++){
                if(expenseptr->user_id == family -> members[i] -> id){
                    if(expenseptr -> date.month == month && expenseptr -> date.year == year){
                        expenseArray[(expenseptr->date.day-1)] += expenseptr -> expense_amount;
                    }
                    found = 1;
                }
            }
            expenseptr = expenseptr -> next;
        }

        // Finding the Maximum expense day
        int maxExpense = 0, maxDay = 1;
        for(int i=0;i<31;i++){
            if(maxExpense<expenseArray[i]){
                maxExpense=expenseArray[i];
                maxDay=i+1;
            }
        }

        printf("Family (ID : %d) had the highest expense for (Month : %d, Year : %d) on \nDay : %d \n", family->family_id, month, year, maxDay);
        printf("The Expense amount was : %d \n", maxExpense);
    }
    return sc;
}

status_code Get_individual_expense(Expense* expenseList, User* user, int month, int year){
    status_code sc = 0;
    if(user == NULL){
        sc = 6;
    }
    else if(expenseList == NULL){
        sc = 4;
    }
    else{
        char *expenseNames[5] = {"Travel", "Leisure", "Rent", "Food", "Other"};
        float CategoricalExpenses[5]={0,0,0,0,0};
        Expense* expenseptr = expenseList;
        int found;
        while(expenseptr != NULL){ 
            
            if(expenseptr -> user_id == user -> id && expenseptr->date.month == month && expenseptr->date.year == year){
                if(!(strcmp("Travel",expenseptr->category)) ){
                    CategoricalExpenses[0] += expenseptr->expense_amount;
                }
                else if(!(strcmp("Leisure",expenseptr->category))){
                    CategoricalExpenses[1] += expenseptr->expense_amount;
                }
                 else if(!(strcmp("Rent",expenseptr->category))){
                    CategoricalExpenses[2] += expenseptr->expense_amount;
                }
                  else if(!(strcmp("Food",expenseptr->category))){
                    CategoricalExpenses[3] += expenseptr->expense_amount;
                }
                else{
                    CategoricalExpenses[4] += expenseptr->expense_amount;
                }
            }
            expenseptr = expenseptr -> next;
        }
        
        // Sorting based on Categories
         for(int i=0;i<5-1;i++){
            for(int j=0;j<5-i-1;j++){
                if(CategoricalExpenses[j]<CategoricalExpenses[j+1]){
                    float temp=CategoricalExpenses[j];
                    CategoricalExpenses[j]=CategoricalExpenses[j+1];
                    CategoricalExpenses[j+1]=temp;
                    char *tempName;
                    tempName = expenseNames[j];
                    expenseNames[j] = expenseNames[j+1];
                    expenseNames[j+1] = tempName;
                }
            }
        }

        printf("The Category Wise Expenses of the User with Id %d are \n",user -> id);
        for(int i=0;i<5;i++){
            printf("Category : %s || Amount : %f \n",expenseNames[i],CategoricalExpenses[i]);
        }
    }
    return sc;
}

void Iniatialise_DB(User** userList, Family** familyList, Expense** expenseList){
    *familyList = NULL;
    *userList = NULL;
    *expenseList = NULL;
    FILE *userFile, *familyFile, *expenseFile;
    User *newUser, *userptr = *userList;
    Family *newFamily, *familyptr = *familyList;
    Expense *newExpense, *expenseptr = *expenseList;
    status_code sc;


    // Open files
    userFile = fopen("users.txt", "r");
    familyFile = fopen("families.txt", "r");
    expenseFile = fopen("expenses.txt", "r");

    // Read users
    while (!feof(userFile)) {
        newUser = (User *)malloc(sizeof(User));
        fscanf(userFile, "%d %s %f", &newUser->id, newUser->name, &newUser->income);
        userptr = *userList;
        // Insert user in sorted order
        if (*userList == NULL || (*userList)->id > newUser->id) {
            newUser->next = *userList;
            *userList = newUser;
        } 
        else {
            User *trail = *userList;
            while (trail->next != NULL && trail->next->id < newUser->id) {
                trail = trail->next;
            }
            newUser->next = trail->next;
            trail->next = newUser;
        }
        printf("Added user having ID : %d \n", newUser -> id);
    }

    // Read families
    while (!feof(familyFile)) {
        newFamily = (Family *)malloc(sizeof(Family));
        fscanf(familyFile, "%d %s", &newFamily->family_id, newFamily->family_name);
        newFamily->No_members = 0;
        newFamily->family_income = 0;
        newFamily->family_expense = 0;
        newFamily->next = NULL;

        // Read user IDs for family members
        int userID;
        int flag;
        User* user;
        for (int i = 0; i < 4; i++) {
            fscanf(familyFile, "%d", &userID);
            if(userID){
                flag = 0;
                userptr = *userList;
                while(userptr && !flag){
                    if(userptr -> id == userID){
                        user = userptr;
                        flag = 1;
                    }
                    userptr = userptr -> next;
                }
                if (user) {
                    newFamily->members[newFamily->No_members++] = user;
                    newFamily->family_income += user->income;
                }
            }
        }

        // Add family to the list in sorted order
        if (*familyList == NULL || (*familyList)->family_id > newFamily->family_id) {
            newFamily->next = *familyList;
            *familyList = newFamily;
        } 
        else {
            Family *trail = *familyList;
            while (trail->next != NULL && trail->next->family_id < newFamily->family_id) {
                trail = trail->next;
            }
            newFamily->next = trail->next;
            trail->next = newFamily;
        }
        printf("Added family having ID : %d \n", newFamily -> family_id);
    }

    // Read expenses
    while (!feof(expenseFile)) {
        newExpense = (Expense *)malloc(sizeof(Expense));
        fscanf(expenseFile, "%d %d %f %s %d %d %d",
               &newExpense->expense_id, &newExpense->user_id, &newExpense->expense_amount,
               newExpense->category, &newExpense->date.day, &newExpense->date.month, &newExpense->date.year);
        newExpense->next = NULL;
        sc = AddExpense(expenseList, *familyList, newExpense);
    }

    // Close files
    fclose(userFile);
    fclose(familyFile);
    fclose(expenseFile);
}

void SaveDB(User** userList, Family** familyList, Expense** expenseList){
    User* userptr = *userList;
    Family* familyptr = *familyList;
    Expense* expenseptr = *expenseList;
    FILE *userFile, *familyFile, *expenseFile;
    // Open files
    userFile = fopen("users.txt", "w");
    familyFile = fopen("families.txt", "w");
    expenseFile = fopen("expenses.txt", "w");

    // Save Users
    if (userptr) {
        fprintf(userFile, "%d %s %.2f", userptr->id, userptr->name, userptr->income);
        userptr = userptr->next;
    }
    while (userptr) {
        fprintf(userFile, "\n%d %s %.2f", userptr->id, userptr->name, userptr->income);
        userptr = userptr->next;
    }
    

    // Save Families
    if (familyptr) {
        fprintf(familyFile, "%d %s", familyptr->family_id, familyptr->family_name);
        for (int i = 0; i < 4; i++) {
            fprintf(familyFile, " %d", (i < familyptr->No_members) ? familyptr->members[i]->id : 0);
        }
        familyptr = familyptr->next;
    }
    while (familyptr) {
        fprintf(familyFile, "\n%d %s", familyptr->family_id, familyptr->family_name);
        for (int i = 0; i < 4; i++) {
            fprintf(familyFile, " %d", (i < familyptr->No_members) ? familyptr->members[i]->id : 0);
        }
        familyptr = familyptr->next;
    }    

    // Save Expenses
    if(expenseptr){
        fprintf(expenseFile, "%d %d %.2f %s %d %d %d",
            expenseptr->expense_id, expenseptr->user_id, expenseptr->expense_amount,
            expenseptr->category, expenseptr->date.day, expenseptr->date.month, expenseptr->date.year);
        expenseptr = expenseptr->next;
    }
    while (expenseptr) {
        fprintf(expenseFile, "\n%d %d %.2f %s %d %d %d",
                expenseptr->expense_id, expenseptr->user_id, expenseptr->expense_amount,
                expenseptr->category, expenseptr->date.day, expenseptr->date.month, expenseptr->date.year);
        expenseptr = expenseptr->next;
    }

    // Close files
    fclose(userFile);
    fclose(familyFile);
    fclose(expenseFile);

    printf("Database saved successfully!\n");
}

int main(){
    int flag = 0;
    int choice;
    status_code status_code;
    User*userList;
    Family*familyList;
    Expense*expenseList;
    Iniatialise_DB(&userList,&familyList,&expenseList);
    
    while (!flag){
        printf("\n---------FAMILY EXPENSE TRACKING SYSTEM--------- \n");
        printf("OPTIONS AVAILABLE:- \n");
        printf("1. Add a User \n");
        printf("2. Add an expense \n");
        printf("3. Update individual details (will affect in family also) \n");
        printf("4. Delete individual details (will affect in family also) \n");
        printf("5. Update  familly details \n");
        printf("6. Delete a family details \n");
        printf("7. Update an expense \n");
        printf("8. Delete an expense \n");
        printf("9. Get highest expense day for a family \n");
        printf("10. Get total family expense \n");
        printf("11. Get categorical expense for a particular category and family \n");
        printf("12. Get individual expense (also shows category-wise expenses) \n");
        printf("13. Display User Database \n");
        printf("14. Display Family Database \n");
        printf("15. Display Expense Database \n");
        printf("16. Exit \n");
        printf("Enter a choice (1/2/3/4/5/6/7/8/9/10/11/12/13/14/15): ");
        scanf("%d", &choice);
        printf("--------------------------------------------------- \n");
        if (choice == 1){
            User*newuser;
            status_code =CreateUserNode(userList,&newuser);
            if(status_code==0){
                status_code=AddUser(&userList,&familyList,newuser);
            }
            status_code_checker(status_code);
        }
        else if (choice == 2){
            Expense* newExpense;
            status_code=CreateExpenseNode(expenseList,userList,&newExpense);
            if(status_code==0){
                status_code=AddExpense(&expenseList,familyList,newExpense);
            }
            status_code_checker(status_code);
        }
        else if (choice == 3){
           User *existingUser=findUser(userList);
           status_code=updateUser(userList,familyList,existingUser);
           status_code_checker(status_code);
        }
        else if (choice == 4){
           User*existingUser=findUser(userList);
           status_code=deleteUser(&userList,&familyList,&expenseList,existingUser);
           status_code_checker(status_code);
        }
        else if (choice == 5){
            Family*existingFamily=findFamily(familyList);
            status_code=updateFamily(familyList,existingFamily);
            status_code_checker(status_code);
        }
        else if (choice == 6){
            Family*existingFamily=findFamily(familyList);
            status_code=deleteFamily(&userList,&familyList,&expenseList,existingFamily);
            status_code_checker(status_code);
        }
        else if (choice == 7){
            Expense*existingExpense=findExpense(expenseList);
            status_code=updateExpense(expenseList,familyList,existingExpense);
            status_code_checker(status_code);
        }
        else if (choice == 8){
            Expense*existingExpense=findExpense(expenseList);
            status_code=deleteExpense(&expenseList,familyList,existingExpense);
            status_code_checker(status_code);
        } 
        else if (choice == 9){
            Family*existingFamily=findFamily(familyList);
            int month,year;
            printf("enter the month and year :");
            scanf("%d%d",&month,&year);
            status_code=Get_highest_expense_day(existingFamily,expenseList,month,year);
           status_code_checker(status_code);
        }
        else if (choice == 10){
            Family*existingFamily=findFamily(familyList);
            int month,year;
            printf("enter the month and year :");
            scanf("%d%d",&month,&year);
            status_code=Get_total_expense(familyList,existingFamily,expenseList,month,year);
            status_code_checker(status_code);
        }
        else if(choice == 11){
            Family* existingFamily = findFamily(familyList);
            int category;
            printf("Enter the Category (1-Travel, 2-Leisure, 3-Rent, 4-Food, 5-Other) :");
            scanf("%d",&category);
            status_code=Get_categorcial_expense(expenseList,existingFamily,category);
            status_code_checker(status_code);
        }
        else if (choice == 12){
           User*existingUser=findUser(userList);
           int month,year;
           printf("enter the month and year :");
           scanf("%d%d",&month,&year);
           status_code= Get_individual_expense(expenseList,existingUser,month,year);
           status_code_checker(status_code);

        }
        else if (choice == 13){
            displayUsers(userList);
        }
        else if (choice == 14){
            displayFamily(familyList);
        }
        else if (choice == 15){
           displayExpenses(expenseList);
        }
        else if(choice ==16){
            printf("Do You Wish to Save this Database (1-Yes, 0-No) : ");
            scanf("%d", &choice);
            if(choice){
                SaveDB(&userList, &familyList, &expenseList);
            }
            flag = 1;
        }
        else {
            printf("Choice is INVALID. Please re-enter choice \n");
        }
    }
    return 0;
}
