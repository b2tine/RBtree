//gcc option to enable debugging: -D DEBUG
//equivalent to defining the macro below.
#define DEBUG

template<typename T>
void RBtree<T>::makeRoot(const T& key)
{
    auto new_root = RBnode<T>::createNode(key);
    new_root->setColor(RBcolor::ROOT);
    this->root = std::move(new_root);
}

template<typename T>
void RBtree<T>::setRoot(std::shared_ptr<RBnode<T>> node)
{
    node->setColor(RBcolor::ROOT);
    node->setParent(nullptr);
    this->root = std::move(node);
}

template<typename T>
const std::weak_ptr<const RBnode<T>> RBtree<T>::Root() const
{
    return std::weak_ptr<RBnode<T>>(this->root);
}

/*
//1. RED nodes must have BLACK children.
//2. BLACK depth equal for every path.
//3. 
template<typename T>
bool RBtree<T>::CheckInvariants() const
{
    //TODO: Implement this.
}*/

template<typename T>
bool RBtree<T>::isRoot(const std::shared_ptr<const RBnode<T>>& node) const
{
    return node->isRoot() &&
        node == this->root; //could be trouble...
}//need to test thoroughly

template<typename T>
void RBtree<T>::Insert(const T& key)
{
    if( !this->root )
    {
        this->makeRoot(key);
        return;
    }

    auto new_node = InsertRecursive(this->root,key);
    RepairTree(std::move(new_node));
}

template<typename T>
std::shared_ptr<RBnode<T>> RBtree<T>::InsertRecursive(
        const std::shared_ptr<RBnode<T>>& subroot, const T& key)
{
    if( key == subroot->Data() )
    {
        #ifdef DEBUG
            printf("\t\t\t\tDuplicate Data: Was Not Inserted\n");
        #endif
        return subroot;
    }

    if( key < subroot->Data() )
    {
        if( subroot->hasLeftChild() )
            return InsertRecursive(subroot->getLeftChild(),key);
        else
            return subroot->createLeftChild(key);
    }
    else
    {
        if( subroot->hasRightChild() )
            return InsertRecursive(subroot->getRightChild(),key);
        else
            return subroot->createRightChild(key);
    }
    //calls to createLeftChild and createRightChild do not violate
    //constness of subroot, because only the underlying resource is
    //modified and not the shared_ptr itself.
}

template<typename T>
void RBtree<T>::RepairTree(std::shared_ptr<RBnode<T>>&& node)
{
    #ifdef DEBUG
        printf("\t\t\t\tEntered RepairTree:\n");
        //printf("\n"); node->printDetailed();
    #endif

    if( node )
    {
        if( this->isRoot(node) )
            InsertCase1(std::move(node));
        else if( this->isRoot(node->Parent().lock()) ||
                node->Parent().lock()->Color() == RBcolor::BLACK )
            InsertCase2(std::move(node));
        else if( node->hasUncle() )
        {
            if( node->Uncle().lock()->Color() == RBcolor::RED )
                InsertCase3(std::move(node));
            else
                InsertCase4(std::move(node));
        }
        else
            InsertCase4(std::move(node));
    }
    else
    {
        #ifdef DEBUG
            printf("\t\t\t\tError in RepairTree: node is nullptr\n");
        #endif    
    }
}

//node is new root: make color ROOT.....now is no-op
template<typename T>
void RBtree<T>::InsertCase1(std::shared_ptr<RBnode<T>>&& node)
{
    #ifdef DEBUG
        printf("\t\t\t\tEntered InsertCase1:\n");
    #endif
}//no-op

//node's parent is BLACK: do nothing
template<typename T>
void RBtree<T>::InsertCase2(std::shared_ptr<RBnode<T>>&& node)
{
    #ifdef DEBUG
        printf("\t\t\t\tEntered InsertCase2:\n");
    #endif
}//no-op

//TODO: If InsertCase1 and InsertCase2 remain no-op functions, combine them.

