#include "euler_round_tree.hpp"

EulerRoundTree::EulerRoundTree() : root(std::make_shared<Node>())
{}

bool EulerRoundTree::insert(const size_t parentIdx, const size_t newIdx) {
    auto cur = root;
    while (cur && cur->idx != parentIdx) {
        cur = cur->next;
    }
    if (cur) {
        auto newNode = std::make_shared<Node>(newIdx);
        auto parentCopy = std::make_shared<Node>(parentIdx);
        newNode->next = parentCopy;
        parentCopy->next = cur->next;
        cur->next = newNode;
        return true;
    }
    return false;
}
bool EulerRoundTree::isAncestor(const size_t first, const size_t second) {
    auto cur = root;
    bool wasFirst = false;

    while (cur && cur->idx != first) {
        if (cur->idx == second) {
            return false;
        }
        cur = cur->next;
    }
    while (cur && cur->idx != second) {
        cur = cur->next;
    }
    return cur == nullptr;
}
