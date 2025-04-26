#pragma once
#include <unordered_set>
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>
using namespace std; using namespace glm;


template<typename T, typename Derived> class InstanceManager {
public:
    static shared_ptr<T> GetOrCreate(const shared_ptr<T>& instance) {
        auto& set = GetInstanceSet();
        if (auto existingInstance = set.find(instance); existingInstance != set.end()) {
            return *existingInstance;
        }
        set.insert(instance);
        return instance;
    }

protected:
    struct Hasher {
        size_t operator()(const shared_ptr<T>& instance) const {
            return Derived::Hash(*instance);
        }
    };
    struct Comparator {
        bool operator()(const shared_ptr<T>& left, const shared_ptr<T>& right) const {
            return Derived::Equals(*left, *right);
        }
    };
    static unordered_set<shared_ptr<T>, Hasher, Comparator>& GetInstanceSet() {
        static unordered_set<shared_ptr<T>, Hasher, Comparator> instanceSet;
        return instanceSet;
    }
};


inline void CombineHashes(size_t& seed, const size_t value) {
    seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}


namespace std {
    template <> struct hash<vec3> {
        size_t operator()(const vec3& vec) const noexcept {
            size_t seed = 0;
            CombineHashes(seed, hash<float>()(vec.x));
            CombineHashes(seed, hash<float>()(vec.y));
            CombineHashes(seed, hash<float>()(vec.z));
            return seed;
        }
    };

    template <> struct hash<vec4> {
        size_t operator()(const vec4& vec) const noexcept {
            size_t seed = 0;
            CombineHashes(seed, hash<float>()(vec.x));
            CombineHashes(seed, hash<float>()(vec.y));
            CombineHashes(seed, hash<float>()(vec.z));
            CombineHashes(seed, hash<float>()(vec.w));
            return seed;
        }
    };

    template <> struct hash<quat> {
        size_t operator()(const quat& quat) const noexcept {
            size_t seed = 0;
            CombineHashes(seed, hash<float>()(quat.x));
            CombineHashes(seed, hash<float>()(quat.y));
            CombineHashes(seed, hash<float>()(quat.z));
            CombineHashes(seed, hash<float>()(quat.w));
            return seed;
        }
    };
} // namespace std