//node's parent and uncle are both RED:
//paint them both BLACK and paint node's grandparent RED.
template<typename T>
void RBtree<T>::InsertCase3(std::shared_ptr<RBnode<T>>&& node)
{//Grandparent should exist since cases 1 & 2 were bypassed.
    #ifdef DEBUG
        printf("\t\t\t\tEntered InsertCase3:\n");
    #endif
    node->getParent()->setColor(RBcolor::BLACK);
    node->getUncle()->setColor(RBcolor::BLACK);
    if( !this->isRoot(node->Grandparent().lock()) )
    {
        node = node->getGrandparent();
        node->setColor(RBcolor::RED);
        RepairTree(std::move(node));
    }
}

//node's parent is RED and uncle is BLACK:
//Rotate node into Grandparent position and paint BLACK.
//Its children will both be RED (Grandparent gets painted RED).
//First need to rotate node to exterior of subtree.
template<typename T>
void RBtree<T>::InsertCase4(std::shared_ptr<RBnode<T>>&& node)
{//Grandparent should exist since cases 1 & 2 were bypassed.
    #ifdef DEBUG
        printf("\t\t\t\tEntered InsertCase4:\n");
    #endif
    if( node->Parent().lock()->isLeftChild() )
    {
        if( node->isRightChild() )
            node = this->RotateLeft(node->getParent());
            //RotateLeft returns original parent in new position
    }
    else
    {
        if( node->isLeftChild() )
            node = this->RotateRight(node->getParent());
            //RotateRight returns original parent in new position
    }

    if( node )
        InsertCase4step2(std::move(node));
        //probably should log if node is nullptr...or guarantee not.
}

//Node is on exterior of subtree.
//Rotate Grandparent node and paint RED; paint Parent BLACK.
template<typename T>
void RBtree<T>::InsertCase4step2(std::shared_ptr<RBnode<T>>&& node)
{
    #ifdef DEBUG
        printf("\t\t\t\tEntered InsertCase4step2:\n");
    #endif
    if( node->Parent().lock()->isLeftChild() )
        node = this->RotateRight(node->getGrandparent());
    else
        node = this->RotateLeft(node->getGrandparent());
    
    node->setColor(RBcolor::RED);
    if( !this->isRoot(node->Parent().lock()) )
        node->getParent()->setColor(RBcolor::BLACK);
}

template<typename T>
void RBtree<T>::Delete(std::shared_ptr<RBnode<T>>&& node)
{
    if( node->hasLeftChild() && node->hasRightChild() )
        DeleteCase0(std::move(node));//node has two children
    else
        DeleteCase1(std::move(node));//node guaranteed at most one (non-leaf) child
    //TODO: checking the reference count may be useful for debugging
    //      and/or performance profiling
}

//Node has two (non-leaf) children:
//replace with maxpredecessor,then remove the
//node it was copied from and repair the tree.
template<typename T>
void RBtree<T>::DeleteCase0(std::shared_ptr<RBnode<T>>&& node)
{
    #ifdef DEBUG
        printf("\t\t\t\tEntered DeleteCase0:\n");
    #endif
    auto maxpred = node->getMaxPredecessor();//take maxpredecessor by default for now.
    node->setData(maxpred->Data());//Guaranteed that maxpred not a nullptr?
    DeleteCase1(std::move(maxpred));//maxpred guaranteed at most one (non-leaf/non-null) child
}

//Node has at most one (non-leaf) child (potentially none).
template<typename T>
void RBtree<T>::DeleteCase1(std::shared_ptr<RBnode<T>>&& node)
{
    #ifdef DEBUG
        printf("\t\t\t\tEntered DeleteCase1:\n");
    #endif
    if( node->hasLeftChild() || node->hasRightChild() )
        DeleteCase2(std::move(node));
    else if( node->Color() == RBcolor::BLACK )//node (nonroot) BLACK and no (non-leaf) children
        DeleteCase3(std::move(node));
    else
        DeleteNode(std::move(node));//Node is RED with no children,
                                    //OR Node is the Root with no children
}      

//Node is BLACK and has exactly one RED (non-leaf) child:
//(child has no (non-leaf) children)
template<typename T>
void RBtree<T>::DeleteCase2(std::shared_ptr<RBnode<T>>&& node)
{
    #ifdef DEBUG
        printf("\t\t\t\tEntered DeleteCase2:\n");
    #endif
    auto child = node->hasLeftChild() ?
        node->getLeftChild() : node->getRightChild();//child guaranteed not nullptr?
    node->setData(child->Data());
    DeleteCase1(std::move(child));
}

