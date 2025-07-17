/*
Expense Tracking System using B-Trees
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORDER 5
#define MAX_KEYS 4
#define MIN_KEYS 2
#define MAX_MEMBERS 4

typedef enum {false,true} boolean;
typedef enum {failure,success} status_code;

typedef struct Date_tag{
    int day;
    int month;
    int year;
}Date;

typedef struct UserNode_tag{
    int user_id;
    char user_name[50];
    float income;
    int family_id;
}userNode;

typedef struct FamilyNode_tag{
    int family_id;
    char family_name[50];
    int num_members;
    userNode* users[MAX_MEMBERS];
    float total_income;
    float total_expense;
}familyNode;

typedef struct ExpenseNode_tag{
    int combined_id; // user_id * 1000 + expense_id
    int user_id;
    int expense_id;
    userNode* user;
    Date date;
    float expense_amount;
	char category[10];
}expenseNode;

typedef struct UserTreeNode_tag{
    int num_keys;
    userNode* keys[ORDER];
    struct UserTreeNode_tag* children[ORDER];
}userTreeNode;

typedef struct FamilyTreeNode_tag{
    int num_keys;
    familyNode* keys[ORDER];
    struct FamilyTreeNode_tag* children[ORDER];
}familyTreeNode;

typedef struct ExpenseTreeNode_tag{
    int num_keys;
    expenseNode* keys[ORDER];
    struct ExpenseTreeNode_tag* children[ORDER];
}expenseTreeNode;

userNode* createUser(int user_id, char user_name[50], float income, int family_id){
    userNode* newUser = (userNode*)malloc(sizeof(userNode));
    if(!newUser){
        printf("Memory Allocation Error\n");
    }
    else{
        newUser -> user_id = user_id;
        strcpy(newUser->user_name,user_name);
        newUser -> income = income;
        newUser -> family_id = family_id;
    }
    return newUser;
}

expenseNode* createExpense(int user_id, int expense_id, Date date, float expense_amount, char category[10]){
    expenseNode* newExpense = (expenseNode*)malloc(sizeof(expenseNode));
    if(!newExpense){
        printf("Memory Allocation Error\n");
    }
    else{
        newExpense -> user_id = user_id;
        newExpense -> expense_id = expense_id;
        // Combined id will be used for comparision
        newExpense -> combined_id = (user_id * 1000) + expense_id;
        newExpense -> user = NULL;
        newExpense -> date = date;
        newExpense -> expense_amount = expense_amount;
        strcpy(newExpense -> category, category);
    }
    return newExpense;
}

// Functions to create User, Family and Expenses BTreeNodes
userTreeNode* createUserTreeNode(){
    userTreeNode* newTreeNode = (userTreeNode*)malloc(sizeof(userTreeNode));
    if(!newTreeNode){
        printf("Memory Allocation Error\n");
    }
    else{
        newTreeNode -> num_keys = 0;
        for(int i=0 ; i<ORDER ; i++){
            newTreeNode -> keys[i] = NULL;
            newTreeNode -> children[i] = NULL;
        }
    }
    return newTreeNode;
}

familyTreeNode* createFamilyTreeNode(){
    familyTreeNode* newTreeNode = (familyTreeNode*)malloc(sizeof(familyTreeNode));
    if(!newTreeNode){
        printf("Memory Allocation Error\n");
    }
    else{
        newTreeNode -> num_keys = 0;
        for(int i=0 ; i<ORDER ; i++){
            newTreeNode -> keys[i] = NULL;
            newTreeNode -> children[i] = NULL;
        }
    }
    return newTreeNode;
}

expenseTreeNode* createExpenseTreeNode(){
    expenseTreeNode* newTreeNode = (expenseTreeNode*)malloc(sizeof(expenseTreeNode));
    if(!newTreeNode){
        printf("Memory Allocation Error\n");
    }
    else{
        newTreeNode -> num_keys = 0;
        for(int i=0 ; i<ORDER ; i++){
            newTreeNode -> keys[i] = NULL;
            newTreeNode -> children[i] = NULL;
        }
    }
    return newTreeNode;
}

// Functions to Search Users, Families and Expenses
boolean SearchUserNode(int user_id, userTreeNode *current, int *pos){
    boolean ret_val;
    if (user_id < current->keys[1]->user_id){ 
        *pos = 0;
        ret_val = false;
    } 
    else{ 
        for(*pos = current->num_keys; (user_id < current->keys[*pos]->user_id) && *pos > 1; (*pos) --);
        ret_val = (user_id == (current->keys[*pos]->user_id));
    }
    return ret_val;
}

userNode* SearchUserTree(int user_id, userTreeNode* root, int *targetpos){
    userNode* ret_val;
    if (!root) ret_val = NULL;
    else if (SearchUserNode(user_id, root, targetpos))  ret_val = root->keys[*targetpos];
    else ret_val = SearchUserTree(user_id, root->children[*targetpos], targetpos);
    return ret_val;
}

boolean SearchFamilyNode(int family_id, familyTreeNode *current, int *pos){
    boolean ret_val;
    if (family_id < current->keys[1]->family_id){ 
        *pos = 0;
        ret_val = false;
    } 
    else{ 
        for(*pos = current->num_keys; (family_id < current->keys[*pos]->family_id) && *pos > 1; (*pos) --);
        ret_val = (family_id == (current->keys[*pos]->family_id));
    }
    return ret_val;
}

familyNode* SearchFamilyTree(int family_id, familyTreeNode* root, int *targetpos){
    familyNode* ret_val;
    if (!root) ret_val = NULL;
    else if (SearchFamilyNode(family_id, root, targetpos))  ret_val = root->keys[*targetpos];
    else ret_val = SearchFamilyTree(family_id, root->children[*targetpos], targetpos);
    return ret_val;
}

boolean SearchExpenseNode(int combined_id, expenseTreeNode *current, int *pos){
    boolean ret_val;
    if (combined_id < current->keys[1]->combined_id){ 
        *pos = 0;
        ret_val = false;
    } 
    else{ 
        for(*pos = current->num_keys; (combined_id < current->keys[*pos]->combined_id) && *pos > 1; (*pos) --);
        ret_val = (combined_id == (current->keys[*pos]->combined_id));
    }
    return ret_val;
}

expenseNode* SearchExpenseTree(expenseTreeNode* root, int expense_id) {
    expenseNode* result = NULL;
    int i = 1;
    if (root != NULL) {
        // Check all keys in the current node
        while (i <= root->num_keys && result == NULL) {
            if (root->keys[i]->expense_id == expense_id) {
                result = root->keys[i];
            }
            i++;
        }
        // Recursively search all children
        i = 0;
        while (i <= root->num_keys && result == NULL) {
            result = SearchExpenseTree(root->children[i], expense_id);
            i++;
        }
    }
    return result;
}

// userTree Insertion and Deletion in B-Tree functions

void PushInUser(userNode* medentry, userTreeNode* medright, userTreeNode* current, int pos){
    int i;
    for (i = current->num_keys; i > pos; i--) {
        current->keys[i + 1] = current->keys[i];
        current->children[i + 1] = current->children[i];
    }
    current->keys[pos + 1] = medentry;
    current->children[pos + 1] = medright;
    current->num_keys++;
}

void SplitUser(userNode* medentry, userTreeNode *medright, userTreeNode *current, int pos, userNode** newmedian, userTreeNode **newright){
    int i; 
    int median; 
    if (pos <= MIN_KEYS) median = MIN_KEYS;
    else median = MIN_KEYS + 1;
    *newright = createUserTreeNode();

    for (i = median + 1; i <= MAX_KEYS; i++) {  
        (*newright)->keys[i - median] = current->keys[i];
        (*newright)->children[i - median] = current->children[i];
    }

    (*newright)->num_keys = MAX_KEYS - median;
    current->num_keys = median;
    if(pos <= MIN_KEYS) PushInUser(medentry, medright, current, pos);
    else PushInUser(medentry, medright, *newright, pos - median);

    *newmedian = current->keys[current->num_keys];
    (*newright)->children[0] = current->children[current->num_keys];
    current->num_keys--;
}

boolean PushDownUser(userNode* newentry, userTreeNode* current, userNode** medentry,userTreeNode **medright){
    int pos; 
    boolean ret_val;
    if (current == NULL){ 
        *medentry = newentry;
        *medright = NULL;
        ret_val = true;
    } 
    else{ 
        if (SearchUserNode(newentry->user_id, current, &pos)){
            printf("Inserting duplicate key into B-tree - Not allowed\n");
            ret_val = false;
        }
        else if (PushDownUser(newentry, current->children[pos], medentry, medright)){
            if (current->num_keys < MAX_KEYS){ 
                PushInUser(*medentry, *medright, current, pos);
                ret_val = false;
            }
            else{
                SplitUser(*medentry, *medright, current, pos, medentry, medright);
                ret_val = true;
            }
        }
        else{
            ret_val = false;
        }
    }
    return ret_val;
}

userTreeNode *InsertUserTree(userNode* newentry, userTreeNode *root){
    userNode* medentry; 
    userTreeNode* medright; 
    userTreeNode* newroot; 
    userTreeNode* ret_val = root;
    if (PushDownUser(newentry, root, &medentry, &medright)){
        newroot = (userTreeNode*)malloc(sizeof(userTreeNode)); /* Make a new root. */
        newroot -> num_keys = 1;
        newroot -> keys[1] = medentry;
        newroot -> children[0] = root;
        newroot -> children[1] = medright;
        ret_val = newroot;
    }
    printf("Entered User having ID : %d\n",newentry->user_id);
    return ret_val;
}

