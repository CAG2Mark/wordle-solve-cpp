from game import Game
from collections import defaultdict

with open("words") as word_f:
    words_all = [x.strip() for x in word_f.readlines()]
with open("possible_words") as possible_f:
    words_possible = [x.strip() for x in possible_f.readlines()]

d = defaultdict(lambda: {})
total = len(words_possible)
for i, answer in enumerate(words_possible):
    if (not i%100):print(i, total)
    G = Game(answer)
    for guess in words_all:
        result = G.guess_word(guess)
        d[guess][answer] = result

with open("guess_table", "w") as f:
    f.write("     ,")
    f.write(",".join(words_possible))
    f.write("\n")
    for guess in d:
        f.write(guess + ",")
        d_ = d[guess]
        f.write(",".join([d_[answer] for answer in words_possible]))
        f.write("\n")

    
    