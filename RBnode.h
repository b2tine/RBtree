#ifndef RBNODE_H
#define RBNODE_H

#include <exception>
#include <iostream>
#include <iomanip>
#include <memory>




enum class RBcolor { RED, BLACK, ROOT };




template<typename T>
class RBnode:
    public std::enable_shared_from_this<RBnode<T>>
{

    private:

        T data;
        RBcolor color;

        std::shared_ptr<RBnode<T>> left;
        std::shared_ptr<RBnode<T>> right;
        std::weak_ptr<RBnode<T>> parent;


    public:

        explicit RBnode(const T& key)
            : data{key}, color{RBcolor::RED},
            left{}, right{}, parent{}
        {}

        RBnode(const RBnode&) = delete;
        RBnode& operator=(const RBnode&) = delete;
        RBnode(RBnode&&) = delete;
        RBnode& operator=(RBnode&&) = delete;
        
        ~RBnode() = default;


        //factory (static) function and helpers
        static std::shared_ptr<RBnode<T>> createNode(const T& key);
        std::shared_ptr<RBnode<T>> createLeftChild(const T& key);
        std::shared_ptr<RBnode<T>> createRightChild(const T& key);

        //setters
        void setData(const T& newkey);
        void setColor(RBcolor Color);
        void setLeftChild(std::shared_ptr<RBnode<T>> node);
        void setRightChild(std::shared_ptr<RBnode<T>> node);
        void setParent(std::shared_ptr<RBnode<T>> node);

        //observers
        const T Data() const;
        const RBcolor Color() const;
        const std::weak_ptr<const RBnode<T>> LeftChild() const;
        const std::weak_ptr<const RBnode<T>> RightChild() const;
        const std::weak_ptr<const RBnode<T>> Parent() const;
        const std::weak_ptr<const RBnode<T>> Sibling() const;
        const std::weak_ptr<const RBnode<T>> Grandparent() const;
        const std::weak_ptr<const RBnode<T>> Uncle() const;

        //getters
        std::shared_ptr<RBnode<T>> getLeftChild();
        std::shared_ptr<RBnode<T>> getRightChild();
        std::shared_ptr<RBnode<T>> getParent();
        std::shared_ptr<RBnode<T>> getSibling();
        std::shared_ptr<RBnode<T>> getGrandparent();
        std::shared_ptr<RBnode<T>> getUncle();
        std::shared_ptr<RBnode<T>> getMaxPredecessor();
        std::shared_ptr<RBnode<T>> getMinSuccessor();

        //use before observer/getter functions to prevent access violations
        bool isRoot() const;
        bool isLeftChild() const;
        bool isRightChild() const;
        bool hasLeftChild() const;
        bool hasRightChild() const;
        bool hasChild() const;
        bool hasParent() const;
        bool hasSibling() const;
        bool hasGrandparent() const;
        bool hasUncle() const;

        //printers
        void print() const;
        void printParent() const;
        void printLeftChild() const;
        void printRightChild() const;
        void printDetailed() const;
        const std::string ColorString() const;
};





#endif