template<typename T>
void RBtree<T>::DeleteCase3(std::shared_ptr<RBnode<T>>&& node)
{
    #ifdef DEBUG
        printf("\t\t\t\tEntered DeleteCase3:\n");
    #endif
    if( !isRoot(node) )
    {
        if( node->Sibling().lock()->Color() == RBcolor::RED )
            DeleteCase3step2(node);
    }
    DeleteCase4(std::move(node));
}

//Sibling of node to be deleted is RED (passed as argument),
//paint it BLACK and paint its Parent RED,
//then rotate at Parent (performed in opposite order below).
template<typename T>
void RBtree<T>::DeleteCase3step2(std::shared_ptr<RBnode<T>>& node)//pass by lvalue reference
{
    #ifdef DEBUG
        printf("\t\t\t\tEntered DeleteCase3step2:\n");
    #endif
    auto P = node->isLeftChild() ?
        RotateLeft(node->getParent()) : RotateRight(node->getParent());
    //P is original parent in the rotated position.
    P->setColor(RBcolor::RED);//Paint original Parent RED
    if( !isRoot(P->Parent().lock()) )
        P->getParent()->setColor(RBcolor::BLACK);//Paint original Sibling BLACK
}

//check if Node's Parent is RED and Sibling and Sibling's Children
//are all BLACK: make Parent BLACK and Sibling RED - done.
template<typename T>
void RBtree<T>::DeleteCase4(std::shared_ptr<RBnode<T>>&& node)
{
    #ifdef DEBUG
        printf("\t\t\t\tEntered DeleteCase4:\n");
    #endif
    auto S = node->getSibling();
    auto P = node->getParent();
    if( P->Color() == RBcolor::RED &&
        S->Color() == RBcolor::BLACK &&
        !S->hasLeftChild() && !S->hasRightChild() )
    {
        S->setColor(RBcolor::RED);//make Sibling RED
        P->setColor(RBcolor::BLACK);//make Parent BLACK
        if( !node->hasChild() )
            DeleteNode(std::move(node));
    }
    else
    {
        DeleteCase5(std::move(node));
    }
}

template<typename T>
void RBtree<T>::DeleteCase5(std::shared_ptr<RBnode<T>>&& node)
{
    #ifdef DEBUG
        printf("\t\t\t\tEntered DeleteCase5:\n");
    #endif
    auto P = node->getParent();
    auto S = node->getSibling();
    if( S->Color() == RBcolor::BLACK &&
        !S->hasLeftChild() && !S->hasRightChild() )
    {
        if( !node->hasChild() )
            DeleteNode(std::move(node));

        if( P->Color() == RBcolor::BLACK ||
                P->Color() == RBcolor::ROOT )
        {
            S->setColor(RBcolor::RED);//make Sibling RED
            if( !isRoot(P) )
                DeleteCase3(std::move(P));//Rebalance at Parent
        }
    }
    else
    {
        DeleteCase6(std::move(node));
    }
}

//Sibling is BLACK check if its RED child is on interior of subtree
template<typename T>
void RBtree<T>::DeleteCase6(std::shared_ptr<RBnode<T>>&& node)
{
    #ifdef DEBUG
        printf("\t\t\t\tEntered DeleteCase6:\n");
    #endif
    auto S = node->getSibling();
    if( S->Color() == RBcolor::BLACK )//should be guaranteed..
    {
        if( node->isLeftChild() && S->hasLeftChild() )
        {
            if( S->LeftChild().lock()->Color() == RBcolor::RED
                    && S->RightChild().lock()->Color() == RBcolor::BLACK )
            {
                S = RotateRight(std::move(S));//Returns S in rotated position.
                S->setColor(RBcolor::RED);//Paint original Sibling RED
                S->getParent()->setColor(RBcolor::BLACK);//Paint original Sibling's child BLACK
            }
        }
        else if( node->isRightChild() && S->hasRightChild() )
        {
            if( S->RightChild().lock()->Color() == RBcolor::RED
                    && S->LeftChild().lock()->Color() == RBcolor::BLACK )
            {
                S = RotateLeft(std::move(S));//Returns S in rotated position.
                S->setColor(RBcolor::RED);//Paint original Sibling RED
                S->getParent()->setColor(RBcolor::BLACK);//Paint original Sibling's child BLACK
            }
        }
    }
    DeleteCase7(std::move(node));
}

