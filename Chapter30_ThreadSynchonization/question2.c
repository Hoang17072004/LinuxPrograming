
#include<pthread.h>
#include<stdio.h>
#include "../Lib/tlpi_hdr.h"
struct Node{
	char *key;
	char *value;
	struct Node *left;
	struct Node *right;
	pthread_mutex_t lock;
};
typedef struct Node* tree;
void initialize(tree root) {
//	pthread_mutex_lock(&root->lock);
	root->left=root->right=NULL;
	pthread_mutex_init(&root->lock,NULL);
//	pthread_mutex_unlock(&root->lock);
}
void add(tree* root,char *key,char *value) {
	if (*root==NULL) {
		*root=(struct Node*)malloc(sizeof(struct Node));
		initialize(*root);

		(*root)->key=strdup(key);
		(*root)->value=strdup(value);
//		pthread_mutex_unlock(&root->lock);

	} else{
		pthread_mutex_lock(&(*root)->lock);
		 if (strcmp(key,(*root)->key)<0) add(&(*root)->left,key,value);
		else add(&(*root)->right,key,value);
		pthread_mutex_unlock(&(*root)->lock);
	}
}
Boolean lookup(tree root,char *key,void **value) {
	if (root==NULL) return FALSE;
	pthread_mutex_lock(&root->lock);
	if (strcmp(root->key,key)==0){
		*value=root->value;
		pthread_mutex_unlock(&root->lock);
		return TRUE;
	}
	pthread_mutex_unlock(&root->lock);
	return  (lookup(root->left,key,value)||lookup(root->right,key,value)) ;

}
int main() {
	tree root = NULL; // ban đầu rỗng
add(&root, "abc", "123");

}