void CombineUser(userTreeNode *current, int pos){
    int c;
    userTreeNode *right;
    userTreeNode  *left;
    right = current->children[pos];
    left  = current->children[pos-1]; 
    left->num_keys++; 
    left->keys[left->num_keys]  = current->keys [pos];
    left->children[left->num_keys] = right->children [0];

    for (c = 1;c <= right->num_keys;c++) { 
        left->num_keys++;
        left->keys[left->num_keys] = right->keys[c];
        left->children[left->num_keys] = right->children[c];
    }

    for (c = pos; c < current->num_keys;c++) { 
        current->keys [c] = current->keys [c + 1];
        current->children[c] = current->children [c + 1];
    }
    current->num_keys--;
    free(right);
}

void MoveLeftUser(userTreeNode *current, int pos){
    int c;
    userTreeNode* t;
    t = current->children[pos-1]; 
    t->num_keys++;
    t->keys[t->num_keys]  = current->keys[pos];
    t->children[t->num_keys] = current->children[pos] -> children[0];
    t = current->children [pos]; 
    current->keys[pos] = t->keys[1];
    t->children[0] = t->children [1];
    t->num_keys--;
    for (c = 1; c <= t->num_keys;c++) {
        t->keys[c] =t->keys[c + 1];
        t->children[c] =t->children[c +1];
    }
}

void MoveRightUser(userTreeNode *current, int pos){
    int c;
    userTreeNode* t;
    t = current->children[pos];
    for (c = t->num_keys; c > 0; c--){
        t->keys[c + 1] = t->keys[c];
        t->children[c + 1] = t->children [c];
    }
    t->children[1] = t->children[0]; 
    t->num_keys++;
    t->keys[1] = current->keys[pos];
    t = current->children [pos-1 ]; 
    current->keys[pos] = t->keys[t->num_keys];
    current->children[pos] -> children[0] = t->children[t->num_keys];
    t->num_keys--;
}

void RestoreUser(userTreeNode* current, int pos){
    if (pos == 0) {
        if (current->children[1] -> num_keys > MIN_KEYS) MoveLeftUser(current, 1);
        else CombineUser(current, 1);
    }
    else if (pos == current -> num_keys) {
        if (current->children[pos-1] -> num_keys > MIN_KEYS) MoveRightUser(current, pos);
        else CombineUser(current, pos);
    }
    else if (current->children [pos-1] -> num_keys > MIN_KEYS){
        MoveRightUser(current, pos);
    }
    else if (current->children [pos+1] -> num_keys > MIN_KEYS){
        MoveLeftUser(current, pos + 1);
    }
    else{
        CombineUser(current, pos);
    }
}

void SuccessorUser(userTreeNode *current, int pos){
    userTreeNode *leaf; 
    for (leaf = current->children [pos]; leaf->children [0]; leaf = leaf->children [0]);
    current->keys[pos] = leaf->keys[1];
}

void RemoveUser(userTreeNode *current, int pos) {
    int i; 
    for (i =pos + 1; i <= current->num_keys; i++) {
        current->keys[i-1] = current->keys[i];
        current->children [i-1] = current->children[i];
    }
    current->num_keys--;
}

void RecDeleteTreeUser(int user_id, userTreeNode* current) {
    int pos; 
    if(!current){
        printf("Requested User was not in the B-tree.\n");
    } 
    else {
        if(SearchUserNode(user_id, current, &pos)){
            if (current->children[pos-1]) {
                SuccessorUser(current, pos); 
                RecDeleteTreeUser(current->keys[pos]->user_id, current->children [pos]);
            }
            else { 
                RemoveUser(current, pos);
            }
        }
        else {
            RecDeleteTreeUser(user_id, current->children [pos]);
            if (current->children[pos]) {
                if (current->children[pos] ->num_keys < MIN_KEYS){
                    RestoreUser(current, pos);
                }
            }
        }
    }
}

userTreeNode *DeleteTreeUser(int user_id, userTreeNode *root){
    userTreeNode *oldroot; 
    RecDeleteTreeUser(user_id, root);
    if (root->num_keys == 0) {
        oldroot = root;
        root = root->children[0];
        free(oldroot);
    }
    printf("Deleted User having ID : %d\n",user_id);
    return root;
}

// familyTree Insertion and Deletion functions

void PushInFamily(familyNode* medentry, familyTreeNode* medright, familyTreeNode* current, int pos){
    int i;
    for (i = current->num_keys; i > pos; i--) {
        current->keys[i + 1] = current->keys[i];
        current->children[i + 1] = current->children[i];
    }
    current->keys[pos + 1] = medentry;
    current->children[pos + 1] = medright;
    current->num_keys++;
}

void SplitFamily(familyNode* medentry, familyTreeNode *medright, familyTreeNode *current, int pos, familyNode** newmedian, familyTreeNode **newright){
    int i; 
    int median; 
    if (pos <= MIN_KEYS) median = MIN_KEYS;
    else median = MIN_KEYS + 1;
    *newright = createFamilyTreeNode();

    for (i = median + 1; i <= MAX_KEYS; i++) {  
        (*newright)->keys[i - median] = current->keys[i];
        (*newright)->children[i - median] = current->children[i];
    }

    (*newright)->num_keys = MAX_KEYS - median;
    current->num_keys = median;
    if(pos <= MIN_KEYS) PushInFamily(medentry, medright, current, pos);
    else PushInFamily(medentry, medright, *newright, pos - median);

    *newmedian = current->keys[current->num_keys];
    (*newright)->children[0] = current->children[current->num_keys];
    current->num_keys--;
}

boolean PushDownFamily(familyNode* newentry, familyTreeNode* current, familyNode** medentry,familyTreeNode **medright){
    int pos; 
    boolean ret_val;
    if (current == NULL){ 
        *medentry = newentry;
        *medright = NULL;
        ret_val = true;
    } 
    else{ 
        if (SearchFamilyNode(newentry->family_id, current, &pos)){
            printf("Inserting duplicate key into B-tree - Not allowed\n");
            ret_val = false;
        }
        else if (PushDownFamily(newentry, current->children[pos], medentry, medright)){
            if (current->num_keys < MAX_KEYS){ 
                PushInFamily(*medentry, *medright, current, pos);
                ret_val = false;
            }
            else{
                SplitFamily(*medentry, *medright, current, pos, medentry, medright);
                ret_val = true;
            }
        }
        else{
            ret_val = false;
        }
    }
    return ret_val;
}

familyTreeNode *InsertFamilyTree(familyNode* newentry, familyTreeNode *root){
    familyNode* medentry; 
    familyTreeNode* medright; 
    familyTreeNode* newroot; 
    familyTreeNode* ret_val = root;
    if (PushDownFamily(newentry, root, &medentry, &medright)){
        newroot = (familyTreeNode*)malloc(sizeof(familyTreeNode)); /* Make a new root. */
        newroot -> num_keys = 1;
        newroot -> keys[1] = medentry;
        newroot -> children[0] = root;
        newroot -> children[1] = medright;
        ret_val = newroot;
    }
    printf("Enteed Family having ID : %d\n", newentry -> family_id);
    return ret_val;
}

