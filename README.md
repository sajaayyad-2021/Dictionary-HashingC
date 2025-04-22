# Dictionary Management System in C 🧠📚

This project implements a basic dictionary (key-value storage) system in the C programming language using a hash table with collision resolution strategies: **linear probing** and **quadratic probing**.

---

## ✨ Features
- Add new key-value pairs
- Delete entries
- Search for a key
- Update existing values
- Rehash table entries
- Choose between linear and quadratic probing

---

## ⚙️ How it works

- The hash function maps strings to indices using ASCII sums.
- Collisions are resolved by probing:
  - Linear probing: check next available index.
  - Quadratic probing: use offset pattern (i²).
- Deleted entries are marked and skipped during insert/search.
- Rehashing reinserts all valid entries after clearing the table.

---

## 🧪 Sample Input & Output

### Example 1 - Linear Probing
```plaintext
Choose probing method:
1. Linear
2. Quadratic
> 1

--- Dictionary Menu ---
1. Insert
2. Delete
3. Search
4. Update
5. Rehash
6. Display
0. Exit
> 1

Enter key: apple
Enter value: fruit
Inserted successfully!

> 1
Enter key: banana
Enter value: yellow
Inserted successfully!

> 3
Enter key to search: banana
Found! Value = yellow

> 6
Index 10: [Key: apple, Value: fruit]
Index 15: [Key: banana, Value: yellow]
