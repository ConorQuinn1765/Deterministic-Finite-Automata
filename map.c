#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"

typedef struct node Node;

struct node
{
    char* key;
    char* value;
    Node* left;
    Node* right;
    int height;
};

/* **********************************************
 *         Helper Function Declarations
 * **********************************************/
Node* createNode(char* key, char* value);
int max(int a, int b);
int height(Node* pNode);
int getBalance(Node* pNode);
Node* leftRotate(Node* pNode);
Node* rightRotate(Node* pNode);
Node* treeMin(Node* pNode);

/* **********************************************
 *           MAP Function Definitions
 * **********************************************/
bool mapInsert(MAP* hNode, char* key, char* value)
{
    if(hNode)
    {
        Node* pNode = (Node*)*hNode;
        if(!pNode)
        {
            *hNode = createNode(key, value);
            return true;
        }
        
        if(strncmp(key, pNode->key, strlen(key)) < 0)
            mapInsert((MAP)&pNode->left, key, value);
        else
            mapInsert((MAP)&pNode->right, key, value);
        
        pNode->height = height(pNode);
        
        int balance = getBalance(pNode);
        
        // Left Left Case
        if(balance > 1 && strncmp(key, pNode->key, strlen(key)) < 0)
        {
            *hNode = rightRotate(pNode);
            return true;
        }

        // Right Right Case
        if(balance < -1 && strncmp(key, pNode->key, strlen(key)) >= 0)
        {
            *hNode = leftRotate(pNode);
            return true;
        }

        // Left Right Case
        if(balance > 1 && strncmp(key, pNode->key, strlen(key)) >= 0)
        {
            pNode->left = leftRotate(pNode->left);
            *hNode = rightRotate(pNode);
            return true;
        }

        // Right Left Case
        if(balance < -1 && strncmp(key, pNode->key, strlen(key)) < 0)
        {
            pNode->right = rightRotate(pNode->right);
            *hNode = leftRotate(pNode);
            return true;
        }
    }
    return false;
}

bool mapRemove(MAP* hNode, char* key)
{
    if(hNode)
    {
        Node* pNode = (Node*)*hNode;
        if(pNode)
        {
            if(strncmp(key, pNode->key, strlen(key)) < 0)
                mapRemove((MAP)&pNode->left, key);
            else if(strncmp(key, pNode->key, strlen(key)) > 0)
                mapRemove((MAP)&pNode->right, key);   
            
            if(!pNode)
                return false;
            
            if(strncmp(key, pNode->key, strlen(key)) == 0)
            {
                if(pNode->left == NULL)
                {
                    Node* temp = pNode->right;
                    free(pNode->key);
                    free(pNode->value);
                    free(pNode);
                    pNode = temp;
                    *hNode = temp;
                }
                else if(pNode->right == NULL)
                {
                    Node* temp = pNode->left;
                    free(pNode->key);
                    free(pNode->value);
                    free(pNode);
                    pNode = temp;
                    *hNode = temp;
                }
                else
                {
                    Node* temp = treeMin(pNode->right);
                    
                    memset(pNode->key, 0, strlen(pNode->key));
                    strncpy(pNode->key, temp->key, strlen(temp->key));
                    
                    memset(pNode->value, 0, strlen(pNode->value));
                    strncpy(pNode->value, temp->value, strlen(temp->value));
                    
                    mapRemove((MAP)&pNode->right, temp->key);
                }
            }
            
            if(!pNode)
                return true;
                
            pNode->height = height(pNode);
        
            int balance = getBalance(pNode);
            
            // Left Left Case
            if(balance > 1 && getBalance(pNode->left) >= 0)
            {
                *hNode = rightRotate(pNode);
                return true;
            }

            // Right Right Case
            if(balance < -1 && getBalance(pNode->right) <= 0)
            {
                *hNode = leftRotate(pNode);
                return true;
            }

            // Left Right Case
            if(balance > 1 && getBalance(pNode->left) < 0)
            {
                pNode->left = leftRotate(pNode->left);
                *hNode = rightRotate(pNode);
                return true;
            }

            // Right Left Case
            if(balance < -1 && getBalance(pNode->right) > 0)
            {
                pNode->right = rightRotate(pNode->right);
                *hNode = leftRotate(pNode);
                return true;
            }
            
            return true;
        }
    }
    return false;
}

