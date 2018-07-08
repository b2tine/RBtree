#include "RBnode.h"
#include "RBnode.cpp"
#include "RBtree.h"
#include "RBtree.cpp"


void find_menu(RBtree<int>& rbt, std::shared_ptr<RBnode<int>>&& node);


int main(int argc, char** argv)
{
    RBtree<int> rbt;

    int qflag = 1;
    while( qflag == 1 )
    {
        int key;
        int option;
        char confirm[8];

        printf("\n");
        printf("1. Insert\n");
        printf("2. Find\n");
        printf("3. Print\n");
        printf("4. Check Invariants\n");
        printf("5. Exit\n");
        
        printf("\nMake a selection: ");
        scanf("%d",&option);
        switch(option)
        {
            case 1:
                printf("\nEnter integer to insert: ");
                scanf("%d",&key);
                printf("\n..Inserting..\n\n");
                rbt.Insert(key);
                break;

            case 2:
                {
                    printf("\nEnter integer to find: ");
                    scanf("%d",&key);
                    printf("\n..Searching..\n");
                    auto node = rbt.Find(key);
                    if( node )
                    {
                        printf("\n\tNode found\n\n");
                        find_menu(rbt,std::move(node));
                    }
                    else
                        printf("\n\tNode Not Found\n");
                    break;
                }

            case 3:
                rbt.Print();
                break;

            case 4:
                printf("\n\tThis feature has not been implemented.\n");
                break;

            case 5:
                printf("\nExit? (y/n) ");
                scanf("%s",confirm);
                if( confirm[0] == 'y' || confirm[0] == 'Y' )
                {
                    printf("\n..Exiting..\n\n");
                    qflag = 0;
                }
                break;

            default:
                printf("\nEnter a valid selection.\n\n");
                break;
        }


    }

    return 0;

}



void find_menu(RBtree<int>& rbt, std::shared_ptr<RBnode<int>>&& node)
{
    int qflag = 1;
    while(qflag == 1)
    {
        printf("\n");
        printf("1. Print Node Details\n");
        printf("2. Get Parent\n");
        printf("3. Get Left Child\n");
        printf("4. Get Right Child\n");
        printf("5. Delete Node\n");
        printf("6. Do Nothing\n");
        
        int option;
        printf("\nMake a selection: ");
        scanf("%d",&option);
        switch(option)
        {
            case 1:
                printf("\n");
                node->printDetailed();
                break;
            
            case 2:
                if( node->hasParent() )
                    node = node->getParent();
                else
                    printf("\t\t\t\tNo Parent\n");
                break;

            case 3:
                if( node->hasLeftChild() )
                    node = node->getLeftChild();
                else
                    printf("\t\t\t\tNo Left Child\n");
                break;

            case 4:
                if( node->hasRightChild() )
                    node = node->getRightChild();
                else
                    printf("\t\t\t\tNo Right Child\n");
                break;

            case 5:
                printf("\n..Deleting..\n");
                rbt.Delete(std::move(node));
                qflag = 0;
                break;

            case 6:
                qflag = 0;
                break;

            default:
                printf("\nEnter a valid selection.\n\n");
                break;
        }
    }
}





