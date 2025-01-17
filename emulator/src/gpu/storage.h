#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include <map>
typedef std::pair<uint64_t, uint64_t> region;

enum Access {
    RO, WO, RW
};

struct Storage {

    ~Storage() { delete storage_; }
    Storage(Storage& other) = delete;
    void operator=(const Storage& other) = delete;
    static Storage* get_instance();
    std::vector<std::pair<region, Access>> access{};

    void init(size_t bufferSize);
    std::vector<uint8_t> read_data(uint64_t address,
                                   uint32_t offset,
                                   uint32_t byteSize,
                                   bool safe = false);
    uint32_t read_4_bytes(uint64_t, uint32_t);
    uint64_t read_8_bytes(uint64_t);
    void write_data(uint64_t addr, uint32_t offset, uint8_t value);
    void write_data(uint64_t addr, uint32_t offset, uint16_t value);
    void write_data(uint64_t addr, uint32_t offset, uint32_t value);
    void write_data(uint64_t addr, uint32_t offset, uint64_t value);
    void write_data(uint64_t addr, uint32_t offset, float value);
    void mark_as_ro(uint64_t l, uint64_t r);
    void mark_as_wo(uint64_t l, uint64_t r);
    void mark_as_rw(uint64_t l, uint64_t r);
   private:
    size_t _size;
    std::unique_ptr<uint8_t[]> _data;

    static Storage* storage_;

    Storage() {}

    void validate_addr(uint64_t, Access);
    bool validate_addr_safe(uint64_t);
};
