# For - Then

Usefull when you want to express a condition and its consequence.

```swift
for _ in 0...10 {
    if condition() {
        for _ in 0...10 {
            print("Hello")
        } then {
            print("World")
        }
    }
}
```