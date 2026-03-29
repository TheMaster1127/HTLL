## Mangling Rules

### Local Variables (inside functions)
- **Integer local:** `__HTLL_flocal_<func>_<var>`
- **Array local:** `__HTLL_flocal_<func>_<var>`

### Local Variables (inside main)
- **Integer local:** `__HTLL_mlocal_<var>`
- **Array local:** `__HTLL_mlocal_<var>`

### Global Variables
- **No mangling** — keep original name.

### Array Parameters
- **Replaced by global buffer:** `__HTLL_param_<func>_<param>`

### Return Values
- **Array return:** global buffer `__HTLL_ret_<func>`  
- **Integer return:** stored in `rax` (or a hidden global like `__HTLL_ret_int_<func>`)

### Function Calls
- **Before call:** copy actual array arguments into `__HTLL_param_<func>_<param>`.
- **After call:** if the function returns an array, copy `__HTLL_ret_<func>` into the target variable.
- **If function returns integer:** move `rax` into the target variable.

### No Mangling Rules without `main`.

1. **No `main` keyword** → no mangling. Everything keeps its original name. Perfect for simple scripts or when you don't need functions.

2. **`main` keyword present** → mangling applies. This happens when you define functions (since you need `main` as the entry point). Locals inside functions, array parameters, return buffers — all get mangled to avoid collisions.

So the presence of `main` acts as a flag: "this program has structured functions, so we need name protection."