void CombineFamily(familyTreeNode *current, int pos){
    int c;
    familyTreeNode* right;
    familyTreeNode* left;
    right = current->children[pos];
    left  = current->children[pos-1]; 
    left->num_keys++; 
    left->keys[left->num_keys]  = current->keys[pos];
    left->children[left->num_keys] = right->children[0];

    for (c = 1;c <= right->num_keys;c++) { 
        left->num_keys++;
        left->keys[left->num_keys] = right->keys[c];
        left->children[left->num_keys] = right->children[c];
    }

    for (c = pos; c < current->num_keys;c++) { 
        current->keys [c] = current->keys [c + 1];
        current->children[c] = current->children [c + 1];
    }
    current->num_keys--;
    free(right);
}

void MoveLeftFamily(familyTreeNode *current, int pos){
    int c;
    familyTreeNode* t;
    t = current->children[pos-1]; 
    t->num_keys++;
    t->keys[t->num_keys]  = current->keys[pos];
    t->children[t->num_keys] = current->children[pos] -> children[0];
    t = current->children [pos]; 
    current->keys[pos] = t->keys[1];
    t->children[0] = t->children [1];
    t->num_keys--;
    for (c = 1; c <= t->num_keys;c++) {
        t->keys[c] =t->keys[c + 1];
        t->children[c] =t->children[c +1];
    }
}

void MoveRightFamily(familyTreeNode *current, int pos){
    int c;
    familyTreeNode* t;
    t = current->children[pos];
    for (c = t->num_keys; c > 0; c--){
        t->keys[c + 1] = t->keys[c];
        t->children[c + 1] = t->children [c];
    }
    t->children[1] = t->children[0]; 
    t->num_keys++;
    t->keys[1] = current->keys[pos];
    t = current->children [pos-1]; 
    current->keys[pos] = t->keys[t->num_keys];
    current->children[pos] -> children[0] = t->children[t->num_keys];
    t->num_keys--;
}

void RestoreFamily(familyTreeNode* current, int pos){
    if (pos == 0) {
        if (current->children[1] -> num_keys > MIN_KEYS) MoveLeftFamily(current, 1);
        else CombineFamily(current, 1);
    }
    else if (pos == current -> num_keys) {
        if (current->children[pos-1] -> num_keys > MIN_KEYS) MoveRightFamily(current, pos);
        else CombineFamily(current, pos);
    }
    else if (current->children [pos-1] -> num_keys > MIN_KEYS){
        MoveRightFamily(current, pos);
    }
    else if (current->children [pos+1] -> num_keys > MIN_KEYS){
        MoveLeftFamily(current, pos + 1);
    }
    else{
        CombineFamily(current, pos);
    }
}

void SuccessorFamily(familyTreeNode *current, int pos){
    familyTreeNode *leaf; 
    for (leaf = current->children [pos]; leaf->children [0]; leaf = leaf->children [0]);
    current->keys[pos] = leaf->keys[1];
}

void RemoveFamily(familyTreeNode *current, int pos) {
    int i; 
    for (i =pos + 1; i <= current->num_keys; i++) {
        current->keys[i-1] = current->keys[i];
        current->children [i-1] = current->children[i];
    }
    current->num_keys--;
}

void RecDeleteTreeFamily(int family_id, familyTreeNode* current) {
    int pos; 
    if(!current){
        printf("Requested Family was not in the B-tree.\n");
    } 
    else {
        if(SearchFamilyNode(family_id, current, &pos)){
            if (current->children[pos-1]) {
                SuccessorFamily(current, pos); 
                RecDeleteTreeFamily(current->keys[pos]->family_id, current->children [pos]);
            }
            else { 
                RemoveFamily(current, pos);
            }
        }
        else {
            RecDeleteTreeFamily(family_id, current->children [pos]);
            if (current->children[pos]) {
                if (current->children[pos] ->num_keys < MIN_KEYS){
                    RestoreFamily(current, pos);
                }
            }
        }
    }
}

familyTreeNode *DeleteTreeFamily(int family_id, familyTreeNode *root){
    familyTreeNode *oldroot; 
    RecDeleteTreeFamily(family_id, root);
    if (root->num_keys == 0) {
        oldroot = root;
        root = root->children[0];
        free(oldroot);
    }
    printf("Deleted Family having ID : %d\n",family_id);
    return root;
}

// Expenses Insert and Delete from B-Tree

void PushInExpense(expenseNode* medentry, expenseTreeNode* medright, expenseTreeNode* current, int pos){
    int i;
    for (i = current->num_keys; i > pos; i--) {
        current->keys[i + 1] = current->keys[i];
        current->children[i + 1] = current->children[i];
    }
    current->keys[pos + 1] = medentry;
    current->children[pos + 1] = medright;
    current->num_keys++;
}

void SplitExpense(expenseNode* medentry, expenseTreeNode *medright, expenseTreeNode *current, int pos, expenseNode** newmedian, expenseTreeNode **newright){
    int i; 
    int median; 
    if (pos <= MIN_KEYS) median = MIN_KEYS;
    else median = MIN_KEYS + 1;
    *newright = createExpenseTreeNode();

    for (i = median + 1; i <= MAX_KEYS; i++) {  
        (*newright)->keys[i - median] = current->keys[i];
        (*newright)->children[i - median] = current->children[i];
    }

    (*newright)->num_keys = MAX_KEYS - median;
    current->num_keys = median;
    if(pos <= MIN_KEYS) PushInExpense(medentry, medright, current, pos);
    else PushInExpense(medentry, medright, *newright, pos - median);

    *newmedian = current->keys[current->num_keys];
    (*newright)->children[0] = current->children[current->num_keys];
    current->num_keys--;
}

boolean PushDownExpense(expenseNode* newentry, expenseTreeNode* current, expenseNode** medentry, expenseTreeNode** medright){
    int pos; 
    boolean ret_val;
    if (current == NULL){ 
        *medentry = newentry;
        *medright = NULL;
        ret_val = true;
    } 
    else{ 
        if (SearchExpenseNode(newentry->combined_id, current, &pos)){
            printf("Inserting duplicate key into B-tree - Not allowed\n");
            ret_val = false;
        }
        else if (PushDownExpense(newentry, current->children[pos], medentry, medright)){
            if (current->num_keys < MAX_KEYS){ 
                PushInExpense(*medentry, *medright, current, pos);
                ret_val = false;
            }
            else{
                SplitExpense(*medentry, *medright, current, pos, medentry, medright);
                ret_val = true;
            }
        }
        else{
            ret_val = false;
        }
    }
    return ret_val;
}

expenseTreeNode *InsertExpenseTree(expenseNode* newentry, expenseTreeNode *root){
    expenseNode* medentry; 
    expenseTreeNode* medright; 
    expenseTreeNode* newroot; 
    expenseTreeNode* ret_val = root;
    if (PushDownExpense(newentry, root, &medentry, &medright)){
        newroot = (expenseTreeNode*)malloc(sizeof(expenseTreeNode)); /* Make a new root. */
        newroot -> num_keys = 1;
        newroot -> keys[1] = medentry;
        newroot -> children[0] = root;
        newroot -> children[1] = medright;
        ret_val = newroot;
    }
    printf("Added Expense having ID : %d\n", newentry->expense_id); 
    return ret_val;
}

void CombineExpense(expenseTreeNode *current, int pos){
    int c;
    expenseTreeNode* right;
    expenseTreeNode* left;
    right = current->children[pos];
    left  = current->children[pos-1]; 
    left->num_keys++; 
    left->keys[left->num_keys]  = current->keys[pos];
    left->children[left->num_keys] = right->children[0];

    for (c = 1;c <= right->num_keys;c++) { 
        left->num_keys++;
        left->keys[left->num_keys] = right->keys[c];
        left->children[left->num_keys] = right->children[c];
    }

    for (c = pos; c < current->num_keys;c++) { 
        current->keys [c] = current->keys [c + 1];
        current->children[c] = current->children [c + 1];
    }
    current->num_keys--;
    free(right);
}