//Sibling's RED non-leaf Child is on exterior of subtree:
//rotate node's parent so that the sibling becomes the Grandparent,
//then Paint the sibling the color of the parent,
//and paint the parent BLACK, and Sibling's non-leaf child BLACK. 
template<typename T>
void RBtree<T>::DeleteCase7(std::shared_ptr<RBnode<T>>&& node)
{
    #ifdef DEBUG
        printf("\t\t\t\tEntered DeleteCase7:\n");
    #endif
    auto P = node->isLeftChild() ?
        RotateLeft(node->getParent()) : RotateRight(node->getParent());
    //RotateLeft and RotateRight return parent in the new rotated position.
    if( !isRoot(P->Parent().lock()) )
        P->getParent()->setColor(P->Color());//sibling color = parent color
    P->setColor(RBcolor::BLACK);//set parent BLACK
    P->getSibling()->setColor(RBcolor::BLACK);//paint exterior child BLACK
    if( !node->hasChild() )
        DeleteNode(std::move(node));
}

template<typename T>
void RBtree<T>::DeleteNode(std::shared_ptr<RBnode<T>>&& node)
{
    if( isRoot(node) )
        this->root.reset();
    else
    {
        if( node->isLeftChild() )
            node->getParent()->setLeftChild(nullptr);
        else
            node->getParent()->setRightChild(nullptr);
    }
    node.reset();
}

template<typename T>
std::shared_ptr<RBnode<T>>
RBtree<T>::RotateLeft(std::shared_ptr<RBnode<T>>&& node)
{
    #ifdef DEBUG
        printf("\t\t\t\tEntered RotateLeft:\n");
    #endif
    if( node->hasRightChild() )
    {
        auto rc = node->getRightChild();
        node->setRightChild(rc->getLeftChild());
        if( isRoot(node) )
        {
            node->setColor(RBcolor::BLACK);
            setRoot(rc);
        }
        else if( node->isLeftChild() )
            node->getParent()->setLeftChild(rc);
        else
            node->getParent()->setRightChild(rc);
        rc->setLeftChild(node);
        return node;
    }
    return {};
    //might be good idea to log when rotation fails.
    //Also, this could be a noexcept function...
}

template<typename T>
std::shared_ptr<RBnode<T>>
RBtree<T>::RotateRight(std::shared_ptr<RBnode<T>>&& node)
{
    #ifdef DEBUG
        printf("\t\t\t\tEntered RotateRight:\n");
    #endif
    if( node->hasLeftChild() )
    {
        auto lc = node->getLeftChild();
        node->setLeftChild(lc->getRightChild());
        if( isRoot(node) )
        {
            node->setColor(RBcolor::BLACK);
            setRoot(lc);
        }
        else if( node->isLeftChild() )
            node->getParent()->setLeftChild(lc);
        else
            node->getParent()->setRightChild(lc);
        lc->setRightChild(node);
        return node;
    }
    return {};
    //might be good idea to log when rotation fails.
    //Also, this could be a noexcept function...
}

template<typename T>
std::shared_ptr<RBnode<T>> RBtree<T>::Find(const T& key)
{
    auto node = this->root;
    while( node )
    {
        if( node->Data() <= key )
        {
            if( node->Data() == key )
                return node;
            else
                node = node->getRightChild();
        }
        else
            node = node->getLeftChild();
    }
    return {};
}

template<typename T>
void RBtree<T>::Print() const
{
    printf("\n\t\t\t\t\t   Current Tree:\n\n");
    if( !this->root )
    {
        printf("\t\t\t\t\t   **EMPTY**\n\n");
        return;
    }
    PrintRecursive(std::weak_ptr<RBnode<T>>(this->root));
}

template<typename T>
void RBtree<T>::PrintRecursive(const std::weak_ptr<const RBnode<T>>& node) const
{
    if( !node.expired() )
    {
        auto snode = node.lock();
        if( snode->hasLeftChild() )
            PrintRecursive(snode->LeftChild());

        printf("\t\t\t\t\t\b\b"); snode->print();

        if( snode->hasRightChild() )
            PrintRecursive(snode->RightChild());
    }
}



