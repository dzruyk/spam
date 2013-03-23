#!/usr/bin/python2.7

alph = [chr(i) for i in range(ord('a'), ord('z') + 1)]
alph_cap = [chr(i) for i in range(ord('A'), ord('Z') + 1)]
digit = [chr(i) for i in range(ord('0'), ord('9') + 1)]


class brute_dict():
    def __init__(self, wordlist = alph + digit, sz = 5):
        self._set_vars_(wordlist, sz);

    def _set_vars_(self, wordlist, sz):
        self.wordlist = wordlist[:];
        self.wlsz = len(self.wordlist)
        self.sz = sz;
        self.arr = [0 for i in range(sz)];

    def __iter__(self):
        return self;
    
    def next(self):
        for i in range(self.sz - 1, -1, -1):
            if self.arr[i] < self.wlsz - 1:
                self.arr[i] += 1
                break
            self.arr[i] = 0;
        if i == 0 and self.arr[0] == 0:
            raise StopIteration
        s = ""
        for i in range(self.sz):
            s += self.wordlist[self.arr[i]];
        return s;

    def get_next(self):
        return;

def runtests():
    
    a = brute_dict(alph + digit, 5);
    for item in a:
        print item
    return;


if __name__ == "__main__":
    runtests();

