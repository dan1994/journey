#pragma once

namespace memory::paging {

enum class PriviledgeLevel : bool { KERNEL = false, USER = true };

enum class AccessType : bool { READ_ONLY = false, READ_WRITE = true };

enum class Present : bool { FALSE = false, TRUE = true };

}  // namespace memory::paging
