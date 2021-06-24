#include <iostream>
#include <unicorn/unicorn.h>
#include <fmt/core.h>

struct Region {
	uint64_t start;
	uint64_t end;
	uint32_t perms;
};

constexpr Region regions[] = {
	{ .start = 0x0,        .end = 0x000fffff, .perms = UC_PROT_READ | UC_PROT_WRITE | UC_PROT_EXEC }, // onboard ram
	{ .start = 0x00f80000, .end = 0x00fbffff, .perms = UC_PROT_READ | UC_PROT_WRITE                }, // IO ports
	{ .start = 0x00fc0000, .end = 0x00ffffff, .perms = UC_PROT_READ                 | UC_PROT_EXEC }, // onboard ROM 
};

int main(int argc, char const *argv[]) {
	uc_engine* uc;
	uc_err err;

	err = uc_open(UC_ARCH_M68K, UC_MODE_BIG_ENDIAN, &uc);
	if (err) {
		std::cerr << "failed uc_open(): " << uc_strerror(err) << " (" << err << ")\n";
	}

	size_t res;
	err = uc_query(uc, UC_QUERY_PAGE_SIZE, &res);
	fmt::print("{}: target pagesize: {}\n", err, res);

	// map memory
	for (auto region : regions) {
		fmt::print("map region {:#010x}-{:#010x}\n", region.start, region.end);
		err = uc_mem_map(uc, region.start, region.end - region.start + 1, region.perms);
		if (err) {
			fmt::print(stderr, "failed uc_mem_map(uc, {}, {}, {}): {} ({})\n",
				region.start, region.end - region.start, region.perms,
				uc_strerror(err), err);
		}
	}

	return 0;
}