void MoveLeftExpense(expenseTreeNode *current, int pos){
    int c;
    expenseTreeNode* t;
    t = current->children[pos-1]; 
    t->num_keys++;
    t->keys[t->num_keys]  = current->keys[pos];
    t->children[t->num_keys] = current->children[pos] -> children[0];
    t = current->children [pos]; 
    current->keys[pos] = t->keys[1];
    t->children[0] = t->children [1];
    t->num_keys--;
    for (c = 1; c <= t->num_keys;c++) {
        t->keys[c] =t->keys[c + 1];
        t->children[c] =t->children[c +1];
    }
}

void MoveRightExpense(expenseTreeNode* current, int pos){
    int c;
    expenseTreeNode* t;
    t = current->children[pos];
    for (c = t->num_keys; c > 0; c--){
        t->keys[c + 1] = t->keys[c];
        t->children[c + 1] = t->children [c];
    }
    t->children[1] = t->children[0]; 
    t->num_keys++;
    t->keys[1] = current->keys[pos];
    t = current->children [pos-1]; 
    current->keys[pos] = t->keys[t->num_keys];
    current->children[pos] -> children[0] = t->children[t->num_keys];
    t->num_keys--;
}

void RestoreExpense(expenseTreeNode* current, int pos){
    if (pos == 0) {
        if (current->children[1] -> num_keys > MIN_KEYS) MoveLeftExpense(current, 1);
        else CombineExpense(current, 1);
    }
    else if (pos == current -> num_keys) {
        if (current->children[pos-1] -> num_keys > MIN_KEYS) MoveLeftExpense(current, pos);
        else CombineExpense(current, pos);
    }
    else if (current->children [pos-1] -> num_keys > MIN_KEYS){
        MoveLeftExpense(current, pos);
    }
    else if (current->children [pos+1] -> num_keys > MIN_KEYS){
        MoveLeftExpense(current, pos + 1);
    }
    else{
        CombineExpense(current, pos);
    }
}

void SuccessorExpense(expenseTreeNode *current, int pos){
    expenseTreeNode *leaf; 
    for (leaf = current->children[pos]; leaf->children[0]; leaf = leaf->children[0]);
    current->keys[pos] = leaf->keys[1];
}

void RemoveExpense(expenseTreeNode* current, int pos) {
    int i; 
    for (i =pos + 1; i <= current->num_keys; i++) {
        current->keys[i-1] = current->keys[i];
        current->children[i-1] = current->children[i];
    }
    current->num_keys--;
}

void RecDeleteTreeExpense(int combined_id, expenseTreeNode* current) {
    int pos; 
    if(!current){
        printf("Requested Family was not in the B-tree.\n");
    } 
    else {
        if(SearchExpenseNode(combined_id, current, &pos)){

            if (current->children[pos-1]) {
                SuccessorExpense(current, pos); 
                RecDeleteTreeExpense(current->keys[pos]->combined_id, current->children [pos]);
            }
            else { 
                RemoveExpense(current, pos);
            }
        }
        else {
            RecDeleteTreeExpense(combined_id, current->children [pos]);
            if (current->children[pos]) {
                if (current->children[pos] ->num_keys < MIN_KEYS){
                    RestoreExpense(current, pos);
                }
            }
        }
    }
}

expenseTreeNode* DeleteTreeExpense(int expense_id, expenseTreeNode *root){
    expenseTreeNode *oldroot; 
    expenseNode* node = SearchExpenseTree(root, expense_id);
    if(node){   
        int combined_id = node -> combined_id;
        RecDeleteTreeExpense(combined_id, root);
        if (root->num_keys == 0) {
            oldroot = root;
            root = root->children[0];
            free(oldroot);
        }
    }
    printf("Deleted Expense having ID : %d\n", expense_id);
    return root;
}

// function to add user
status_code addUser(userTreeNode* user_root, familyTreeNode* family_root, userTreeNode** userptr, familyTreeNode** familyptr){
    status_code sc = success;
    userNode* newuser = (userNode*)malloc(sizeof(userNode));
    if(!newuser){
        printf("Memory Allocation Error\n");
        sc = failure;
    }
    else{
        userNode* user;
        int user_id;
        int choice = 1;
        int flag = 0;
        while(choice && !flag){
            int target_pos = 0;
            printf("Enter the user-id: ");
            scanf("%d", &user_id);
            user = SearchUserTree(user_id, user_root, &target_pos);
            if(user){
                printf("Enter User-ID already exist in the database\n");
                printf("Do you wish to continue ? (1-yes, 0-no): ");
                scanf("%d", &choice);
            }
            else{
                flag = 1;
            }
        }
        if(choice && flag){
            newuser -> user_id = user_id;
            printf("Enter the user name: ");
            scanf("%49s", newuser -> user_name);
            printf("Enter the user-income: ");
            scanf("%f", &newuser -> income);
            int newFamilyChoice;
            printf("Does the user wish to enter an existing family or create new family ? \n");
            printf("1 - New, 0 - Existing: ");
            scanf("%d", &newFamilyChoice);
            if(newFamilyChoice){
                // Entering user in new family
                familyNode* newFamily = (familyNode*)malloc(sizeof(familyNode));
                int nchoice = 1, nflag = 0;
                int family_id;
                familyNode* family;
                while(nchoice && !nflag){
                    printf("Enter the family ID: ");
                    int target = 0;
                    scanf("%d", &family_id);
                    family = SearchFamilyTree(family_id,family_root,&target);
                    if(family){
                        printf("Family ID Entered Already exists, do you wish to continue ?\n");
                        printf("Choice (Yes-1, No-0): ");
                        scanf("%d", &nchoice);
                    }
                    else{
                        nflag = 1;
                    }
                }
                if(nchoice && nflag){
                    newFamily -> family_id = family_id;
                    printf("Enter the family name: ");
                    scanf("%49s", newFamily -> family_name);
                    newFamily -> num_members = 1;
                    newFamily -> total_expense = 0.0;
                    newFamily -> total_income = newuser -> income;
                    newFamily -> users[0] = newuser;
                    newuser -> family_id = newFamily -> family_id;
                    user_root = InsertUserTree(newuser,user_root);
                    family_root = InsertFamilyTree(newFamily,family_root);
                }
                else{
                    sc = failure;
                    free(newFamily);
                    free(newuser);
                }
            }
            else{
                // Entering user in existing family
                int target;
                int nchoice = 1, nflag = 0;
                familyNode* oldFamily;
                int family_id;
                while(nchoice && !nflag){
                    printf("Enter the family ID: ");
                    int target = 0;
                    scanf("%d", &family_id);
                    oldFamily = SearchFamilyTree(family_id,family_root,&target);
                    if(!oldFamily){
                        printf("Family ID doesnt exist, do you wish to continue ?\n");
                        printf("Choice (Yes-1, No-0): ");
                        scanf("%d", &nchoice);
                    }
                    else{
                        nflag = 1;
                    }
                }
                if(nchoice && nflag){
                    if(oldFamily -> num_members == MAX_MEMBERS){
                        printf("Family has maximum member capacity\n");
                        free(newuser);
                        sc = failure;
                    }
                    else{
                        oldFamily -> users[oldFamily -> num_members] = newuser;
                        oldFamily -> num_members++;
                        oldFamily -> total_income += newuser -> income;
                        newuser -> family_id = oldFamily -> family_id;
                        user_root = InsertUserTree(newuser, user_root);
                    }
                }
                else{
                    free(newuser);
                    sc = failure;
                }
            }
        }
        else{
            sc = failure;
            free(newuser);
        }
    }
    *userptr = user_root;
    *familyptr = family_root;
    return sc;
}

