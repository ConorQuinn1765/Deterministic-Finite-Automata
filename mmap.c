#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mmap.h"

const int MMAP_MAX_STRING = 1024;

typedef struct node Node;

struct node
{
    char* key;
    MAP value;
    Node* left;
    Node* right;
    int height;
};

/* **********************************************
 *         Helper Function Declarations
 * **********************************************/
Node* mmapCreateNode(char* key);
extern int max(int a, int b);
extern int height(Node* pNode);
extern int getBalance(Node* pNode);
extern Node* leftRotate(Node* pNode);
extern Node* rightRotate(Node* pNode);
extern Node* treeMin(Node* pNode);

/* **********************************************
 *           MMAP Function Definitions
 * **********************************************/
bool mmapInsert(MMAP* hNode, char* key)
{
    if(hNode)
    {
        Node* pNode = (Node*)*hNode;
        if(!pNode)
        {
            *hNode = mmapCreateNode(key);
            return true;
        }
        
        if(strncmp(key, pNode->key, strlen(key)) < 0)
            mmapInsert((MMAP)&pNode->left, key);
        else if(strncmp(key, pNode->key, strlen(key)) > 0)
            mmapInsert((MMAP)&pNode->right, key);
        else
            return false;
        
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
        
        return true;
    }
    return false;
}

bool mmapSetValue(MMAP* hNode, char* key, MAP hMap)
{
    if(hNode)
    {
        Node* pNode = (Node*)*hNode;
        if(!pNode)
        {
            printf("mmapSetValue - Invalid argument passed\n");
            return false;   
        }
        
        if(strncmp(key, pNode->key, strlen(key)) < 0)
            return mmapSetValue((MMAP)&pNode->left, key, hMap);
        else if(strncmp(key, pNode->key, strlen(key)) > 0)
            return mmapSetValue((MMAP)&pNode->right, key, hMap);
        else
        {
            pNode->value = hMap;
            *hNode = pNode;
            return true;   
        }
    }
    return false;   
}

bool mmapRemove(MMAP* hNode, char* key)
{
    if(hNode)
    {
        Node* pNode = (Node*)*hNode;
        if(pNode)
        {
            if(strncmp(key, pNode->key, strlen(key)) < 0)
                mmapRemove((MMAP)&pNode->left, key);
            else if(strncmp(key, pNode->key, strlen(key)) > 0)
                mmapRemove((MMAP)&pNode->right, key);   
            
            if(!pNode)
                return false;
            
            if(strncmp(key, pNode->key, strlen(key)) == 0)
            {
                if(pNode->left == NULL)
                {
                    Node* temp = pNode->right;
                    free(pNode->key);
                    mapDestroy(&pNode->value);
                    free(pNode);
                    pNode = temp;
                    *hNode = temp;
                }
                else if(pNode->right == NULL)
                {
                    Node* temp = pNode->left;
                    free(pNode->key);
                    mapDestroy(&pNode->value);
                    free(pNode);
                    pNode = temp;
                    *hNode = temp;
                }
                else
                {
                    Node* temp = treeMin(pNode->right);
                    
                    memset(pNode->key, 0, strlen(pNode->key));
                    strncpy(pNode->key, temp->key, strlen(temp->key));
                    
                    mapDestroy(&pNode->value);
                    pNode->value = temp->value;
                    
                    mmapRemove((MMAP)&pNode->right, temp->key);
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

void mmapKeys(MMAP hNode, VECTOR hVector)
{
    Node* pNode = (Node*)hNode;
    if(pNode)
    {
        vectorInsert(hVector, pNode->key);
        
        return mmapKeys((MMAP)pNode->left, hVector);
        return mmapKeys((MMAP)pNode->right, hVector);
    }
}

MAP mmapFind(MMAP hNode, char* key)
{
    Node* pNode = (Node*)hNode;
    if(pNode)
    {
        if(strncmp(key, pNode->key, strlen(key)) < 0)
            return mmapFind((MMAP)pNode->left, key);
        else if(strncmp(key, pNode->key, strlen(key)) > 0)
            return mmapFind((MMAP)pNode->right, key);
        else
            return pNode->value;
    }
    return NULL;
}

void mmapDestroy(MMAP* phNode)
{
    if(phNode)
    {
        Node* pNode = (Node*)*phNode;
        if(pNode)
        {
            mmapDestroy((MMAP)&pNode->left);
            mmapDestroy((MMAP)&pNode->right);
            free(pNode->key);
            mapDestroy(&pNode->value);
            free(pNode);
        }   
    }
}

void mmapPreOrderTraversal(MMAP hNode)
{
    Node* pNode = (Node*)hNode;
    if(pNode)
    {
        printf("{%s", pNode->key);
        mapPreOrderTraversal(pNode->value);
        printf("}\n");
        
        mmapPreOrderTraversal((MMAP)pNode->left);
        mmapPreOrderTraversal((MMAP)pNode->right);   
    }
}

void mmapInOrderTraversal(MMAP hNode)
{
    Node* pNode = (Node*)hNode;
    if(pNode)
    {
        mmapInOrderTraversal((MMAP)pNode->left);
        
        printf("{%s", pNode->key);
        mapInOrderTraversal(pNode->value);
        printf("}\n");
        
        mmapInOrderTraversal((MMAP)pNode->right);   
    }
}

void mmapPostOrderTraversal(MMAP hNode)
{
    Node* pNode = (Node*)hNode;
    if(pNode)
    {
        mmapPostOrderTraversal((MMAP)pNode->left);
        mmapPostOrderTraversal((MMAP)pNode->right);
        
        printf("{%s", pNode->key);
        mapPostOrderTraversal(pNode->value);
        printf("}\n");
    }
}

/* **********************************************
 *         Helper Function Definitions
 * **********************************************/
Node* mmapCreateNode(char* key)
{
    Node* pNode = malloc(sizeof(Node));
    
    if(!pNode)
    {
        fprintf(stderr, "mmapCreateNode - Failed to allocate memory for new node\n");
        return NULL;
    }
        
    pNode->key = malloc(sizeof(char) * MMAP_MAX_STRING);
    if(!pNode->key)
    {
        fprintf(stderr, "mmapCreateNode - Failed to allocate space for key\n");
        free(pNode);
        return NULL;    
    }
    memset(pNode->key, 0, MMAP_MAX_STRING);
    strncpy(pNode->key, key, strlen(key));
    
    pNode->value = NULL;
    pNode->left = NULL;
    pNode->right = NULL;
    pNode->height = 0;
    
    return pNode;
}