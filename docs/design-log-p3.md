# Project 3 Design Log
**Author:** <Israel Mattocks> · **P2 code used:** <instructor reference>
**Tools used:** <GPT-6 Luna was used for the purpose of explaning the spec in greater detail and providing an code outline to refine and work off of.>

## 1. Hash table

The hash table uses chaining, because buckets_ is a vector of vectors. When a collision occurs, additional items are "chained" to a single "bucket". Advantages of this method include: 
     - making it so that find() only has to search one "bucket"
     - resizing is easy, because a "bucket" can be moved along with all of its chained contents

The hash table used the FNV-1a function as mentioned in the spec; specifically, the 64-bit version is used. In short, the function XORs each byte before multiplying it by the FNV prime. 

Resizing policy is to double the amount of buckets when the load factor reaches 75% of capacity.

add() takes O(1) time, because the table does not need to be iterated through; a Tool is simply added to the table at its specific index. Linear time is required when resizing, but naturally resizing will become less frequent over time.

## 2. Pointer stability
<!-- Why does a Tool* returned by find() survive a resize in your design? -->
The address of the Tool pointed to by the unique_ptr Tool* does not change, even if the unique_ptr Tool* is moved into a new bucket during resizing.

## 3. Calculator trace
<!-- Trace the values and ops stacks for 2 * (3 + 4) - 5, one token at a
     time, in your own words. -->

- 2 | Pushes 2 to VALUE stack
- * | Pushes * to OPERATION stack
- ( | Pushes ( to OPERATION stack
- 3 | Pushes 3 to VALUE stack
- + | Pushes + to OPERATION stack
- 4 | Pushes 4 to VALUE stack
- ) | Applies + to 3 and 4 resulting in 7; 3 and 4 are removed, + and ( are removed
- - | Applies * to 2 and 7 resulting in 14; 2 and 7 are removed, * is removed and - is pushed to OPERATION stack
- 5 | Pushes 5 to VALUE stack

Finally, - is applied to 14 and 5 resulting in the answer 9 in the VALUE stack.

## 4. Errors as values
<!-- Why do tools, resolve() and calc_evaluate() return errors instead of
     throwing? When IS an exception the right choice? -->
Errors are returned here instead of throwing exceptions because the errors provide data which the model can relay to the user. Explaining the error directly to the user is efficient; the user can adjust their usage accordingly to yield a successful response to the next prompt. Exceptions can be the better choice, however, when the problem does not arise from the user. User-generated problems should be shown to the user; the specific details of other errors should be abstracted or otherwise obscured.

## 5. What I would design differently
