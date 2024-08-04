# Return Blocks

Allow to have inner blocks that return values

```rust

let x: i32 = {
	if 1>2 { 1 }
	2
}

print!(x) // 2
```