// function to add expense
status_code addExpense(userTreeNode* user_root, familyTreeNode* family_root, expenseTreeNode** expenseptr){
    status_code sc = success;
    expenseTreeNode* expense_root = *expenseptr;
    if(!user_root || !family_root){
        sc = failure;
    }
    else{
        expenseNode* newexpense = (expenseNode*)malloc(sizeof(expenseNode));
        int user_id, expense_id, family_id;
        int choice = 1, flag = 0;
        userNode* user;
        expenseNode* expense;
        while(choice && !flag){
            int target = 0;
            printf("Enter the user-id: ");
            scanf("%d", &user_id);
            user = SearchUserTree(user_id,user_root,&target);
            if(!user){
                printf("User-id Entered doesnt exist, do you wish to continue?\n");
                printf("Yes-1, No-0 : ");
                scanf("%d", &choice);
            }
            else{
                flag = 1;
                family_id = user -> family_id;
            }
        }
        if(choice && flag){
            int nchoice = 1, nflag = 0;
            while(nchoice && !nflag){
                printf("Enter the expense-id: ");
                scanf("%d", &expense_id);
                expense = SearchExpenseTree(expense_root,expense_id);
                if(expense){
                    printf("Entered expense-id is not unique, do you wish to continue? \n");
                    printf("yes-1, no-0 \n");
                    scanf("%d", &nchoice);
                }
                else{
                    nflag = 1;
                }
            }
            if(nchoice && nflag){
                newexpense -> expense_id = expense_id;
                newexpense -> user_id = user_id;
                newexpense -> user = user;
                newexpense -> combined_id = (user_id * 1000) + expense_id;
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
                printf("Enter the Amount of Expense: ");
                scanf("%f", &newexpense -> expense_amount);
                // Input expense date
                printf("Enter the date of the Expense in dd mm yyyy format: ");
                scanf("%d %d %d", &(newexpense->date.day), &(newexpense->date.month), &(newexpense->date.year)); 
                newexpense->date.day = (newexpense->date.day > 31)? 31 : newexpense->date.day;
                newexpense->date.month = (newexpense->date.month > 12)? 12 : newexpense->date.month;
                newexpense->date.year = (newexpense->date.year > 2025)? 2025 : newexpense->date.year;
                expense_root = InsertExpenseTree(newexpense,expense_root);
                int target_pos = 0;
                familyNode* family = SearchFamilyTree(family_id,family_root,&target_pos);
                family -> total_expense += newexpense -> expense_amount;
            }
            else{
                sc = failure;
                free(newexpense);
            }
        }
        else{
            sc = failure;
            free(newexpense);
        }
    }
    *expenseptr = expense_root;
    return sc;
}

status_code updateUser(userTreeNode* user_root, familyTreeNode* family_root){
    status_code sc = success;
    if(!user_root){
        printf("The User Database is Empty\n");
        sc = failure;
    }
    else{
        int choice = 1, flag = 0;
        int user_id;
        userNode* user;
        while(choice && !flag){
            int target = 0;
            printf("Enter the user-id\n");
            scanf("%d", &user_id);
            user = SearchUserTree(user_id,user_root,&target);
            if(!user){
                printf("User-id entered doesnt exist in the database, do you wish to continue?\n");
                printf("Yes - 1, No - 0: ");
                scanf("%d", &choice);
            }
            else{
                flag = 1;
            }
        }
        if(choice && flag){
            int target = 0;
            int old_income = user -> income;
            printf("Enter the New Name: ");
            scanf("%49s", user -> user_name);
            printf("Enter the new income : ");
            scanf("%f", &user -> income);
            familyNode* family = SearchFamilyTree(user -> family_id, family_root, &target);
            family -> total_income -= old_income;
            family -> total_income += user -> income;
        }
        else{
            sc = failure;
        }
    }
}

status_code updateFamily(familyTreeNode* family_root){
    status_code sc = success;
    if(!family_root){
        printf("The Family database is empty\n");
        sc = failure;
    }
    else{
        int choice = 1, flag = 0;
        int family_id;
        familyNode* family;
        while(choice && !flag){
            int target = 0;
            printf("Enter the family-id: ");
            scanf("%d",&family_id);
            family = SearchFamilyTree(family_id,family_root,&target);
            if(!family){
                printf("Entered family-id doesnt exist in the database, do you wish to continue?\n");
                printf("yes - 1, no - 0 : ");
                scanf("%d", &choice);
            }
            else{
                flag = 1;
            }
        }
        if(choice && flag){
            printf("Enter new family-name: ");
            scanf("%49s", family -> family_name);
        }
        else{
            sc = failure;
        }
    }
    return sc;
}

status_code updateExpense(userTreeNode* user_root, familyTreeNode* family_root, expenseTreeNode* expense_root){
    status_code sc = success;
    if(!expense_root || !user_root || !family_root){
        printf("Expense Database is empty\n");
        sc = failure;
    }
    else{
        int choice = 1, flag = 0;
        userNode* user;
        familyNode* family;
        expenseNode* expense;
        int expense_id;
        while(choice && !flag){
            printf("Enter the expense-id: ");
            scanf("%d", &expense_id);
            expense = SearchExpenseTree(expense_root,expense_id);
            if(!expense){
                printf("The entered expense-id doesnt exist in the database, do you wish to continue?\n");
                printf("Yes - 1, No - 0: ");
                scanf("%d", &choice);
            }
            else{
                flag = 1;
            }
        }
        if(choice && flag){
            float old_expense = expense -> expense_amount;
            user = expense -> user;
            int target = 0;
            family = SearchFamilyTree(user->family_id,family_root,&target);
            printf("Enter new expense: ");
            scanf("%f", &expense -> expense_amount);
            family -> total_expense -= old_expense;
            family -> total_expense += expense -> expense_amount;
        }
        else{
            sc = failure;
        }
    }
    return sc;
}

int findFirstExpenseByUserId(expenseTreeNode* root, int user_id) {
    int result = -1; // Default value if no matching node is found
    int i = 1;

    if (root != NULL) {
        // Check all keys in the current node
        while (i <= root->num_keys && result == -1) {
            if (root->keys[i]->user_id == user_id) {
                result = root->keys[i]->expense_id;
            }
            i++;
        }

        // Recursively search all children
        i = 0;
        while (i <= root->num_keys && result == -1) {
            result = findFirstExpenseByUserId(root->children[i], user_id);
            i++;
        }
    }

    return result;
}

status_code deleteUser(userTreeNode** userptr, familyTreeNode** familyptr, expenseTreeNode** expenseptr){
    status_code sc = success;
    userTreeNode* user_root = *userptr;
    familyTreeNode* family_root = *familyptr;
    expenseTreeNode* expense_root = *expenseptr;
    if(!user_root){
        printf("The user database is empty\n");
        sc = failure;
    }
    else{
        int choice = 1, flag = 0;
        int user_id;
        userNode* user;
        familyNode* family;
        expenseNode* expense;
        while(choice && !flag){
            printf("Enter the user-id: ");
            scanf("%d", &user_id);
            int target = 0;
            user = SearchUserTree(user_id,user_root,&target);
            if(!user){
                printf("Entered user-id doesnt exist - (do you wish to continue?)\n");
                printf("Yes - 1, No - 0: ");
                scanf("%d", &choice);
            }
            else{
                flag = 1;
            }
        }
        if(choice && flag){
            int target = 0;
            family = SearchFamilyTree(user -> family_id,family_root,&target);
            family -> total_income -= user -> income;
            int done = 0;
            while(!done){
                // Remove all expenses of user
                int expense_id = findFirstExpenseByUserId(expense_root,user_id);
                if(expense_id != -1){
                    expense = SearchExpenseTree(expense_root,expense_id);
                    family -> total_expense -= expense -> expense_amount;
                    expense_root = DeleteTreeExpense(expense_id,expense_root);
                }
                else{
                    done = 1;
                }
            }
            if(family -> num_members == 1){
                // Delete family entirely
                family_root = DeleteTreeFamily(user -> family_id, family_root);
            }
            else{
                int i;
                for(i=0 ; i<family->num_members && family -> users[i] -> user_id != user_id; i++);
                for(i; i<family -> num_members-1; i++){
                    family -> users[i] = family -> users[i+1];
                }
                family -> num_members--;
            }
            user_root = DeleteTreeUser(user_id,user_root);
        }
        else{
            sc = failure;
        }
    }
    *userptr = user_root;
    *familyptr = family_root;
    *expenseptr = expense_root;
    return sc;
}

status_code deleteExpense(expenseTreeNode** expenseptr, familyTreeNode* family_root){
    expenseTreeNode* expense_root = *expenseptr;
    status_code sc = success;
    if(!expense_root){
        printf("The expense databse is empty\n");
        sc = failure;
    }
    else{
        int choice = 1, flag = 0;
        int expense_id;
        userNode* user;
        familyNode* family;
        expenseNode* expense;
        while(choice && !flag){
            printf("Enter the expense-id: ");
            scanf("%d", &expense_id);
            expense = SearchExpenseTree(expense_root,expense_id);
            if(!expense){
                printf("The given expense-id doesnt exist in the database, do youy wish to continue?\n");
                printf("Yes - 1, No - 0: ");
                scanf("%d", &choice);
            }
            else{
                flag = 1;
            }
        }
        if(choice && flag){
            user = expense -> user;
            int target = 0;
            family = SearchFamilyTree(user -> family_id,family_root,&target);
            family -> total_expense -= expense -> expense_amount;
            expense_root = DeleteTreeExpense(expense_id,expense_root);
        }
        else{
            sc = failure;
        }
    }
    *expenseptr = expense_root;
    return sc;
}

