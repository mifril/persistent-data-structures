#include "version_tree.h"

VersionTree::VersionTree() {
    _events.push_back(Node(0, true));
    _events.push_back(Node(0, false));
}

/* insert newVersion after parentVersion */
void VersionTree::insert(const size_t newVersion, const size_t parentVersion) {
    if (_events.empty()) {
        throw new std::out_of_range("Empty version tree");
    }
    auto it = _events.begin();
    for (; it != _events.end(); ++it) {
        if (it->version == parentVersion) {
            it = ++it;
            auto pos = _events.insert(it, Node(newVersion, false));
            pos = _events.insert(pos, Node(newVersion, true));
            break;
        }
    }
    if (it == _events.end()) {
        throw new std::out_of_range("Version tree doesn't contain parent version " + parentVersion);
    }
}

void VersionTree::remove(const size_t version) {
    _events.remove(Node(version, true));
    _events.remove(Node(version, false));
}

/* if lv < rv returns true, else false */
bool VersionTree::order(const size_t lv, const size_t rv) {
    return _getMark(lv) < _getMark(rv);
}

size_t VersionTree::_getMark(const size_t version) {
    return version;
}
