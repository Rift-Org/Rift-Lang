# Custom Truthy Values

Ability to define your own truthy values for a custom class.

```python

class MyCustomClass:
    def __bool__(self):
        return False

    def __truthy__(self):
        return True

...

if MyCustomClass():
    print("Truthy")

```