status_code deleteFamily(userTreeNode** userptr, familyTreeNode** familyptr, expenseTreeNode** expenseptr){
    userTreeNode* user_root = *userptr;
    familyTreeNode* family_root = *familyptr;
    expenseTreeNode* expense_root = *expenseptr;
    status_code sc = success;
    if(!family_root){
        printf("The Family Database is empty\n");
        sc = failure;
    }
    else{
        int choice = 1, flag = 0;
        userNode* user;
        familyNode* family;
        expenseNode* expense;
        int expense_id;
        int family_id;
        while(choice && !flag){
            printf("Enter the family-id: ");
            scanf("%d", &family_id);
            int target = 0;
            family = SearchFamilyTree(family_id,family_root,&target);
            if(!family){
                printf("The entered family-id doenst exist, do you wish to continue?\n");
                printf("Yes-1, No-0 : ");
                scanf("%d", &choice);
            }
            else{
                flag = 1;
            }
        }
        if(choice && flag){
            // Deleting all expenses of all users first
	    int num_members = family -> num_members;
            for(int i=0 ; i<num_members ; i++){
                int done = 0;
                while(!done){
                    expense_id = findFirstExpenseByUserId(expense_root,family->users[i]->user_id);
                    if(expense_id != -1){
                        expense = SearchExpenseTree(expense_root,expense_id);
                        expense_root = DeleteTreeExpense(expense_id,expense_root);
                    }
                    else{
                        done = 1;
                    }
                }
                user_root = DeleteTreeUser(family->users[i]->user_id,user_root);
            }
            family_root = DeleteTreeFamily(family->family_id,family_root);
        }
        else{
            sc = failure;
        }
    }
    *userptr = user_root;
    *familyptr = family_root;
    *expenseptr = expense_root;
    return sc;
}

// Helper function to recursively calculate total expenses for a given family and month
void calculateFamilyExpenses(expenseTreeNode* expense_root, int user_id, int family_id, int month, int year, float* sum) {
    int i = 1;

    if (expense_root != NULL) {
        // Check all keys in the current node
        while (i <= expense_root->num_keys) {
            if (expense_root->keys[i]->user_id == user_id && expense_root->keys[i]->date.month == month && expense_root->keys[i]->date.year == year) {
                *sum += expense_root->keys[i]->expense_amount;
            }
            i++;
        }

        // Recursively search all children
        i = 0;
        while (i <= expense_root->num_keys) {
            calculateFamilyExpenses(expense_root->children[i], user_id, family_id, month, year, sum);
            i++;
        }
    }
}

// Main function to calculate total expenses for a family and check debt/profit
status_code getTotalExpense(familyTreeNode* family_root, expenseTreeNode* expense_root) {
    status_code sc = success;
    if(!family_root){
        printf("The family database is empty\n");
        sc = failure;
    }
    else{
        float total_expense = 0.0;
        float family_income = 0.0;
        familyNode* family = NULL;
        int target = 0;

        int choice = 1, flag = 0;
        int family_id;
        while(choice && !flag){
            printf("Enter the family_id: ");
            scanf("%d", &family_id);
            family = SearchFamilyTree(family_id,family_root,&target);
            if(!family){
                printf("The family-id entered doesnt exist in the database, do you wish to continue?\n");
                printf("Yes-1, No-0 \n");
                scanf("%d",&choice);
            }
            else{
                flag = 1;
            }
        }
        if(choice && flag){
            int month, year;
            printf("Enter the month : ");
            scanf("%d ", &month);
            printf("Enter the year: ");
            scanf("%d", &year);
            family_income = family->total_income;
            for (int i = 0; i < family->num_members; i++) {
                int user_id = family->users[i]->user_id;
                calculateFamilyExpenses(expense_root, user_id, family_id, month, year, &total_expense);
            }
            printf("Total Expense for Family ID %d in %02d/%04d: %.2f\n", family_id, month, year, total_expense);
            if (total_expense > family_income) {
                printf("The family is in debt. Expense surpasses income by %.2f\n", total_expense - family_income);
            } else {
                printf("The family is within budget. Remaining income: %.2f\n", family_income - total_expense);
            }
        }
        else{
            sc = failure;
        }
    }
    return sc;
}

void getExpenseInPeriod(expenseTreeNode* expense_root, Date date1, Date date2, int* flag) {
    if (expense_root != NULL) {
        int i = 1;

        while (i <= expense_root->num_keys) {
            Date expense_date = expense_root->keys[i]->date;

            if ((expense_date.year > date1.year || 
                (expense_date.year == date1.year && expense_date.month > date1.month) || 
                (expense_date.year == date1.year && expense_date.month == date1.month && expense_date.day >= date1.day)) &&
                (expense_date.year < date2.year || 
                (expense_date.year == date2.year && expense_date.month < date2.month) || 
                (expense_date.year == date2.year && expense_date.month == date2.month && expense_date.day <= date2.day))) {
                printf("Expense ID: %d, User ID: %d, Date: %02d/%02d/%04d, Amount: %.2f, Category: %s\n",
                       expense_root->keys[i]->expense_id,
                       expense_root->keys[i]->user_id,
                       expense_date.day,
                       expense_date.month,
                       expense_date.year,
                       expense_root->keys[i]->expense_amount,
                       expense_root->keys[i]->category);
                *flag = 1;
            }
            i++;
        }

        i = 0;
        while (i <= expense_root->num_keys) {
            getExpenseInPeriod(expense_root->children[i], date1, date2, flag);
            i++;
        }
    }
}

void getExpenseInRange(expenseTreeNode* expense_root, int expense_id1, int expense_id2, int user_id, int* flag){
    int combined_id1 = (user_id * 1000) + expense_id1;
    int combined_id2 = (user_id * 1000) + expense_id2;
    if(expense_root != NULL){
        int i = 1;

        while(i <= expense_root -> num_keys){
            if((expense_root -> keys[i] -> combined_id >= combined_id1) && 
            (expense_root -> keys[i] -> combined_id <= combined_id2)){
                printf("Expense ID: %d, User ID: %d, Amount: %.2f, Category: %s\n",
                    expense_root->keys[i]->expense_id,
                    expense_root->keys[i]->user_id,
                    expense_root->keys[i]->expense_amount,
                    expense_root->keys[i]->category);
                *flag = 1;
            }
            i++;
        }

        i = 0;
        while(i <= expense_root->num_keys){
            getExpenseInRange(expense_root->children[i],expense_id1,expense_id2,user_id,flag);
            i++;
        }
    }
}

void searchExpenseCategoryUser(expenseTreeNode* expense_root, int user_id, char category[15], float* amount_arr, int pos){
    if(expense_root != NULL){
        int i = 1;
        while(i <= expense_root -> num_keys){
            if(expense_root->keys[i]->user_id == user_id && strcmp(expense_root->keys[i]->category,category) == 0){
                amount_arr[pos] += expense_root->keys[i]->expense_amount;
            }
            i++;
        }

        i=0;
        while(i <= expense_root->num_keys){
            searchExpenseCategoryUser(expense_root->children[i],user_id,category,amount_arr,pos);
            i++;
        }
    }
}

