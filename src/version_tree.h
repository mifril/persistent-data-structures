#ifndef VERSION_TREE_H
#define VERSION_TREE_H

#include <iostream>
#include <list>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <limits>
#include <algorithm>
#include <iterator>

template <typename VersionData>
class VersionTree {
private:
    struct Node {
        VersionData data;
        bool inEvent;
        size_t label;

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
            return data == other.data && inEvent == other.inEvent && label == other.label;
        }
        bool operator==(const Node& other) const {
            return data == other.data && inEvent == other.inEvent && label == other.label;
        }
    };

public:
    VersionTree() : _labelsNumber(4), _labelToVersion(_labelsNumber, NONE_VERSION) {
        _init();
    }

    VersionTree(const VersionTree & other) : _events(other._events), _labelsNumber(other._labelsNumber),
            _labelToVersion(other._labelToVersion), _versionToLabel(other._versionToLabel)
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
                auto prevLabel = _getLabel(it->data.version);
                auto next = it;
                ++next;
                auto nextLabel = _getLabel(next->data.version);

                if (nextLabel - prevLabel < 2) {
                    _relabel(prevLabel, nextLabel);
                    prevLabel = _getLabel(it->data.version);
                    nextLabel = _getLabel(next->data.version);
                }
                _setLabel(data.version, prevLabel + 1);

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
        return _getLabel(lv) < _getLabel(rv);
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
        _init();
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
    size_t _labelsNumber;
    std::vector<size_t> _labelToVersion;
    std::unordered_map<size_t, size_t> _versionToLabel;

    static const size_t NONE_VERSION;
    static const double OVERFLOW_THRESHOLD_BASE;

    void _relabel(const size_t firstLabel, const size_t secondLabel) {
        size_t rangeSize = 2;
        while (rangeSize < _labelsNumber) {
            size_t firstRangeNum = firstLabel / rangeSize;
            size_t secondRangeNum = secondLabel / rangeSize;
            if (firstRangeNum == secondRangeNum) {
                size_t rangeStart = rangeSize * firstRangeNum;
                size_t rangeEnd = (rangeSize + 1) * firstRangeNum;
                size_t rangeDensity = _getRangeDensity(rangeStart, rangeEnd);

                double overflowThreshold = std::pow(OVERFLOW_THRESHOLD_BASE, -1 * (long long)rangeSize);
                if (rangeDensity < overflowThreshold) {
                    _relabelRange(rangeStart, rangeEnd);
                    break;
                }
            }
            rangeSize *= 2;
        }
        if (rangeSize >= _labelsNumber) {
            _relabelAll();
        }
    }

    size_t _getRangeDensity(const size_t rangeStart, const size_t rangeEnd) {
        size_t density = 0;
        for (size_t i = rangeStart; i < rangeEnd; ++i) {
            if (_labelToVersion[i] != NONE_VERSION) {
                ++density;
            }
        }
        return density;
    }

    void _relabelRange(const size_t rangeStart, const size_t rangeEnd) {
        std::list<size_t> rangeVersions;
        for (size_t i = rangeStart; i < rangeEnd; ++i) {
            if (_labelToVersion[i] != NONE_VERSION) {
                rangeVersions.push_back(_labelToVersion[i]);
            }
        }

        auto rangeStartIt = _labelToVersion.begin();
        std::advance(rangeStartIt, rangeStart);
        auto rangeEndIt = _labelToVersion.begin();
        std::advance(rangeEndIt, rangeEnd);
        std::fill(rangeStartIt, rangeEndIt, NONE_VERSION);

        for (size_t i = rangeStart; i < rangeEnd; i += 2) {
            if (rangeVersions.empty()) {
                break;
            }
            auto version = rangeVersions.front();
            _labelToVersion[i] = version;
            _versionToLabel[version] = i;
            rangeVersions.pop_front();
        }
    }

    void _relabelAll() {
        std::list<size_t> rangeVersions;
        for (auto version : _labelToVersion) {
            if (version != NONE_VERSION) {
                rangeVersions.push_back(version);
            }
        }

        _labelsNumber *= 2;
        _labelToVersion.resize(_labelsNumber, NONE_VERSION);
        std::fill(_labelToVersion.begin(), _labelToVersion.end(), NONE_VERSION);

        for (size_t i = 0; i < _labelsNumber; i += 2) {
            if (rangeVersions.empty()) {
                break;
            }
            auto version = rangeVersions.front();
            _labelToVersion[i] = version;
            _versionToLabel[version] = i;
            rangeVersions.pop_front();
        }
    }

    void _setLabel(const size_t version, const size_t label) {
        _labelToVersion[label] = version;
        _versionToLabel[version] = label;
    }

    size_t _getLabel(const size_t version) {
        return _versionToLabel[version];
    }

    void _init() {
        _events.push_back(Node(true));
        _events.push_back(Node(false));
        _labelToVersion[0] = 0;
        _versionToLabel[0] = 0;
    }
};

template <typename VersionData>
const size_t VersionTree<VersionData>::NONE_VERSION = std::numeric_limits<size_t>::max();
template <typename VersionData>
const double VersionTree<VersionData>::OVERFLOW_THRESHOLD_BASE = 1.5;

#endif // VERSION_TREE_H
