#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <unordered_set>
#include <memory>


namespace pixelforge::management {

    /// Base class to manage instances of a specific type, ensuring
    /// that only one instance of each unique object is present.
    /// @tparam InstanceT The type of instance being managed.
    /// @tparam DerivedManager The derived manager class implementing hashing and comparison.
    template<typename InstanceT, typename DerivedManager>
    class InstanceManager {
    public:
        /// Retrieves an existing instance or creates a new one if it doesn't exist.
        /// @param instance Shared pointer to the instance to retrieve or create.
        /// @return Shared pointer to the existing or newly created instance.
        static std::shared_ptr<InstanceT> getOrCreate(const std::shared_ptr<InstanceT>& instance) {
            auto& set = getInstanceSet();
            if (auto existingInstance = set.find(instance); existingInstance != set.end()) {
                return *existingInstance;
            }
            set.insert(instance);
            return instance;
        }

        /// Clears the complete instance registry.
        static void clear() {
            getInstanceSet().clear();
        }

    protected:
        /// Custom hash function for instances of the type InstanceT
        /// using the Hash()-function of the derived instance manager.
        struct Hasher {
            /// Computes the hash value for a given instance.
            /// @param instance Shared pointer to the instance to hash.
            /// @return The computed hash value.
            size_t operator()(const std::shared_ptr<InstanceT>& instance) const {
                return DerivedManager::hash(*instance);
            }
        };

        /// Custom comparator function for instances the of type InstanceT
        /// using the Equals()-function of the derived instance manager.
        struct Comparator {
            /// Compares two given instances for equality.
            /// @param left The first instance to compare.
            /// @param right The second instance to compare.
            /// @return True if the instances are equal, false otherwise.
            bool operator()(const std::shared_ptr<InstanceT>& left, const std::shared_ptr<InstanceT>& right) const {
                return DerivedManager::equals(*left, *right);
            }
        };

        ///Retrieves the set of instances used for managing their uniqueness.
        ///@return Reference to the unordered set of instances.
        static std::unordered_set<std::shared_ptr<InstanceT>, Hasher, Comparator>& getInstanceSet() {
            static std::unordered_set<std::shared_ptr<InstanceT>, Hasher, Comparator> instanceSet;
            return instanceSet;
        }
    };

} // namespace pixelforge::management


/// Combines multiple hash values into a single hash value.
/// @param seed The current seed value for the hash.
/// @param value The new value to combine into the hash.
inline void combineHashes(size_t& seed, const size_t value) { //
    seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}


namespace std {

    /// Specialization of `std::hash` for `glm::vec3`.
    template <> struct hash<glm::vec3> {
        /// Computes the hash value for a 3D vector.
        /// @param vec The 3D vector to hash.
        /// @return The computed hash value.
        size_t operator()(const glm::vec3& vec) const noexcept {
            size_t seed = 0;
            combineHashes(seed, hash<float>()(vec.x));
            combineHashes(seed, hash<float>()(vec.y));
            combineHashes(seed, hash<float>()(vec.z));
            return seed;
        }
    };


    /// Specialization of `std::hash` for `glm::vec4`.
    template <> struct hash<glm::vec4> {
        /// Computes the hash value for a 4D vector.
        /// @param vec The 4D vector to hash.
        /// @return The computed hash value.
        size_t operator()(const glm::vec4& vec) const noexcept {
            size_t seed = 0;
            combineHashes(seed, hash<float>()(vec.x));
            combineHashes(seed, hash<float>()(vec.y));
            combineHashes(seed, hash<float>()(vec.z));
            combineHashes(seed, hash<float>()(vec.w));
            return seed;
        }
    };


    /// Specialization of `std::hash` for `glm::quat`.
    template <> struct hash<glm::quat> {
        /// Computes the hash value for a quaternion.
        /// @param quat The quaternion to hash.
        /// @return The computed hash value.
        size_t operator()(const glm::quat& quat) const noexcept {
            size_t seed = 0;
            combineHashes(seed, hash<float>()(quat.x));
            combineHashes(seed, hash<float>()(quat.y));
            combineHashes(seed, hash<float>()(quat.z));
            combineHashes(seed, hash<float>()(quat.w));
            return seed;
        }
    };

} // namespace std