status_code getCategoricalExpense(expenseTreeNode* expense_root, familyTreeNode* family_root){
    status_code sc = success;
    if(!family_root){
        sc = failure;
        printf("The Family database is empty\n");
    }
    else{
        int family_id, expense_id;
        familyNode* family;
        expenseNode* expense;
        int target = 0;

        int choice = 1, flag = 0;
        while(choice && !flag){
            printf("Enter the family_id: ");
            scanf("%d", &family_id);
            family = SearchFamilyTree(family_id,family_root,&target);
            if(!family){
                printf("The family-id entered doesnt exist in the database, do you wish to continue?\n");
                printf("Yes-1, No-0 \n");
                scanf("%d",&choice);
            }
            else{
                flag = 1;
            }
        }
        if(choice && flag){
            int category_no;
            char category[15];
            printf("Enter the Category (1-Travel, 2-Leisure, 3-Rent, 4-Food, 5-Other) :");
            scanf("%d",&category_no);
            switch(category_no){
                case 1:
                    strcpy(category, "Travel"); break;
                case 2:
                    strcpy(category, "Leisure"); break;
                case 3:
                    strcpy(category, "Rent"); break;
                case 4:
                    strcpy(category, "Food"); break;
                default:
                    strcpy(category, "Other"); break;
            }
            float total_sum = 0.0;
            int* user_ids = (int*)malloc(sizeof(int) * family -> num_members);
            float* amount_arr = (float*)malloc(sizeof(float) * family -> num_members);
            for(int i=0 ; i<family->num_members ; i++){
                amount_arr[i] = 0;
                user_ids[i] = family -> users[i] -> user_id;
                searchExpenseCategoryUser(expense_root,user_ids[i],category,amount_arr,i);
                total_sum += amount_arr[i];
            }
            int temp;
            float tempf;
            // Sorting in Descending order
            for(int i=0 ; i<family->num_members-1 ; i++){
                for(int j=0 ; j<family->num_members-i-1 ; j++){
                    if(amount_arr[j] < amount_arr[j+1]){
                        tempf = amount_arr[j];
                        amount_arr[j] = amount_arr[j+1];
                        amount_arr[j+1] = tempf;
                        temp = user_ids[j];
                        user_ids[j] = user_ids[j+1];
                        user_ids[j+1] = temp;
                    }
                }
            }
            printf("The total Category expense for Family (ID : %d) is : %f \n", family->family_id, total_sum);
            printf("Member-wise contribution to category Expenses:- \n");
            for(int i=0 ; i < family -> num_members ; i++){
                printf("User ID : %d, Expense Amount : %.2f \n", user_ids[i], amount_arr[i]);
            }
        }
        else{
            sc = failure;
        }
    }
    return sc;
}

void findDayWiseExpenses(expenseTreeNode* expense_root, float amounts[], int family_id, int month, int year){
    if(expense_root){
        int i = 1;
        for(i ; i<=expense_root->num_keys ; i++){
            if((expense_root->keys[i]->user->family_id == family_id)
             && (expense_root->keys[i]->date.month == month)
             && (expense_root->keys[i]->date.year == year)){
                amounts[expense_root->keys[i]->date.day] += expense_root->keys[i]->expense_amount;
            }
        }

        for(i=0 ; i<=expense_root->num_keys ; i++){
            findDayWiseExpenses(expense_root->children[i],amounts,family_id,month,year);
        }
    }
}

status_code getHighestExpenseDay(expenseTreeNode* expense_root, familyTreeNode* family_root){
    status_code sc = success;
    if(!family_root){
        sc = failure;
        printf("The family database is empty\n");
    }
    else{
        int family_id, expense_id;
        familyNode* family;
        expenseNode* expense;
        int target = 0;

        int choice = 1, flag = 0;
        while(choice && !flag){
            printf("Enter the family_id: ");
            scanf("%d", &family_id);
            family = SearchFamilyTree(family_id,family_root,&target);
            if(!family){
                printf("The family-id entered doesnt exist in the database, do you wish to continue?\n");
                printf("Yes-1, No-0 \n");
                scanf("%d",&choice);
            }
            else{
                flag = 1;
            }
        }
        if(choice && flag){
            int month;
            printf("Enter the month: ");
            scanf("%d", &month);
            int year;
            printf("Enter the year: ");
            scanf("%d", &year);
            float amounts[32];
            for(int i=0 ; i<32 ; i++){
                amounts[i] = 0.0;
            }
            findDayWiseExpenses(expense_root,amounts,family_id,month,year);
            float max_amount =  0.0;
            int max_date = 0;
            for(int i=0 ; i<32 ; i++){
                if(amounts[i] > max_amount){
                    max_amount = amounts[i];
                    max_date = i;
                }
            }
            if(max_amount == 0.0){
                printf("No expense by the family in the given month/year\n");
            }
            else{
                printf("Max Amount %.2f was spent on date %02d/%02d/%04d \n", max_amount, max_date, month, year);
            }
        }
        else{
            sc = failure;
        }
    }
    return sc;
}

void findIndividualExpenses(expenseTreeNode* expense_root, int user_id, float expenses[]){
    if(expense_root){
        for(int i=1 ; i<=expense_root->num_keys ; i++){
            if(expense_root->keys[i]->user_id == user_id){
                if(strcmp("Travel",expense_root->keys[i]->category) == 0){
                    expenses[0] += expense_root->keys[i]->expense_amount;
                }
                else if(strcmp("Leisure",expense_root->keys[i]->category) == 0){
                    expenses[1] += expense_root->keys[i]->expense_amount;
                }
                else if(strcmp("Rent",expense_root->keys[i]->category) == 0){
                    expenses[2] += expense_root->keys[i]->expense_amount;
                }
                else if(strcmp("Food",expense_root->keys[i]->category) == 0){
                    expenses[3] += expense_root->keys[i]->expense_amount;
                }
                else{
                    expenses[4] += expense_root->keys[i]->expense_amount;
                }
            }
        }

        for(int i=0 ; i<=expense_root->num_keys ; i++){
            findIndividualExpenses(expense_root->children[i],user_id,expenses);
        }
    }
}

status_code getIndividualExpense(userTreeNode* user_root, expenseTreeNode* expense_root){
    status_code sc = success;
    if(!user_root){
        printf("The User database is empty\n");
        sc = failure;
    }
    else{
        int user_id;
        userNode* user;
        int choice = 1, flag = 0;
        while(choice && !flag){
            int target = 0;
            printf("Enter the user-id :");
            scanf("%d",&user_id);
            user = SearchUserTree(user_id,user_root,&target);
            if(!user){
                printf("The user-id entered doesnt exist, do you wish to continue?\n");
                printf("Yes-1, No-0 : ");
                scanf("%d", &choice);
            }
            else{
                flag = 1;
            }
        }
        if(choice && flag){
            char *expenseNames[5] = {"Travel", "Leisure", "Rent", "Food", "Other"};
            float CategoricalExpenses[5]={0.0,0.0,0.0,0.0,0.0};
            findIndividualExpenses(expense_root,user_id,CategoricalExpenses);
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
            printf("The Category Wise Expenses of the User with Id %d are \n",user_id);
            for(int i=0;i<5;i++){
                printf("Category : %s || Amount : %f \n",expenseNames[i],CategoricalExpenses[i]);
            }
        }
        else{
            sc = failure;
        }
    }
    return sc;
}

void inorderUser(userTreeNode* user_root){
    if(user_root){
        for (int i = 0; i < user_root->num_keys; i++) {
            inorderUser(user_root->children[i]);
            userNode* user = user_root->keys[i+1];
            printf("User-ID : %d, Name : %s, Income : %.2f \n", user->user_id, user->user_name, user->income);
        }
        inorderUser(user_root->children[user_root->num_keys]);
    }
}

void inorderFamily(familyTreeNode* family_root){
    if(family_root){
        for(int i=0 ; i < family_root->num_keys ; i++){
            inorderFamily(family_root->children[i]);
            familyNode* family = family_root->keys[i+1];
            printf("\nFamily-ID : %d, Family Name : %s, Total Members : %d\n",family->family_id,family->family_name,family->num_members);
            printf("Family Members : [ ");
            for(int i=0 ; i<family->num_members ; i++){
                printf("%d ", family->users[i]->user_id);
            }
            printf("]\nTotal Income : %.2f, Total expense : %.2f\n",family->total_income,family->total_expense);
        }
        inorderFamily(family_root->children[family_root->num_keys]);
    }
}

void inorderExpense(expenseTreeNode* expense_root){
    if(expense_root){
        for(int i=0 ; i<expense_root->num_keys ; i++){
            inorderExpense(expense_root->children[i]);
            expenseNode* expense = expense_root->keys[i+1];
            printf("\nExpense-ID : %d, User-ID : %d, Amount : %.2f\n",expense->expense_id,expense->user_id,expense->expense_amount);
            printf("Date : %02d/%02d/%04d , Cateogry : %s\n",expense->date.day,expense->date.month,expense->date.year,expense->category);
        }
        inorderExpense(expense_root->children[expense_root->num_keys]);
    }
}

