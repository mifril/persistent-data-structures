#ifndef EULER_ROUND_TREE_HPP
#define EULER_ROUND_TREE_HPP

#include <memory>

class EulerRoundTree {
private:
    class Node {
        size_t idx;
        std::shared_ptr<Node> next;

        Node(const size_t idx_) : idx(idx_)
        {}
    };

public:
    EulerRoundTree();
    bool insert(const size_t parentIdx, const size_t newIdx);
    bool isAncestor(const size_t first, const size_t second);

private:
    std::shared_ptr<Node> root;
};

#endif // EULER_ROUND_TREE_HPP
