// src/tools/tool_registry.cpp — YOU IMPLEMENT (spec §4.3).
//
// A hand-written hash table. No std::unordered_map / std::map / std::set.
// Every function below is a stub so the starter compiles. Replace each body.

#include "tools/tool_registry.h"

#include <cstdint>
#include <utility>

// Hash size is initialized to 16
ToolRegistry::ToolRegistry() : buckets_(16) {}
ToolRegistry::~ToolRegistry() = default;   // unique_ptrs in your buckets free the tools

// hash function is using 64-bit FNV-1a, processing each byte in order
std::size_t ToolRegistry::hash(std::string_view key) noexcept {
    std::uint64_t value = 1469598103934665603ULL;
    for (unsigned char byte : key) {
        value ^= byte;
        value *= 1099511628211ULL;
    }
    return static_cast<std::size_t>(value);
}

// First checks to ensure that the tool exists and is not already in the hash table
bool ToolRegistry::add(std::unique_ptr<Tool> tool) {
    if (!tool) return false;

    std::string name = tool->name();
    std::size_t index = hash(name) % buckets_.size();
    for (const Entry& entry : buckets_[index]) {
        if (entry.name == name) return false;
    }

    // Resizes the hash map (to double its previous size) if size exceeds 75% of capacity
    if (size_ + 1 > buckets_.size() * 3 / 4) {
        std::vector<std::vector<Entry>> new_buckets(buckets_.size() * 2);
        for (auto& bucket : buckets_) {
            for (Entry& entry : bucket) {
                const std::size_t new_index = hash(entry.name) % new_buckets.size();
                new_buckets[new_index].push_back(std::move(entry));
            }
        }
        buckets_.swap(new_buckets);
        index = hash(name) % buckets_.size();
    }

    buckets_[index].push_back(Entry{std::move(name), std::move(tool)});
    ++size_;
    return true;
}

Tool* ToolRegistry::find(std::string_view name) const {
    const std::size_t index = hash(name) % buckets_.size();
    for (const Entry& entry : buckets_[index]) {
        if (std::string_view(entry.name) == name) return entry.tool.get();
    }
    return nullptr;
}

std::size_t ToolRegistry::size() const noexcept {
    return size_;
}

void ToolRegistry::for_each(const std::function<void(const Tool&)>& fn) const {
    for (const auto& bucket : buckets_) {
        for (const Entry& entry : bucket) {
            fn(*entry.tool);
        }
    }
}

std::size_t ToolRegistry::bucket_count() const noexcept {
    return buckets_.size();
}

// Returns a double equal to the size of the hash table / how many buckets are occupied
double ToolRegistry::load_factor() const noexcept {
    if (buckets_.empty()) return 0.0;
    return static_cast<double>(size_) / buckets_.size();
}

// Iterates through each bucket to check its size; the largest one has its size returned
std::size_t ToolRegistry::max_probe_length() const {
    std::size_t longest_chain = 0;
    for (const auto& bucket : buckets_) {
        if (bucket.size() > longest_chain) longest_chain = bucket.size();
    }
    return longest_chain;
}
