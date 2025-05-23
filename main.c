#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>
typedef struct node{
        char *word;
        int height;
        struct node*left;
        struct node*right;
}node;
node* createnode(char* w){
        node*n = malloc (sizeof(node));
        n->word = malloc ( strlen(w) + 1);
        strcpy(n->word , w);
        n->left=n->right=NULL;
        n->height=1;
        return n ;
}
int max(int x , int y){
        if(x>y){
                return x;
        }
        return y;
}
int getHeight(node*n){
       if( n==NULL){
               return 0 ;
       }
       return n->height;
}
void updateHeight(node*n){
        if(n==NULL){
                return ;
        }
        n->height = max(getHeight(n->right) , getHeight(n->left)) +1;
        
}
int getBalance(node*n){
        if (n==NULL) {
                return 0 ;
        }
        return getHeight(n->left)-getHeight(n->right);
}
int count(node* root){
if (root==NULL){
    return 0;
}
    else{
  return 1+count(root->right)+count(root->left);
    }
}
node*findMin(node*root){
        while(root!=NULL&&root->left!=NULL){
         root=root->left;}
          return root;
           }
node* successor(node* root, node* lastVisited) {
    if (lastVisited == NULL)
        return NULL;

    if (lastVisited->right != NULL) {
        node* temp = lastVisited->right;
        while (temp->left != NULL)
            temp = temp->left;
        return temp;
    }

    node* succ = NULL;
    node* current = root;

    while (current != NULL) {
        int x = strcasecmp(lastVisited->word, current->word);
        if (x < 0) {
            succ = current;
            current = current->left;
        } else if (x > 0) {
            current = current->right;
        }
else {
            break;
        }
    }
    return succ;
}
node*rightRotate(node*n){
        node*x=n->left;
        node*y=x->right;
        x->right=n;
        n->left=y;
        updateHeight(n);
        updateHeight(x);
        return x;
}
node*leftRotate(node*n){
        node*x=n->right;
        node*y=x->left;
        x->left=n;
        n->right=y;
        updateHeight(n);
        updateHeight(x);
        return x;
}
node*search(node*root , char*w){
        if(root==NULL){
                return NULL;
        }
        int x = strcasecmp(root->word , w);
        if(x==0){
                return root;
        }
        else if(x>0){
                return search(root->left , w);
        }
        else{
                return search(root->right , w);
        }
}
node*insert( node*root , char*w){
        if(root==NULL){
                return createnode(w);
        }
        node*n=createnode(w);
        int x = strcasecmp(root->word , w);
        if(x>0){
               root->left = insert(root->left , w);
        }
        else if((x<0)){
               root->right=insert(root->right , w);
        }
        else{
                return root;
        }
        updateHeight(root);
        int b = getBalance(root);
        if(b > 1 && strcasecmp(root->left->word , w ) > 0 ){
             return rightRotate(root);
        }
        if(b < -1 && strcasecmp(root->right->word , w ) < 0){
             return leftRotate(root);
        }
        if(b > 1 && strcasecmp(root->left->word , w ) < 0 ){
                root->left=leftRotate(root->left);
                root=rightRotate(root);
        }
       if(b < -1 && strcasecmp(root->right->word , w ) > 0 ){
                root->right=rightRotate(root->right);
                root=leftRotate(root);
       }
       return root;
}
  node*findMax(node*root){
         while(root!=NULL&&root->right!=NULL){
          root=root->right;
         }
           return root;
         }
node* predecessor(node* root, node* lastVisited) {
    node* x = NULL;
    node* y = root;

    while (y != NULL) {
        int cmp = strcasecmp(lastVisited->word, y->word);
        if (cmp > 0) {
            x = y;
            y = y->right;
        }
        else if (cmp < 0) {
            y = y->left;
        }
        else {
            if (y->left != NULL) {
                return findMax(y->left);
            }
            break;
        }
    }
    return x;
}
node* load(char* fileName) {
    FILE *fptr = fopen(fileName, "r");
    if (!fptr) {
        printf("couldn't open file\n");
        return NULL;
    }

    node *root = NULL;
    char word[100];

    while (fgets(word, 100, fptr)) {
        word[strcspn(word, "\n")] = '\0';
        root = insert(root, word);
    }

    printf("Dictionary loaded successfully\n");
    fclose(fptr);
    return root;
}
void printTreeInfo(node *root) {
    if (root == NULL) {
        printf("Tree is empty\n");
        return ;
    }
    printf("\n.............................\n");
    printf("Size = %d", count(root));
    printf("\n.............................\n");
    printf("Height = %d", root->height);
    printf("\n.............................\n");
}
node*lastVisited(node*root,char*word){
    node*current=root;
    node*last=NULL;
    while(current!=NULL){
        last=current;
        int x=strcasecmp(word, current->word);
        if(x<0)
       current=current->left;
        else if (x>0)
       current=current->right;
       else{
               break;
       }
    }
    return last;
}
void printSuggestions(node *root, char *word) {
    node *last = lastVisited(root,word);
    if (last == NULL) {
        printf("no suggestions \n");
        return;
    }

    printf("%s - Incorrect\n", word);
    printf("suggestions: ");

    printf("%s\n", last->word);

    node *pred = predecessor(root ,last);
    node *succ = successor(root, last);
    if (pred!=NULL){
            printf("predecessor of last visited node is %s\n", pred->word);
    }else{
            printf("predecessor of last visited word is NULL\n");
    }
    if(succ!=NULL){
             printf("successor of last visited node is %s ", succ->word);
    }
    else{
            printf("Successor of last visited word is NULL \n");
    }

}
void freeTree(node*root){
        if(root==NULL){
                return ;
        }
        freeTree(root->left);
        freeTree(root->right);
        freeTree(root);
}
int main(){
    node* dict = NULL;
    dict = insert(dict, "apple");
    dict = insert(dict, "banana");
    dict = insert(dict, "cherry");
    dict = insert(dict, "date");
    dict = insert(dict, "elderberry");
    char* testWord = "chary";
    node* result = search(dict, testWord);
    if (result) {
        printf("%s - Correct\n", testWord);
    } else {
        printSuggestions(dict, testWord);
    }
    printTreeInfo(dict);

    return 0;
}