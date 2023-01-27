# wordle-solve-cpp

![SCreenshot of the program in the terminal](https://user-images.githubusercontent.com/55091936/215071721-ca0ef803-582c-42bb-97f6-5e3052ab8990.png)

C++ solver for Wordle that I wrote out of boredom. Main features include:

* Fast (nearly instant guess generation in most scenarios)
* Supports multiple boards
* Visual UI in your terminal

## How does it work?

Just like many other solvers, this just uses information theory to get the best guess. In the case of multiple boards, note that we can add up the entropy of multiple boards to compute the total entropy if we assume the boards act independently. In reality this may not be the case. For example, some implementations may require that different boards do not share the same answer, but it should be good enough.

The main optimisation is to first pre-compute the guess colours. For every word that could be an answer, the guess colours of every allowed word is computed and stored in a table. This is then loaded.

The second optimisation is to re-label words as numbers, so that I can access data using a vector (array) instead of a hashmap. This greatly improves the speed because, while array random access and hashmap access are both O(1), hashmap access has a much larger constant factor.

# Running
Run:
```bash
git clone https://github.com/CAG2Mark/wordle-solve-cpp
cd wordle-solve-cpp

python3 scripts/gen_combos.py # Note: You can use pypy3 to have this run faster.

make # Or make -j4 to use four threads

build/main # Runs the program
```