void statusCodeChecker(status_code sc){
    if(sc == failure){
        printf("The Operation was a Failure\n");
    }
    else{
        printf("The Operation was a Success\n");
    }
}

void initializeDB(userTreeNode** userptr, familyTreeNode** familyptr, expenseTreeNode** expenseptr){
    userTreeNode* user_root = *userptr;
    familyTreeNode* family_root = *familyptr;
    expenseTreeNode* expense_root = *expenseptr;
    user_root = NULL;
    family_root = NULL;
    expense_root = NULL;
    FILE *userFile, *familyFile, *expenseFile;

    // Open files
    userFile = fopen("user.txt", "r");
    familyFile = fopen("family.txt", "r");
    expenseFile = fopen("expense.txt", "r");

    while(!feof(userFile)){
        userNode* newUser = (userNode*)malloc(sizeof(userNode));
        fscanf(userFile,"%d %s %f", &newUser->user_id, newUser->user_name, &newUser->income);
        user_root = InsertUserTree(newUser,user_root);
    }

    while(!feof(expenseFile)){
        int target = 0;
        expenseNode* newExpense = (expenseNode*)malloc(sizeof(expenseNode));
        fscanf(expenseFile, "%d %d %f %s %d %d %d",
            &newExpense->expense_id, &newExpense->user_id, &newExpense->expense_amount,
            newExpense->category, &newExpense->date.day, &newExpense->date.month, &newExpense->date.year);
        newExpense -> user = SearchUserTree(newExpense->user_id,user_root,&target);
        newExpense -> combined_id = ((newExpense -> user_id) * 1000) + (newExpense -> expense_id);
        expense_root = InsertExpenseTree(newExpense,expense_root);
    }

    while(!feof(familyFile)){
        familyNode* newFamily = (familyNode*)malloc(sizeof(familyNode));
        newFamily -> total_expense = 0.0;
        newFamily -> total_income = 0.0;
        newFamily -> num_members = 0;
        int user_ids[MAX_MEMBERS];
        int target = 0;
        fscanf(familyFile, "%d %s %d %d %d %d", &newFamily -> family_id, newFamily -> family_name,
            &user_ids[0], &user_ids[1], &user_ids[2], &user_ids[3]);
        for(int i=0 ; i<MAX_MEMBERS ; i++){
            if(user_ids[i] != 0){
                float expenses[] = {0.0,0.0,0.0,0.0,0.0};
                newFamily -> users[i] = SearchUserTree(user_ids[i],user_root,&target);
                newFamily -> total_income += newFamily -> users[i] -> income;
                newFamily -> num_members++;
                newFamily -> users[i] -> family_id = newFamily -> family_id;
                findIndividualExpenses(expense_root,newFamily->users[i]->user_id,expenses);
                for(int i=0 ; i<5 ; i++){
                    newFamily -> total_expense += expenses[i];
                }
            }
        }
        family_root = InsertFamilyTree(newFamily,family_root);
    }

    // Close files
    fclose(userFile);
    fclose(familyFile);
    fclose(expenseFile);

    *userptr = user_root;
    *familyptr = family_root;
    *expenseptr = expense_root;
}

void freeDatabase(userTreeNode* user_root, familyTreeNode* family_root, expenseTreeNode* expense_root) {
    if (user_root) {
        for (int i = 0; i <= user_root->num_keys; i++) {
            freeDatabase(user_root->children[i], NULL, NULL);
        }
        for (int i = 1; i <= user_root->num_keys; i++) {
            free(user_root->keys[i]);
        }
        free(user_root);
    }

    if (family_root) {
        for (int i = 0; i <= family_root->num_keys; i++) {
            freeDatabase(NULL, family_root->children[i], NULL);
        }
        for (int i = 1; i <= family_root->num_keys; i++) {
            free(family_root->keys[i]);
        }
        free(family_root);
    }

    if (expense_root) {
        for (int i = 0; i <= expense_root->num_keys; i++) {
            freeDatabase(NULL, NULL, expense_root->children[i]);
        }
        for (int i = 1; i <= expense_root->num_keys; i++) {
            free(expense_root->keys[i]);
        }
        free(expense_root);
    }
}

int main(){
    userTreeNode* user_root;
    familyTreeNode* family_root;
    expenseTreeNode* expense_root;
    initializeDB(&user_root,&family_root,&expense_root);
    int done = 0;
    int flag;
    status_code sc;
    int choice;
    while(!done){
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
        printf("13. Display expenses in period (date1 to date2) \n");
        printf("14. Display expenses in range for a user (expenseid1 to expenseid2)  \n");
        printf("15. Display User Database \n");
        printf("16. Display Family Database \n");
        printf("17. Display Expense Database \n");
        printf("18. Exit \n");
        printf("Enter a choice (1/2/3/4/5/6/7/8/9/10/11/12/13/14/15/16/17/18): ");
        scanf("%d", &choice);
        printf("--------------------------------------------------- \n");
        switch(choice){
            case 1:
                sc = addUser(user_root,family_root,&user_root,&family_root);
                statusCodeChecker(sc);
                break;
            case 2:
                sc = addExpense(user_root,family_root,&expense_root);
                statusCodeChecker(sc);
                break;
            case 3:
                sc = updateUser(user_root,family_root);
                statusCodeChecker(sc);
                break;
            case 4:
                sc = deleteUser(&user_root,&family_root,&expense_root);
                statusCodeChecker(sc);
                break;
            case 5:
                sc = updateFamily(family_root);
                statusCodeChecker(sc);
                break;
            case 6:
                sc = deleteFamily(&user_root,&family_root,&expense_root);
                statusCodeChecker(sc);
                break;
            case 7:
                sc = updateExpense(user_root,family_root,expense_root);
                statusCodeChecker(sc);
                break;
            case 8:
                sc = deleteExpense(&expense_root,family_root);
                statusCodeChecker(sc);
                break;
            case 9:
                sc = getHighestExpenseDay(expense_root,family_root);
                statusCodeChecker(sc);
                break;
            case 10:
                sc = getTotalExpense(family_root,expense_root);
                statusCodeChecker(sc);
                break;
            case 11:
                sc = getCategoricalExpense(expense_root,family_root);
                statusCodeChecker(sc);
                break;
            case 12:
                sc = getIndividualExpense(user_root,expense_root);
                statusCodeChecker(sc);
                break;
            case 13:
                if(!expense_root) {
                    printf("The Expense database is empty\n");
                }
                else{
                    Date d1, d2;
                    flag = 0;
                    printf("Enter the date1 in dd mm yyyy format : ");
                    scanf("%d %d %d", &d1.day, &d1.month, &d1.year);
                    printf("Enter the date2 in dd mm yyyy format : ");
                    scanf("%d %d %d", &d2.day, &d2.month, &d2.year);
                    getExpenseInPeriod(expense_root,d1,d2,&flag);
                    if(!flag){
                        printf("No expense found in given range\n");
                    }
                }
                break;
            case 14:
                if(!expense_root){
                    printf("The Expense database is empty\n");
                }
                else{
                    int expense_id1, expense_id2, user_id;
                    flag = 0;
                    printf("Enter the user-id : ");
                    scanf("%d", &user_id);
                    printf("Enter the expense-id-1 : ");
                    scanf("%d",&expense_id1);
                    printf("Enter the expense-id-2 : ");
                    scanf("%d", &expense_id2);
                    getExpenseInRange(expense_root,expense_id1,expense_id2,user_id,&flag);
                    if(!flag){
                        printf("No expense in given range / given user-id is invalid\n");
                    }
                }
                break;
            case 15:
                printf("USER DATABASE\n");
                if(!user_root){
                    printf("The databse is empty\n");
                }
                else{
                    inorderUser(user_root);
                }
                break;
            case 16:
                printf("FAMILY DATABASE\n");
                if(!family_root){
                    printf("THe database is empty");
                }
                else{
                    inorderFamily(family_root);
                }
                break;
            case 17:
                printf("EXPENSE DATABASE\n");
                if(!expense_root){
                    printf("The databse is empty\n");
                }
                else{
                    inorderExpense(expense_root);                
                }
                break;
            case 18:
                done = 1;
                freeDatabase(user_root,family_root,expense_root);
                break;
            default:
                printf("Wrong Choice Entered !!! Please Re-enter your choice\n");
                break;
        }
    }
    return 0;
}