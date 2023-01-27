from collections import defaultdict

def guess_word(guess, answer):
    d = defaultdict(lambda: 0)
    ret = list("00000")

    for ch in answer:
        d[ch] += 1

    for i, ch in enumerate(answer):
        if guess[i] == answer[i]:
            ret[i] = "2"
            d[ch] -= 1

    for i, ch in enumerate(guess):
        if ret[i] == "2": continue

        if d[ch] != 0:
            ret[i] = "1"
            d[ch] -= 1
        else:
            ret[i] = "0"

    return ''.join(ret)

with open("all_words") as word_f:
    words_all = [x.strip() for x in word_f.readlines()]
with open("guess_words") as possible_f:
    words_possible = [x.strip() for x in possible_f.readlines()]

d = defaultdict(lambda: {})
total = len(words_possible)
for i, answer in enumerate(words_possible):
    if (not i%100):print(i, total)
    for guess in words_all:
        result = guess_word(guess, answer)
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

    
    
