#!/usr/bin/python2.7

import sys

alph = [chr(i) for i in range(ord('a'), ord('z') + 1)]
alph_cap = [chr(i) for i in range(ord('A'), ord('Z') + 1)]
digit = [chr(i) for i in range(ord('0'), ord('9') + 1)]


class brute_dict():
    """
        brute dict - class for brute force password list generation
        After initialisation returns iterator, that return next string from
        brute dictionary
        Sample usage:
            iter = brute_dict()
            for item in iter:
                print item
    """
    def __init__(self, wordlist = alph + digit, sz = 5, start = None):
        """
            sets wordlist, lenght, start string of brute list
        """
        self._set_vars_(wordlist, sz, start);

    def _set_vars_(self, wordlist, sz, start):
        self.wordlist = wordlist[:];
        self.wlsz = len(self.wordlist)
        self.sz = sz;
        self.set_start_symbol(start);
        #for first time
        self.arr[self.sz - 1] -= 1
    
    def set_start_symbol(self, start):
        if start is None:
            self.arr = [0 for i in range(self.sz)];
            return
        if len(start) > self.sz:
            start = start[0:self.sz]
        else:
            start = start.ljust(self.sz, self.wordlist[0])

        self.arr = []
        for ch in start:
            try:
                ret = self.wordlist.index(ch)
            except ValueError:
                self.arr.append(0)
            else:
                self.arr.append(ret)

            
    def __iter__(self):
        return self;
    
    def next(self):
        for i in range(self.sz - 1, -2, -1):
            if i == -1:
                raise StopIteration
            if self.arr[i] < self.wlsz - 1:
                self.arr[i] += 1
                break
            self.arr[i] = 0;
        s = ""
        for i in range(self.sz):
            s += self.wordlist[self.arr[i]];
        return s;

def runtests():
    i = 0
    total = 0
    for i in range(2, 3): 
            a = brute_dict(alph + digit, i, "sb");
            for item in a:
                total += 1
                if total % 1000000 == 0:
                    sys.stderr.write("%d elems traversed, %s\n" % (total, item))
                print item


if __name__ == "__main__":
    runtests();

