//gcc option to enable debugging: -D DEBUG
//equivalent to defining the macro below.
#define DEBUG

template<typename T>
std::shared_ptr<RBnode<T>> RBnode<T>::createNode(const T& key)
{
    return std::make_shared<RBnode<T>>(key);
}//Note: this is a (public) static member function; the factory function

template<typename T>
std::shared_ptr<RBnode<T>> RBnode<T>::createLeftChild(const T& key)
{
    auto lc = createNode(key);
    this->setLeftChild(lc);
    return std::move(lc);
}

template<typename T>
std::shared_ptr<RBnode<T>> RBnode<T>::createRightChild(const T& key)
{
    auto rc = createNode(key);
    this->setRightChild(rc);
    return std::move(rc);
}

template<typename T>
void RBnode<T>::setData(const T& newkey)
{
    this->data = newkey;
}

template<typename T>
void RBnode<T>::setColor(RBcolor Color)
{
    this->color = Color;
}

template<typename T>
void RBnode<T>::setLeftChild(std::shared_ptr<RBnode<T>> node)
{
    if( node )
        node->setParent(this->shared_from_this());
    this->left = std::move(node);
}

template<typename T>
void RBnode<T>::setRightChild(std::shared_ptr<RBnode<T>> node)
{
    if( node )
        node->setParent(this->shared_from_this());
    this->right = std::move(node);
}

template<typename T>
void RBnode<T>::setParent(std::shared_ptr<RBnode<T>> node)
{
    this->parent = std::move(node);
}

template<typename T>
const T RBnode<T>::Data() const
{
    return this->data;
}

template<typename T>
const RBcolor RBnode<T>::Color() const
{
    auto node = this->shared_from_this();
    if( node )
        return this->color;
    else
        return RBcolor::BLACK;
    //TODO: See if RBtree functions can be refactored now that
    //      a null node (leaf) returns RBcolor::BLACK.
    //      This was originally added to get RBtree::DeleteCase6
    //      to work correctly (no other cases affected).
}

template<typename T>
const std::weak_ptr<const RBnode<T>> RBnode<T>::LeftChild() const
{
    return std::weak_ptr<RBnode<T>>(this->left);
}

template<typename T>
const std::weak_ptr<const RBnode<T>> RBnode<T>::RightChild() const
{
    return std::weak_ptr<RBnode<T>>(this->right);
}

template<typename T>
const std::weak_ptr<const RBnode<T>> RBnode<T>::Parent() const
{
    return this->parent;
}

template<typename T>
const std::weak_ptr<const RBnode<T>> RBnode<T>::Grandparent() const
{
    if( this->hasParent() )
    {
        auto P = this->Parent().lock();
        return P->Parent();
    }
    return {};  //return default constructed function return type
                //i.e. weak_ptr to underlying nullptr
}

template<typename T>
const std::weak_ptr<const RBnode<T>> RBnode<T>::Sibling() const
{
    if( this->hasParent() )
    {
        auto P = this->Parent().lock();
        return (P->LeftChild().lock() == this->shared_from_this()) ?
            P->RightChild() : P->LeftChild();
    }
    return {};
}

template<typename T>
const std::weak_ptr<const RBnode<T>> RBnode<T>::Uncle() const
{
    if( this->hasParent() )
    {
        auto P = this->Parent().lock();
        return P->Sibling();
    }
    return {};
}

//not sure of exact criteria to determine yet..
template<typename T>
bool RBnode<T>::isRoot() const
{
    if( !this->hasParent() )
        return this->color == RBcolor::ROOT;
    return false;
}//might be ok though ... need to test thoroughly


template<typename T>
bool RBnode<T>::isLeftChild() const
{
    if( this->hasParent() )
    {
        auto P = this->Parent().lock();
        return P->LeftChild().lock() == this->shared_from_this();
    }
    return false;
}

template<typename T>
bool RBnode<T>::isRightChild() const
{
    if( this->hasParent() )
    {
        auto P = this->Parent().lock();
        return P->RightChild().lock() == this->shared_from_this();
    }
    return false;
}

template<typename T>
bool RBnode<T>::hasLeftChild() const
{
    return (this->left) ? true : false;
}

template<typename T>
bool RBnode<T>::hasRightChild() const
{
    return (this->right) ? true : false;
}

template<typename T>
bool RBnode<T>::hasChild() const
{
    return this->hasLeftChild() || this->hasRightChild();
}

template<typename T>
bool RBnode<T>::hasParent() const
{
    return (!this->parent.expired()) ? true : false;
}

template<typename T>
bool RBnode<T>::hasSibling() const
{
    if( this->hasParent() )
    {
        auto P = this->Parent().lock();
        return (P->LeftChild().lock() == this->shared_from_this()) ?
            P->hasRightChild() : P->hasLeftChild();
    }
    return false;
}

