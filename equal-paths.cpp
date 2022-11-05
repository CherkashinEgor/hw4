#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int height(Node * root){
    if(root == NULL){
        return 0;
    }

    int left = height(root->left);
    int right = height(root->right);
    if(left >= right){
        return left + 1; 
    } else {
        return right + 1;
    }
}

bool equalPaths(Node * root)
{
    // Add your code below
    if(root == NULL){
        return true;
    }
    int left = height(root->left);
    int right = height(root->right);
    if(left == 0 && right == 1){
        return true;
    }
    if(right == 0 && left == 1){
        return true;
    }
    if(abs(left-right) >= 1){
        return false;
    }
    return true;
}