char* mapFind(MAP hNode, char* key)
{
    Node* pNode = (Node*)hNode;
    if(pNode)
    {
        if(strncmp(key, pNode->key, strlen(key)) < 0)
            return mapFind((MAP)pNode->left, key);
        else if(strncmp(key, pNode->key, strlen(key)) > 0 && pNode->right == NULL)
            return NULL;
        else if(strncmp(key, pNode->key, strlen(key)) > 0)
            return mapFind((MAP)pNode->right, key);
        else
            return pNode->value;
    }
    return NULL;
}

void mapDestroy(MAP* phNode)
{
    if(phNode)
    {
        Node* pNode = (Node*)*phNode;
        if(pNode)
        {
            mapDestroy((MAP)&pNode->left);
            mapDestroy((MAP)&pNode->right);
            free(pNode->key);
            free(pNode->value);
            free(pNode);
        }   
    }
}

void preOrderTraversal(MAP hNode)
{
    Node* pNode = (Node*)hNode;
    if(pNode)
    {
        printf("{%s, %s} - %d\n%p\n%p\n\n", pNode->key, pNode->value, pNode->height, pNode->left, pNode->right);
        preOrderTraversal((MAP)pNode->left);
        preOrderTraversal((MAP)pNode->right);   
    }
}

void inOrderTraversal(MAP hNode)
{
    Node* pNode = (Node*)hNode;
    if(pNode)
    {
        inOrderTraversal((MAP)pNode->left);
        printf("{%s, %s}\n", pNode->key, pNode->value);
        inOrderTraversal((MAP)pNode->right);   
    }
}

void postOrderTraversal(MAP hNode)
{
    Node* pNode = (Node*)hNode;
    if(pNode)
    {
        postOrderTraversal((MAP)pNode->left);
        postOrderTraversal((MAP)pNode->right);
        printf("{%s, %s}\n", pNode->key, pNode->value);
    }
}

/* **********************************************
 *         Helper Function Definitions
 * **********************************************/
Node* createNode(char* key, char* value)
{
    Node* pNode = malloc(sizeof(Node));
    
    if(!pNode)
    {
        fprintf(stderr, "createNode - Failed to allocate memory for new node\n");
        return NULL;
    }
        
    pNode->key = malloc(sizeof(char) * 1024);
    if(!pNode->key)
    {
        fprintf(stderr, "createNode - Failed to allocate space for key\n");
        free(pNode);
        return NULL;    
    }
    
    pNode->value = malloc(sizeof(char) * 1024);
    if(!pNode->key)
    {
        fprintf(stderr, "createNode - Failed to allocate space for value\n");
        free(pNode->key);
        free(pNode);
        return NULL;    
    }
    
    strncpy(pNode->key, key, strlen(key));
    strncpy(pNode->value, value, strlen(value));
    pNode->left = NULL;
    pNode->right = NULL;
    pNode->height = 0;
    
    return pNode;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int height(Node* pNode)
{
    if(!pNode)
        return 0;
    
    return max(height(pNode->left), height(pNode->right)) + 1;
}

int getBalance(Node* pNode)
{
    if(!pNode)
        return 0;
    
    return height(pNode->left) - height(pNode->right);
}

Node* leftRotate(Node* pNode)
{
    Node* n1 = pNode->right;
    Node* n2 = n1->left;
    
    n1->left = pNode;
    pNode->right = n2;
    
    pNode->height = height(pNode);
    n1->height = height(n1);
    
    return n1;
}

Node* rightRotate(Node* pNode)
{
    Node* n1 = pNode->left;
    Node* n2 = n1->right;
    
    n1->right = pNode;
    pNode->left = n2;
    
    pNode->height = height(pNode);
    n1->height = height(n1);
    
    return n1;
}

Node* treeMin(Node* pNode)
{
    if(!pNode)
        return NULL;
        
    while(pNode->left != NULL)
        pNode = pNode->left;
        
    return pNode;
}