template<typename T>
bool RBnode<T>::hasGrandparent() const
{
    if( this->hasParent() )
    {
        auto P = this->Parent().lock();
        return P->hasParent();
    }
    return false;
}

template<typename T>
bool RBnode<T>::hasUncle() const
{
    if( this->hasParent() )
    {
        auto P = this->Parent().lock();
        return P->hasSibling();
    }
    return false;
}

template<typename T>
std::shared_ptr<RBnode<T>> RBnode<T>::getLeftChild()
{
    return this->left;
}

template<typename T>
std::shared_ptr<RBnode<T>> RBnode<T>::getRightChild()
{
    return this->right;
}

template<typename T>
std::shared_ptr<RBnode<T>> RBnode<T>::getParent()
{
    if( this->hasParent() )
        return this->parent.lock();
    return {};
}

template<typename T>
std::shared_ptr<RBnode<T>> RBnode<T>::getGrandparent()
{
    if( this->hasParent() )
    {
        auto P = this->getParent();
        return P->getParent();
    }
    return {};
}

template<typename T>
std::shared_ptr<RBnode<T>> RBnode<T>::getSibling()
{
    if( this->hasParent() )
    {
        auto P = this->getParent();
        return (P->getLeftChild() == this->shared_from_this()) ?
            P->getRightChild() : P->getLeftChild();
    }
    return {};
}

template<typename T>
std::shared_ptr<RBnode<T>> RBnode<T>::getUncle()
{
    if( this->hasParent() )
    {
        auto P = this->getParent();
        return P->getSibling();
    }
    return {};
}

template<typename T>
std::shared_ptr<RBnode<T>> RBnode<T>::getMaxPredecessor()
{
    if( this->hasLeftChild() )
    {
        auto maxpred = this->getLeftChild();
        while( maxpred->hasRightChild() )
            maxpred = maxpred->getRightChild();
        return std::move(maxpred);
    }
    return {};
}

template<typename T>
std::shared_ptr<RBnode<T>> RBnode<T>::getMinSuccessor()
{
    if( this->hasRightChild() )
    {
        auto minsucc = this->getRightChild();
        while( minsucc->hasLeftChild() )
            minsucc = minsucc->getLeftChild();
        return std::move(minsucc);
    }
    return {};
}

template<typename T>
void RBnode<T>::print() const
{
    std::cout << std::setw(5) << this->data << "\t";
    std::cout << std::left << std::setw(15) << this->ColorString() << "\n";
}

template<typename T>
void RBnode<T>::printParent() const
{
    if( this->hasParent() )
        this->parent.lock()->print();
    else
    {
        printf("\tNo Parent\n");
        //std::cout << "\t";
        //std::cout << std::left << std::setw(15) << "No Parent\n";
    }
}

template<typename T>
void RBnode<T>::printLeftChild() const
{
    if( this->hasLeftChild() )
        this->left->print();
    else
    {
        printf("\tNo Left Child\n");
        //std::cout << "\t";
        //std::cout << std::left << std::setw(15) << "No Left Child\n";
    }
}

template<typename T>
void RBnode<T>::printRightChild() const
{
    if( this->hasRightChild() )
        this->right->print();
    else
    {
        printf("\tNo Right Child\n");
        //std::cout << "\t";
        //std::cout << std::left << std::setw(15) << "No Right Child\n";
    }
}

template<typename T>
void RBnode<T>::printDetailed() const
{
    
    printf("\t\t\t\t\t\tPARENT:\t"); this->printParent(); 
    printf("\t\t\t\t\t\tNODE:\t"); this->print(); 
    printf("\t\t\t\t\t\tLEFT:\t"); this->printLeftChild(); 
    printf("\t\t\t\t\t\tRIGHT:\t"); this->printRightChild(); 
    printf("\n");
}
    /*
     * TODO: Figure out the formatting for this:
    std::cout << std::setfill('\t');
    std::cout << std::setw(5) << "\t" << std::right << std::setw(8) << "PARENT: ";
    this->printParent();
    std::cout << std::setw(5) << "\t" << std::right << "NODE: ";
    this->print();
    std::cout << std::setw(5) << "\t" << std::right << "LEFT: ";
    this->printLeftChild();
    std::cout << std::setw(5) << "\t" << std::right << "RIGHT: ";
    this->printRightChild();*/

template<typename T>
const std::string RBnode<T>::ColorString() const
{
    std::string colorstring =
        this->color == RBcolor::RED ? "RED" : "BLACK";
    
    if( this->isRoot() )
    {
        colorstring += " \t ROOT";
    }
    else if( this->parent.lock()->isRoot() )
    {
        colorstring += this->isLeftChild() ?
            " \t LEFT SUBROOT" : " \t RIGHT SUBROOT";
    }
    return std::move(colorstring);
}







