#local imports

imports resolve to only that scope

```

@import std/core

func main() {
	// code 1...

	@import std/sec/hash {
		// utilize hash methods
	}

	// code 2
}

```