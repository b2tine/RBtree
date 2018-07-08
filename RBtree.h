#ifndef RBTREE_H
#define RBTREE_H

template<typename T>
class RBtree
{
    private:

        std::shared_ptr<RBnode<T>> root;

        void makeRoot(const T& key);
        void setRoot(std::shared_ptr<RBnode<T>> node);
        //std::shared_ptr<RBnode<T>> getRoot();
        
        std::shared_ptr<RBnode<T>> InsertRecursive(
                const std::shared_ptr<RBnode<T>>& subroot, const T& key);

        void RepairTree(std::shared_ptr<RBnode<T>>&& node);
        void InsertCase1(std::shared_ptr<RBnode<T>>&& node);
        void InsertCase2(std::shared_ptr<RBnode<T>>&& node);
        void InsertCase3(std::shared_ptr<RBnode<T>>&& node);
        void InsertCase4(std::shared_ptr<RBnode<T>>&& node);
        void InsertCase4step2(std::shared_ptr<RBnode<T>>&& node);

        void DeleteCase0(std::shared_ptr<RBnode<T>>&& node);
        void DeleteCase1(std::shared_ptr<RBnode<T>>&& node);
        void DeleteCase2(std::shared_ptr<RBnode<T>>&& node);
        void DeleteCase3(std::shared_ptr<RBnode<T>>&& node);
        void DeleteCase3step2(std::shared_ptr<RBnode<T>>& node);//pass by lvalue reference
        void DeleteCase4(std::shared_ptr<RBnode<T>>&& node);
        void DeleteCase5(std::shared_ptr<RBnode<T>>&& node);
        void DeleteCase6(std::shared_ptr<RBnode<T>>&& node);
        void DeleteCase7(std::shared_ptr<RBnode<T>>&& node);

        void DeleteNode(std::shared_ptr<RBnode<T>>&& node);

        std::shared_ptr<RBnode<T>> RotateLeft(std::shared_ptr<RBnode<T>>&& node);
        std::shared_ptr<RBnode<T>> RotateRight(std::shared_ptr<RBnode<T>>&& node);
        
        void PrintRecursive(const std::weak_ptr<const RBnode<T>>& node) const;


    public:

        RBtree() = default;
        RBtree(const RBtree&) = delete;
        RBtree(RBtree&&) = delete;
        RBtree& operator=(const RBtree&) = delete;
        RBtree& operator=(RBtree&&) = delete;
        
        ~RBtree() = default;

        //bool CheckInvariants() const;
        const std::weak_ptr<const RBnode<T>> Root() const;
        bool isRoot(const std::shared_ptr<const RBnode<T>>& node) const;
        void Insert(const T& key);
        std::shared_ptr<RBnode<T>> Find(const T& key);
        void Delete(std::shared_ptr<RBnode<T>>&& node);
        void Print() const;
};



#endif
