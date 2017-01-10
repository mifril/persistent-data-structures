#ifndef VERSION_TREE_H
#define VERSION_TREE_H

#include <iostream>
#include <list>

template <typename VersionData>
class VersionTree {
private:
    struct Node {
        VersionData data;
        bool inEvent;
        size_t mark;

        Node() : data(VersionData()), inEvent(false)
        {}
        Node(const Node& other) : data(other.data), inEvent(other.inEvent)
        {}
        Node(const bool inEvent_) : inEvent(inEvent_)
        {}
        Node(const VersionData & data_, const bool inEvent_)
            : data(data_), inEvent(inEvent_)
        {}

        bool operator==(const Node& other) {
            return data == other.data && inEvent == other.inEvent && mark == other.mark;
        }
        bool operator==(const Node& other) const {
            return data == other.data && inEvent == other.inEvent && mark == other.mark;
        }
    };

public:
    VersionTree() {
        _events.push_back(Node(true));
        _events.push_back(Node(false));
    }

    VersionTree(const VersionTree & other) : _events(other._events)
    {}

    bool operator==(const VersionTree& other) {
        return _events == other._events;
    }
    bool operator==(const VersionTree& other) const {
        return _events == other._events;
    }
    bool operator!=(const VersionTree& other) {
        return _events != other._events;
    }
    bool operator!=(const VersionTree& other) const {
        return _events != other._events;
    }

    /* insert data.version after parentVersion */
    void insert(const VersionData & data, const size_t parentVersion) {
        if (_events.empty()) {
            throw new std::out_of_range("Empty version tree");
        }
        auto it = _events.begin();
        for (; it != _events.end(); ++it) {
            if (it->data.version == parentVersion) {
                it = ++it;
                auto pos = _events.insert(it, Node(data, false));
                pos = _events.insert(pos, Node(data, true));
                break;
            }
        }
        if (it == _events.end()) {
            throw new std::out_of_range("Version tree doesn't contain parent version " + parentVersion);
        }
    }

    void remove(const size_t version) {
        bool wasDelete = false;
        for (auto it = _events.begin(); it != _events.end(); ++it) {
            if (it->data.version == version) {
                auto next = it;
                ++next;
                _events.erase(it);
                it = next;
                // _events always contains 2 entries of one version with diferent inEvent flag values
                if (wasDelete) {
                    break;
                }
                wasDelete = true;
            }
        }
    }

    /* if lv < rv returns true, else false */
    bool order(const size_t lv, const size_t rv) {
        return _getMark(lv) < _getMark(rv);
    }

    // empty version tree's _events contains only 2 entries for starting version
    bool empty() const {
        return _events.size() == 2;
    }

    size_t size() const {
        return _events.size() / 2;
    }

    void clear() {
        _events.clear();
        _events.push_back(Node(true));
        _events.push_back(Node(false));
    }

    const VersionData & getData(const size_t version) const {
        for (auto it = _events.begin(); it != _events.end(); ++it) {
            if (it->data.version == version) {
                return it->data;
            }
        }
        throw new std::out_of_range("No version " + version);
    }

private:
    std::list<Node> _events;

    size_t _getMark(const size_t version) {
        return version;
    }
};

#endif // VERSION_TREE_H
