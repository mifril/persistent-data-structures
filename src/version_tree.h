#ifndef VERSION_TREE_H
#define VERSION_TREE_H

#include <iostream>
#include <list>

class VersionTree {
private:
    struct Node {
        size_t version;
        size_t mark;
        bool inEvent;

        Node(const size_t version_, const bool inEvent_) : version(version_), inEvent(inEvent_)
        {}
    };

public:
    VersionTree();

    /* insert newVersion after parentVersion */
    void insert(const size_t newVersion, const size_t parentVersion);

    void remove(const size_t version);

    /* if lv < rv returns true, else false */
    bool order(const size_t lv, const size_t rv);

private:
    std::list<Node> _events;
};

#endif // VERSION_TREE_H
